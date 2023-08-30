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



#ifndef _MAIN_UART_H
#define _MAIN_UART_H

#include "wiced.h"
#include "stdbool.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "BIOMETRICS/biometric.h"

#include "waf_platform.h"

#define  MAC_OTA   "12:23:34:45:56:67"


//#include "TXT/Defines.h"
#include "TXT/manager_menssage_vh.h"
#include "DATA/Data_control.h"
//#include "TIMER/manager_timer_vh.h"
//#include "NET/manager_tcp_client_vh.h"


static wiced_thread_t UART_M3;


typedef struct data_in
{
     unsigned char type [17];
     unsigned char mac_bt[17];
     unsigned char Date[17];
     unsigned char rssi[4];
     unsigned char fallen[2];


}dataa;

struct data_in data_btt [100+10];


typedef struct
{
    uint8_t mac_rewrite[6]; /**< Unique 6-byte MAC address */
} bt_mc_cyp;

bt_mc_cyp mac_Re;

bt_mc_cyp re_mac(wiced_mac_t mac){
    bt_mc_cyp mm;
    int v;
    int c_c=0;
    for(int v=5;v>=0;v--){
        if(mac.octet[v]==0){
            mm.mac_rewrite[v]=c_c+1;
            c_c=c_c+1;
        }
        else
            mm.mac_rewrite[v]=mac.octet[v];
    }
    return mm;
}


wiced_uart_config_t uart_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};
#define RX_BUFFER_SIZE    64
#define limit_data   100

wiced_ring_buffer_t rx_buffer;
DEFINE_RING_BUFFER_DATA( uint8_t, rx_data, RX_BUFFER_SIZE)

char c;
uint32_t expected_data_size = 1;
unsigned char rx_buffer3[128];
char recive3;



char uart3[64];
int k;

uint8_t lcd_fallen=5;
uint8_t count_v=1;
uint8_t count_l=1;
uint8_t* proximity=5;
wiced_bool_t Evacaution=WICED_FALSE;



void SEND_OTA(unsigned char* buffer_in ){

    unsigned char s_Mac_W[20];
    unsigned char str_split[128];
    strcpy(str_split,&buffer_in[4]);

    if(strstr(str_split,MAC_OTA)){
        wiced_dct_restore_factory_reset( );
        wiced_waf_app_set_boot( DCT_FR_APP_INDEX, PLATFORM_DEFAULT_LOAD );
        wiced_framework_reboot();
    }

}

void main_uart(wiced_thread_arg_t arg){
        char str;
        while(1){
//            WPRINT_APP_INFO( ("mutex \n") );

        /* Wait for user input. If received, echo it back to the terminal */
        while ( wiced_uart_receive_bytes( WICED_UART_1, &c, &expected_data_size, NEVER_TIMEOUT   ) == WICED_SUCCESS )
        {

//            wiced_uart_transmit_bytes( WICED_UART_1, &c, 1);

            if(c == '\n'){
                if(k==0){
                    memset(&rx_buffer3,'\0',128);
                    memset(&rx_buffer,'\0',100);
                    memset(&c,'\0',k);
                    k=0;
                    expected_data_size=1;
                }
                else{
                    sprintf(uart3,"%s\n",rx_buffer3);
                    wiced_uart_transmit_bytes( WICED_UART_1, uart3, strlen(uart3));

                    lcd_data_update(rx_buffer3,&count_v,&count_l,&proximity);
                    lcd_fallen_update(rx_buffer3,&lcd_fallen);
                    data_bt_send(rx_buffer3);
                    SEND_OTA(rx_buffer3);

                    save_data(rx_buffer3, &data_adq, identify_device(rx_buffer3));
//                    printf("heart rate %d\n",data_adq.h_rate);
//                    printf("Uart: %s\n",define_variable( &data_adq, 2));
//                    data_adq.h_rate=20;

                    memset(&rx_buffer3,'\0',128);
                    memset(&rx_buffer,'\0',100);
                    memset(&c,'\0',k);
                    k=0;
                    expected_data_size=1;
                }

            }
            else if(recive3 == '\r'){}
            else{ rx_buffer3[k++] = c; }

            expected_data_size = 1;

            }
        }

        wiced_rtos_delay_milliseconds(1); // Yield control when button is not pressed
    }



void uart_int(){
    /* Initialise ring buffer */
//       ring_buffer_init(&rx_buffer, rx_data, RX_BUFFER_SIZE );

       /* Initialise UART. A ring buffer is used to hold received characters */
       wiced_uart_init( WICED_UART_1, &uart_config, NULL );
   //    wiced_gpio_output_low(WICE_GPIO_12);

       /* Send a test string to the terminal */
       wiced_uart_transmit_bytes( WICED_UART_1, TEST_STR, sizeof( TEST_STR ) - 1 );


       wiced_rtos_create_thread(&UART_M3, THREAD_BASE_PRIORITY+2, "UART MAIN", main_uart, THREAD_STACK_SIZE, NULL);
}

#endif  /* stdbool.h */
