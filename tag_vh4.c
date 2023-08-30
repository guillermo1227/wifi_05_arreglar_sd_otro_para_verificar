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



#include "wiced.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"


/* RTOS global constructs */
static wiced_semaphore_t displaySemaphore;
static wiced_semaphore_t tcpGatewaySemaphore;
static wiced_semaphore_t tcpReceptionSemaphore;

static wiced_mutex_t i2cMutex;
static wiced_mutex_t pubSubMutex;
static wiced_queue_t pubQueue;
static wiced_timer_t publishTimer;
static wiced_thread_t displayThreadHandle;
static wiced_thread_t publishThreadHandle;
static wiced_thread_t subscribeThreadHandle;

static wiced_thread_t commandThreadHandle;


uint8_t main_c=0;


/* Queue configuration */
#define MESSAGE_SIZE        (2)
#define QUEUE_SIZE          (50)

/* Thread parameters */
#define THREAD_BASE_PRIORITY    (10)
#define THREAD_STACK_SIZE       (8192)



#include "u8g_arm.h"
#include "NET/gateway.h"
#include "NET/Init_Net_vh.h"
#include "TXT/manager_menssage_vh.h"
#include "UART/main_uart.h"
#include "LCD/lcd_interface.h"
#include "GPIO/manager_gpio_vh.h"
#include "TIMER/manager_timer_vh.h"
#include "NET/wireless_config.h"
#include "NET/manager_net.h"


/*************** Function Prototypes ***************/
/* ISRs */

/* Threads and timer functions */
void getWeatherDataThread(wiced_thread_arg_t arg);
void commandThread(wiced_thread_arg_t arg);
void publish30sec(void* arg);


void application_start( ){

    wiced_init();   /* Initialize the WICED device */

    /* Setup Thread Control functions */
    wiced_rtos_init_mutex(&i2cMutex);
    wiced_rtos_init_mutex(&pubSubMutex);
    wiced_rtos_init_semaphore(&displaySemaphore);
    wiced_rtos_init_queue(&pubQueue, NULL, MESSAGE_SIZE, QUEUE_SIZE);

    wwd_wifi_set_preferred_association_band(WLC_BAND_2G );

//    wwd_wifi_set_bandwidth(40);
//
//    wwd_wifi_set_roam_trigger(2);

    wwd_wifi_set_tx_power(31);

//    wwd_wifi_set_mcs_rate (WICED_STA_INTERFACE, 0, WICED_TRUE);

    uart_int();
    Init_gpio();
    init_all_timer();

    main_c=is_config();

//    if(get_gpio_menssage(RESET_WIFI)==1){
//        set_gpio_menssage(RESET_WIFI,0);
////        silent=get_gpio_menssage(SOUND_ONOFF);
////        set_gpio_menssage(SOUND_ONOFF,0);
//            silent=WICED_TRUE;
//    }

    silent=get_gpio_menssage(SOUND_ONOFF);



    wiced_rtos_create_thread(&displayThreadHandle, THREAD_BASE_PRIORITY+4, NULL, displayThread, THREAD_STACK_SIZE, NULL);

//    blink_vehicle();
//    wiced_gpio_output_high(Sat_WiFi);
//    buzz(150,0);
//    buzz(85,0);



    switch (main_c) {
           case 0:
               set_name();
               net_config();
               break;
           case 3:
               init_tcp_w();
                net_vehicle();
               break;
           case 51:

               break;
           default:
               break;
       }

    wiced_rtos_set_semaphore(&displaySemaphore);



    }


/*************** UART Command Interface Thread ***************/
/* Tread to handle UART command input/output */
void commandThread(wiced_thread_arg_t arg)
{

}
