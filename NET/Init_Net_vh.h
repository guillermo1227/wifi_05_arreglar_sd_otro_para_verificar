/**********************************************************************************
* Copyright (C) 2018-2021 LASEC®️ Telecomunicaciones S.A.P.I. de C.V.
* All rights reserved.
*
* This document is the property of LASEC®️ Telecomunicaciones S.A.P.I. de C.V.
* It is considered confidential and proprietary.
*
* This document may not be reproduced or transmitted in any form,
* in whole or in part, without the express written permission of
* LASEC®️ Telecomunicaciones S.A.P.I. de C.V.
*
*********************************************************************************/



#ifndef INIT_NET_VH_H
#define INIT_NET_VH_H

#include "wiced.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

//#include "manager_tcp_client_vh.h"
#include "gateway.h"
#include "wireless_config.h"

/* Time in milliseconds for automatic publish of weather info */
#define TIMER_TIME (30000)
#define LASEC_JOIN_RETRY_ATTEMPTS   (4000)


wiced_config_ap_entry_t* ap;
unsigned int             a;
int                      retries;

wiced_bool_t delete_class_socket=WICED_FALSE;
wiced_bool_t netdown=WICED_FALSE;
static wiced_timed_event_t tcp_client_event;


#define LASEC_JOIN_RETRY_ATTEMPTS   (4000)

wiced_ip_setting_t device_init_ip_settings2;

wiced_config_ap_entry_t* ap;
unsigned int             a;
int                      retries;



void net_vehicle(){
    d1= ((rg[0]<< 24) | (rg[1] << 16) | ( rg[2] << 8) | (rg[3]));
    d2= ((rn[0]<< 24) | (rn[1] << 16) | ( rn[2] << 8) | (rn[3]));
    d3= ((ri[0]<< 24) | (ri[1] << 16) | ( ri[2] << 8) | (ri[3]));

    s1 = MAKE_IPV4_ADDRESS(rs[0],rs[1],rs[2],rs[3]);

    device_init_ip_settings2.gateway.ip.v4=(uint32_t)d1;
    device_init_ip_settings2.netmask.ip.v4=(uint32_t)d2;
    device_init_ip_settings2.ip_address.ip.v4=(uint32_t)d3;
    /* Bring up the network interface */
      wiced_result_t result;

      WPRINT_APP_INFO( ("Net config\r\n") );

  //    result = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER,&device_init_settings);

      for ( retries = LASEC_JOIN_RETRY_ATTEMPTS; retries != 0; --retries )
        {
     wiced_dct_read_lock( (void**) &ap, WICED_FALSE, DCT_WIFI_CONFIG_SECTION, (uint32_t) ( OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) ), sizeof(wiced_config_ap_entry_t) ) ;
                  if ( ap->details.SSID.length != 0 )
                  {
                      result=wiced_join_ap_specific( &ap->details, ap->security_key_length, ap->security_key );
                      wiced_dct_read_unlock( (wiced_config_ap_entry_t*) ap, WICED_FALSE );


                  }
          if ( result == WICED_SUCCESS )
          {
             break;
          }

        }

      result = wiced_ip_up( interface, WICED_USE_STATIC_IP, &device_init_ip_settings2 );




      wiced_rtos_create_thread(&publishThreadHandle, THREAD_BASE_PRIORITY+1, NULL, publishThread, THREAD_STACK_SIZE, NULL);
//      wiced_rtos_create_thread(&subscribeThreadHandle, THREAD_BASE_PRIORITY+1, NULL, tcp_client_lamp, THREAD_STACK_SIZE, NULL);


}

void net_config(){

    /* Bring up the network interface */
    wiced_result_t result;


    Set_SSID(SIID_C,sizeof(SIID_C),WICED_UART_1);
    Set_KEY(PASS_C,sizeof(PASS_C),WICED_UART_1);

    WPRINT_APP_INFO( ("Net config\r\n") );

//    result = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER,&device_init_settings);

    for ( retries = LASEC_JOIN_RETRY_ATTEMPTS; retries != 0; --retries )
      {
   wiced_dct_read_lock( (void**) &ap, WICED_FALSE, DCT_WIFI_CONFIG_SECTION, (uint32_t) ( OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) ), sizeof(wiced_config_ap_entry_t) ) ;
                if ( ap->details.SSID.length != 0 )
                {
                    result=wiced_join_ap_specific( &ap->details, ap->security_key_length, ap->security_key );
                    wiced_dct_read_unlock( (wiced_config_ap_entry_t*) ap, WICED_FALSE );


                }
        if ( result == WICED_SUCCESS )
        {
           break;
        }

      }

    result = wiced_ip_up( interface, WICED_USE_EXTERNAL_DHCP_SERVER, &device_init_settings );



    /* Create a TCP socket */
    if ( wiced_tcp_create_socket( &tcp_client_socket, interface ) != WICED_SUCCESS )
    {

    }
    /* Bind to the socket */
    wiced_tcp_bind( &tcp_client_socket, TCP_SERVER_PORT_c );
    /* Register a function to send TCP packets */
    wiced_rtos_register_timed_event( &tcp_client_event, WICED_NETWORKING_WORKER_THREAD, &tcp_client_config, TCP_CLIENT_INTERVAL_c, 0 );
}


void set_name(){
   wiced_mac_t mac;

    if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
    {
        WPRINT_NETWORK_INFO(("WLAN MAC : %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]));
        mac_Re=re_mac(mac);
        sprintf(mac_wifi,"%02X:%02X:%02X:%02X:%02X:%02X", mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]);
        sprintf(mac_wifi_bt,"WMA%c%c%c%c%c%c\n\n", mac_Re.mac_rewrite[0] ,mac_Re.mac_rewrite[1],mac_Re.mac_rewrite[2],mac_Re.mac_rewrite[3],mac_Re.mac_rewrite[4],mac_Re.mac_rewrite[5]);
        sprintf(mac_wifi_Send,"%02X:%02X:%02X:%02X:%02X:%02X\n",mac_Re.mac_rewrite[0],mac_Re.mac_rewrite[1],mac_Re.mac_rewrite[2],mac_Re.mac_rewrite[3],mac_Re.mac_rewrite[4],mac_Re.mac_rewrite[5]);
        sprintf(mac_find,"WMA%c%c%c%c%c%c\n\n",mac_Re.mac_rewrite[0],mac_Re.mac_rewrite[1],mac_Re.mac_rewrite[2],mac_Re.mac_rewrite[3],mac_Re.mac_rewrite[4],mac_Re.mac_rewrite[5]);

    }
    wiced_uart_transmit_bytes( WICED_UART_1, mac_wifi_bt, sizeof(  mac_wifi_bt ) - 1 );
    wiced_uart_transmit_bytes( WICED_UART_1, mac_find, sizeof( mac_find ) - 1 );
//    wiced_uart_transmit_bytes( WICED_UART_1, "WMA123456\n\n", sizeof( mac_find ) - 1 );
//    wiced_uart_transmit_bytes( WICED_UART_1, "WMA123456\n\n", sizeof( mac_find ) - 1 );

    WPRINT_APP_INFO(("Mac bluetooth:  %s  \n",mac_wifi_bt));

}

#endif  /* stdbool.h */
