/**********************************************************************************
* Copyright (C) 2018-2021 LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
* All rights reserved.
*
* This document is the property of LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
* It is considered confidential and proprietary.
*
* This document may not be reproduced or transmitted in any form,
* in whole or in part, without the express written permission of
* LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
*
*********************************************************************************/


#ifndef _DATA_CONTROL_H
#define _DATA_CONTROL_H

#include "wiced.h"
#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

wiced_bool_t fallen_f =WICED_FALSE;
wiced_bool_t risk_z=WICED_FALSE;

unsigned char _lateral_veh[2];
unsigned char _lateral_lam[2];


void lcd_data_update(unsigned char* buffer_in,uint8_t *c_v,uint8_t *c_l,wiced_bool_t *proximity){

    unsigned char str_switch[3];
    char *cvl;
    int x=0;
    char delim[] = ",";     //establece como  realizara el split
    unsigned char str_split[128];
    int count_lcd_q[3 ];

    unsigned char temp[8];
    memset(temp,NULL,8);


//    WPRINT_APP_INFO( (">>VBT \r\n") );
//    WPRINT_APP_INFO( ("%s \r\n",buffer_in) );

    strncpy(str_switch,buffer_in,3);
    strcpy(str_split,&buffer_in[3]);

//    WPRINT_APP_INFO( ("Data split[3] = %s <\r\n",str_switch) );

//    WPRINT_APP_INFO( ("Data split[3] to end = %s \r\n",str_split) );

    if(strstr(str_switch,"CVL")){
        char *cvl1 = strtok(str_split, delim);
        while(cvl1 != NULL)
        {
            count_lcd_q[x]= atoi(cvl1);

//            strncpy(temp,cvl1,strlen(cvl1));
            cvl1=strtok(NULL, delim);
//
////            printf("Valor1> %s\n",temp);
//
//            char * token1 = strtok(temp, "-");
////            printf("Valor2> %s\n",token1);
//
//            token1 = strtok(NULL, "-");
////            printf("Valor3 >%s\n",token1);
//
//            if(x==0){
//                 strncpy(_lateral_veh,token1,2);
////                 printf("Valor4 >%s\n",_lateral_veh);
//                 }
//            if(x==1){
//                 strncpy(_lateral_lam,token1,2);
////                 printf("Valor5 >%s\n",_lateral_lam);
//             }
           x++;

        }
        x=0;
        *c_v=count_lcd_q[0];
        *c_l=count_lcd_q[1];
        *proximity=count_lcd_q[2];
//        WPRINT_APP_INFO( ("strtok -> %d and %d \r\n",(*c_v),(*c_l)) );
        wiced_rtos_set_semaphore(&displaySemaphore);


    }

}


void lcd_fallen_update(unsigned char* buffer_in,uint8_t *fallen){
    unsigned char str_temp[17];
    unsigned char str_switch[3];
    char *cvl;
    int x=0;
    char delim[] = ",";     //establece como  realizara el split
    unsigned char str_split[128];

    strncpy(str_switch,buffer_in,4);
    strcpy(str_split,&buffer_in[4]);

    int fal;

    int t=0;
    char fall_at[2];

    if(strstr(str_switch,"BNM:")){

            unsigned char *cvf = strtok(str_split, delim);
            while(cvf != NULL){
                switch (t) {
                    case 0:
                    break;
                    case 1:
                        strcpy(str_temp,cvf);
                        if((strstr(str_temp,"BEAC"))){
                           risk_z=WICED_TRUE;
                        }
                    break;
                    case 2:
                    break;
                    case 3:
                        strcpy(fall_at,cvf);
                        fal=atoi(fall_at);
                        *fallen=fal;
                        if(fal==1){
                            fallen_f=WICED_TRUE;
                        }
//                        WPRINT_APP_INFO(("\r\n\r\n\r\nProcess: %d \r\n",*fallen));
                        break;
                    default:
                        break;
                }
                t++;
                cvf=strtok(NULL, delim);
            }
            t=0;
        }


    wiced_rtos_set_semaphore(&displaySemaphore);


}






#endif  /* stdbool.h */
