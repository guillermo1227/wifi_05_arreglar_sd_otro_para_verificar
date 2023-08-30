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



#ifndef _MANAGER_TIMER_VH_H
#define _MANAGER_TIMER_VH_H

#include "wiced.h"
#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "GPIO/manager_gpio_vh.h"

void Time_reboot(void* arg);
#define TIMER_TIME (1000)

static wiced_timer_t timerHandle_reset;

uint32_t c_down_net=1;
uint8_t c_down_TASK=1;



static wiced_timed_event_t guardian;
static wiced_timed_event_t guardian2;


static wiced_result_t guardian_v( void );
static wiced_result_t guardian_V2( void );


uint8_t last_count_l=1;
uint8_t last_count_v=1;
//wiced_bool_t any_proximity=10;

wiced_bool_t first_seen_vh=WICED_TRUE;
wiced_bool_t first_seen_lm=WICED_TRUE;
wiced_bool_t first_seen_both=WICED_FALSE;

uint8_t rz_c=1;
void publish30sec(void* arg);
void init_all_timer(){
/*  Initialize timer*/
    WPRINT_APP_INFO( ("Initialize timer\n") );

    wiced_rtos_init_timer(&timerHandle_reset, TIMER_TIME, Time_reboot, NULL);
//
    wiced_rtos_start_timer(&timerHandle_reset);

//        wiced_rtos_init_timer(&publishTimer, TIMER_TIME, publish30sec, NULL);
//        wiced_rtos_start_timer(&publishTimer);

        wiced_rtos_register_timed_event( &guardian, WICED_NETWORKING_WORKER_THREAD, &guardian_v, 1200, 0 );
        wiced_rtos_register_timed_event( &guardian2, WICED_NETWORKING_WORKER_THREAD, &guardian_V2, 1000, 0 );

}


static wiced_result_t guardian_v( void ){

//    WPRINT_APP_INFO( ("LAMPARA Last count %d  actual count %d \r\n",last_count_l,count_l) );
//    WPRINT_APP_INFO( ("VEHICULO Last count %d  actual count %d \r\n",last_count_v,count_v) );


    WPRINT_APP_INFO( ("its worked timed event\n") );
    if((count_v>=1)&&(first_seen_vh==WICED_FALSE)){
        if(last_count_v<count_v){
            first_seen_vh=WICED_TRUE;
        }
        else{
            first_seen_vh=WICED_FALSE;
        }
    }

    if((count_v>=1)&&(first_seen_vh==WICED_TRUE)){
        first_seen_vh=WICED_FALSE;
        if(silent==WICED_FALSE){
            buzz(150,0);
        }
    }



    if(((count_l>=1)||(count_v>=1))&&(proximity==1)){
        if(silent==WICED_FALSE){
            buzz(150,1);
        }
    }

    last_count_v=count_v;

    if(button_down_net){
        Down_net();
        wiced_rtos_delay_milliseconds( 100);
        wiced_framework_reboot();
    }
//
////    first_seen_lm=WICED_FALSE;
//
//    WPRINT_APP_INFO( ("FRIST LAMP %d \r\n",first_seen_lm) );
//    WPRINT_APP_INFO( ("FRIST VEHC %d \r\n",first_seen_vh) );
//



    }

static wiced_result_t guardian_V2( void ){

//    WPRINT_APP_INFO( ("LAMPARA Last count %d  actual count %d \r\n",last_count_l,count_l) );
//    WPRINT_APP_INFO( ("VEHICULO Last count %d  actual count %d \r\n",last_count_v,count_v) );

    check_sound();

//    printf("gateway: %d\n",data_adq.h_rate);

    WPRINT_APP_INFO( ("its worked timed event\n") );
    if((count_l>=1)&&(first_seen_lm==WICED_FALSE)){
        if(last_count_l<count_l){
            first_seen_lm=WICED_TRUE;
        }
        else{
            first_seen_lm=WICED_FALSE;
        }
    }

    if((count_l>=1)&&(first_seen_lm==WICED_TRUE)){
            first_seen_lm=WICED_FALSE;
            if(silent==WICED_FALSE){
                buzz(85,0);
            }
        }
    last_count_l=count_l;

    }

/*************** Timer to publish weather data every 30sec ***************/
void publish30sec(void* arg)
{
//    wiced_gpio_output_high(PWM3);
//    wiced_gpio_output_high(PWM2);
    }

//static wiced_result_t warrior_buz_thread( void ){
//
//
//    if((count_v>=1)&&(first_seen_vh==WICED_FALSE)){
//        if(last_count_v!=count_v){
//            first_seen_vh=WICED_TRUE;
//        }
//        else{
//            first_seen_vh=WICED_FALSE;
//        }
//    }
//    if((count_l>=1)&&(first_seen_lm==WICED_FALSE)){
//        if(last_count_l!=count_l){
//            first_seen_lm=WICED_TRUE;
//        }
//        else{
//            first_seen_lm=WICED_FALSE;
//        }
//    }
//
//    if((count_v>=1)&&(first_seen_vh==WICED_TRUE)){
//        buzz(160,0);
//        first_seen_vh=WICED_FALSE;
//    }
//    else if((count_l>=1)&&(first_seen_lm==WICED_TRUE)){
//        buzz(80,0);
//        first_seen_lm=WICED_FALSE;
//    }
//    else if((count_v>=1)&&(count_l>=1)&&(first_seen_both==WICED_TRUE)){
//        buzz(50,0);
//        first_seen_both=WICED_FALSE;
//    }
//    else{
//
//    }
//
//    if(((count_l>=1)||(count_v>=1))&&(proximity==1)){
//        buzz(180,1);
//    }
//
//    last_count_v=count_v;
//    last_count_l=count_l;
//
//    }

void Time_reboot(void* arg){
    wiced_bool_t upnet=WICED_FALSE;
//                     c_down_net=c_down_net+1;
//    WPRINT_APP_INFO( ("down %d \r\n",c_down_net) );

     if(main_c==3){
         upnet=wiced_network_is_up(WICED_STA_INTERFACE);
         if(upnet==WICED_FALSE){
                 WPRINT_APP_INFO( ("down %d \r\n",c_down_net) );
                 c_down_net=c_down_net+1;
             if(c_down_net==7200){
                 wiced_framework_reboot();
             }
         }
         else{
//             wiced_gpio_output_high(Sat_WiFi);
//             wiced_gpio_output_high(Sat_WiFi);
//             platform_gpio_output_high(Sat_WiFi);

             c_down_net=0;
             if((send_data_task==WICED_FALSE)){
                 c_down_TASK=c_down_TASK+1;
                 if((c_down_TASK>=15)){
                     wiced_framework_reboot();
                 }
             }

             else{
                 c_down_TASK=0;
                 send_data_task=WICED_FALSE;
             }

//             wiced_gpio_output_high(Sat_WiFi);
//             wiced_gpio_output_high(Sat_WiFi);
//             platform_gpio_output_high(Sat_WiFi);
//             blink_vehicle();

         }
     }

     wiced_bool_t on_off_b;
     wiced_gpio_input_get_bool(GPIO_ON_OF,&on_off_b);

     if(on_off_b==WICED_FALSE){
         c_silent=c_silent+1;
         if((c_silent==2)&&(silent==WICED_FALSE)){
             _sound_flag=WICED_TRUE;
             silent=WICED_TRUE;
             frist_seen_silent=WICED_TRUE;
             wiced_rtos_set_semaphore(&displaySemaphore);

         }
         else if((c_silent==2)&&(silent==WICED_TRUE)){
             silent=WICED_FALSE;
             frist_seen_silent=WICED_TRUE;
             _sound_flag=WICED_TRUE;
             wiced_rtos_set_semaphore(&displaySemaphore);

         }
     }
     else{
         c_silent=0;
     }

}


#endif  /* stdbool.h */
