#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "wiced.h"

#include "RTC_manager.h"


wiced_i2c_device_t i2c_rtc;
uint8_t* current_file;

void application_start( )
{
    wiced_init( );
    init_rtc(&i2c_rtc);
    date_set("09/08/22",&i2c_rtc);
    time_set("09:50:00",&i2c_rtc);

//    current_file = malloc(sizeof(uint8_t)*128);

    while(1)
    {

        WPRINT_APP_INFO( ( "TIEMPO: \t %s\n" ,time_get(&i2c_rtc)) );

        WPRINT_APP_INFO( ( "FECHA: \t %s\n" ,date_get(&i2c_rtc)) );

//        sprintf(current_file,"{\"mac\"[as]}\n");
//        printf("%s",current_file);

            wiced_rtos_delay_milliseconds( 1000 );

    }
}


