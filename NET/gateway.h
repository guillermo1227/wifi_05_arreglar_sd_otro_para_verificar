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

#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "wiced.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"


//#include "BIOMETRICS/biometric.h"

#include "manager_net.h"
#include "TXT/manager_menssage_vh.h"
#include "UART/main_uart.h"
#include "GPIO/manager_gpio_vh.h"


int status_config=0;


/******************************************************
 *                     TCP Client
 ******************************************************/
//This is needed to implement tcp services
#define TCP_CLIENT_INTERVAL               1
#define STREAM_DATA_BUFFER_LENGTH         1000
#define limit_data                        100

#define TCP_PACKET_MAX_DATA_LENGTH        60


#define TCP_SERVER_PORT                   8010
#define TCP_CLIENT_CONNECT_TIMEOUT        500
#define TCP_CLIENT_RECEIVE_TIMEOUT        200
#define TCP_CLIENT_RECEIVE_TIMEOUT_LAMP        3000

#define TCP_CONNECTION_NUMBER_OF_RETRIES  6
#define TCP_DOWN_NUMBER                   10
#define TCP_CLIENT_STACK_SIZE   (10000)

#define SIID_C "-SSF-HARDWARE"
#define PASS_C "-KLasec123."

uint8_t mac_wifi_Send[40];

struct net_Save net_value;

wiced_tcp_stream_t* tcp_stream_wr;
static wiced_tcp_socket_t  tcp_client_socket;

//Flag to start tcp service
#define TCP_SERVER_IP_ADDRESS MAKE_IPV4_ADDRESS(10,86,10,1)

void select_net(uint8_t* msn1,uint8_t thread1);
void switch_msm(int status_c);
wiced_interface_t interface;

static const wiced_ip_setting_t device_init_settings =
{
    INITIALISER_IPV4_ADDRESS( .ip_address, MAKE_IPV4_ADDRESS(10,86,  11,  1) ),
    INITIALISER_IPV4_ADDRESS( .netmask,    MAKE_IPV4_ADDRESS(255,255,255,  0) ),
    INITIALISER_IPV4_ADDRESS( .gateway,    MAKE_IPV4_ADDRESS(10,86,  10,  254) ),
};

char mac_wifi[40];
char mac_ap[40];
char ip[30];
char mac_wifi_bt[12];
char mac_find[24];
int s_count_x=0;
uint8_t try_n=0;
char data_out[1000];
char bt_msm[50];
int data_send_bt;


// Variables for network change
uint32_t rs[4];
uint32_t rn[4];
uint32_t ri[4];
uint32_t rg[4];

uint32_t d1;
uint32_t d2;
uint32_t d3;
uint32_t d4;

unsigned long s1;

void publishThread(wiced_thread_arg_t arg);

void check_sound_onoff(){
    if(silent==WICED_TRUE){
        if(_sound_flag==WICED_TRUE){
            _sound_flag=WICED_FALSE;
//            set_gpio_menssage(SOUND_ONOFF,silent);
            set_gpio_menssage(RESET_WIFI,1);
        }
    }


}

/*************** Tcp Configurator Thread ***************/
/* Thread to publish data to the cloud */
void publishThread(wiced_thread_arg_t arg)
{
    wiced_mac_t myMac;
    wiced_tcp_socket_t socket;                      // The TCP socket
    wiced_tcp_stream_t stream;                      // The TCP stream
    char sendMessage[80];
    wiced_result_t result;
    wiced_packet_t* tx_packet;
    uint8_t *tx_data;
    uint16_t available_data_length;


/* Create a TCP socket */
        if ( wiced_tcp_create_socket( &tcp_client_socket, interface ) != WICED_SUCCESS )
        {
            WPRINT_APP_INFO( ("TCP socket creation failed\n") );
        }
        else
            WPRINT_APP_INFO( ("TCP socket creation \n") );


        /* Bind to the socket */
        wiced_tcp_bind( &tcp_client_socket, TCP_SERVER_PORT );

        /*Initialize tcp_stream*/
        tcp_stream_wr = (wiced_tcp_stream_t*)malloc(sizeof(wiced_tcp_stream_t));
        if (wiced_tcp_stream_init(tcp_stream_wr, &tcp_client_socket)!= WICED_SUCCESS)
            WPRINT_APP_INFO(("TCP stream init failed \n"));
        else
            WPRINT_APP_INFO(("TCP stream init \n"));


//        wiced_rtos_set_semaphore(&tcpGatewaySemaphore);

    while ( 1 )
    {
//        wiced_rtos_get_semaphore(&tcpGatewaySemaphore,WICED_WAIT_FOREVER);
//        wiced_gpio_output_low(Sat_WiFi);

//        wiced_rtos_set_semaphore(&displaySemaphore);
        wiced_rtos_lock_mutex(&pubSubMutex);

        wiced_ip_address_t INITIALISER_IPV4_ADDRESS( server_ip_address, s1 );


        WPRINT_APP_INFO(("Event Thread Tcp client\n"));
//        WPRINT_APP_INFO( ("Count of bluetooth with mac sssssssss -> %d \r\n",s_count_x) );

        wiced_tcp_socket_t socket;                      // The TCP socket
        wiced_tcp_stream_t stream;                      // The TCP stream
        wiced_result_t result;

        // Open the connection to the remote server via a Socket
        result = wiced_tcp_create_socket(&socket, WICED_STA_INTERFACE);

        if ( result != WICED_SUCCESS )
        {
            try_n=try_n+1;
            if(try_n==TCP_DOWN_NUMBER){
                wiced_gpio_output_low(Sat_WiFi);
//                   set_name();
                check_sound_onoff();
                 wiced_rtos_delay_milliseconds(100);
//                     set_name();
               wiced_framework_reboot();
            }

            WPRINT_APP_INFO(("falied 1\n"));

        }
        else{
            try_n=0;
            WPRINT_APP_INFO(("e  falied 1\n"));

        }

        result = wiced_tcp_bind( &socket, WICED_ANY_PORT ); /* Poner any port para que actualice el uerto de manera automatica */
        if(result!=WICED_SUCCESS)
        {
            wiced_gpio_output_low(Sat_WiFi);
            try_n=try_n+1;
            if(try_n==TCP_DOWN_NUMBER){
//                   set_name();
                check_sound_onoff();

                 wiced_rtos_delay_milliseconds(100);
//                     set_name();
               wiced_framework_reboot();
            }

            WPRINT_APP_INFO(("falied 2\n"));

            wiced_tcp_delete_socket(&socket); /* Delete socket and return*/
        }
        else{
            try_n=0;
            WPRINT_APP_INFO((" e falied 2\n"));

        }

        result = wiced_tcp_connect(&socket,&server_ip_address,TCP_SERVER_PORT,2500); // 2 second timeout
        if ( result != WICED_SUCCESS )
        {
            try_n=try_n+1;
            if(try_n==TCP_DOWN_NUMBER){
                wiced_gpio_output_low(Sat_WiFi);
////                   set_name();
                check_sound_onoff();
                     wiced_rtos_delay_milliseconds(100);
//                     set_name();
               wiced_framework_reboot();
            }

            WPRINT_APP_INFO(("falied 3\n"));

            wiced_tcp_delete_socket(&socket);

        }
        else{
            try_n=0;
            WPRINT_APP_INFO(("  e  falied 3\n"));

        }

        WPRINT_APP_INFO(("try %d\n",try_n));


        wiced_ip_address_t myIpAddress;

        wl_bss_info_t ap_info_buffer;
        wiced_security_t ap_security;
        // Format the data per the specification in section 6
        wwd_wifi_get_ap_info(&ap_info_buffer, &ap_security);            // Se obtiene la MAC de la red a la que estamos conectados
        wiced_ip_get_ipv4_address( WICED_STA_INTERFACE, &myIpAddress);  // Se obtiene la IP del dispositivo
        wiced_wifi_get_mac_address(&myMac);                             // Se obtiene la MAC del dispositivo



        sprintf(mac_wifi,"%02X:%02X:%02X:%02X:%02X:%02X",myMac.octet[0],myMac.octet[1],myMac.octet[2],myMac.octet[3],myMac.octet[4],myMac.octet[5]);

        sprintf(mac_ap,"%02X:%02X:%02X:%02X:%02X:%02X",ap_info_buffer.BSSID.octet[0], ap_info_buffer.BSSID.octet[1],ap_info_buffer.BSSID.octet[2],ap_info_buffer.BSSID.octet[3],ap_info_buffer.BSSID.octet[4],ap_info_buffer.BSSID.octet[5]);
        sprintf(ip,"%u.%u.%u.%u", (uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 24),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress)>> 16),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 8),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 0));

        // Initialize the TCP stream
        wiced_tcp_stream_init(&stream, &socket);

        // Send the data via the stream
//        if((s_count_x<=limit_data)){
//            WPRINT_APP_INFO(("Multiple Tcp client\n"));
//            if(s_count_x==limit_data){
//                data_send_bt=limit_data;
////                count_bt=0;
//            }
//            else if(s_count_x<limit_data){
//                data_send_bt=s_count_x;
////                count_bt=0;
//            }
//            if(s_count_x!=0){
//
////                blink_vehicle();
//
////                wiced_gpio_output_high(Sat_WiFi);
////                wiced_gpio_output_high(Sat_WiFi);
//
//                WPRINT_APP_INFO( (">> es igual a %d en %s\n\n\n",s_count_x,mac_ap) );
//
//                for(int f=0;f<data_send_bt;f++){
//                    if(f==0){
////                    sprintf(data_out,"\nV;%s,%s,%s\r\n",mac_wifi,mac_ap,ip);
//                        sprintf(data_out,"\nU;%s,1600,%d%d%d%d0000000000000,%s,%s\r\n",mac_wifi,t1,t2,t3,t4,mac_ap,ip);
//                        result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
//                        if(result==WICED_TCPIP_SUCCESS){
//                            wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
//                            send_data_task=WICED_TRUE;
//                        }
//                    }
//                    else{
//
//
//                    wiced_rtos_delay_milliseconds( 1 );
////                    sprintf(data_out,"\nB;%s,%s,%s,%s,%s,%s\r\n",mac_ap,data_btt[f].mac_bt,mac_wifi,data_btt[f].type,data_btt[f].rssi,data_btt[f].fallen);
//                    sprintf(data_out,"\nB;%s,%s,%s,%s,%s\r\n",mac_ap,data_btt[f].mac_bt,mac_wifi,data_btt[f].rssi,data_btt[f].fallen);
//
//                                    memcpy(data_btt[f].mac_bt,NULL,17);
////                                    memcpy(data_btt[f].type,NULL,17);
//                                    memcpy(data_btt[f].rssi,NULL,5);
//                                    memcpy(data_btt[f].fallen,NULL,2);
//
//
//
//                    result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
//                    if(result==WICED_TCPIP_SUCCESS){
//
//                        wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
//                        send_data_task=WICED_TRUE;
//
//                     }
//                    }
//                }
//                s_count_x=0;
//                data_send_bt=0;
//            }
//            else{
////                wiced_gpio_output_high(Sat_WiFi);
////                wiced_gpio_output_high(Sat_WiFi);
////                blink_vehicle();
//
////                sprintf(data_out,"\nV;%s,%s,%s\r\n",mac_wifi,mac_ap,ip);
//                sprintf(data_out,"\nU;%s,1600,%d%d%d%d0000000000000,%s,%s\r\n",mac_wifi,t1,t2,t3,t4,mac_ap,ip);
//                result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
//                   if(result==WICED_TCPIP_SUCCESS){
//                       wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
//                       send_data_task=WICED_TRUE;
//
//                    }
//
//            }
//
//        }

//        wiced_gpio_output_low(Sat_WiFi);
//        printf("gateway: %s\n",define_variable( &data_adq, 2));
//        printf("gateway: %d\n",data_adq.h_rate);

        memcpy(data_adq.mac,mac_wifi,strlen(mac_wifi));

        sprintf(data_out,"\n%s\r\n\n%s\r\n",define_variable( &data_adq, 1),define_variable( &data_adq, 2));
        result=wiced_tcp_stream_write(&stream, data_out, strlen(data_out));
           if(result==WICED_TCPIP_SUCCESS){
               wiced_uart_transmit_bytes(WICED_UART_1,(("%s",data_out)),strlen(data_out));
               send_data_task=WICED_TRUE;

            }

        wiced_tcp_stream_flush(&stream);


        // Delete the stream and socket
        wiced_tcp_stream_deinit(&stream);
        wiced_tcp_delete_socket(&socket);
        wiced_rtos_delay_milliseconds( 1050 );


    wiced_rtos_unlock_mutex(&pubSubMutex);

//    wiced_rtos_set_semaphore(&tcpReceptionSemaphore);

    }
}


//static wiced_result_t tcp_client_lamp( void )
//{
//    WPRINT_APP_INFO(("Event Thread Tcp client\n"));
//        char* resultado;
//        wiced_result_t result;
//        wiced_mac_t myMac;
//        wiced_ip_address_t myIpAddress;
//        wl_bss_info_t ap_info_buffer;
//        wiced_security_t ap_security;
//
//        wiced_packet_t*          packet;
//        wiced_packet_t*          rx_packet;
//        unsigned char*                    tx_data;
//        unsigned char*                    rx_data;
//        uint16_t                 rx_data_length;
//        uint16_t                 available_data_length;
//        int                      connection_retries;
//        wiced_ip_address_t INITIALISER_IPV4_ADDRESS( server_ip_address, s1 );
//
//
//        wiced_rtos_set_semaphore(&tcpReceptionSemaphore);
//
//
//        while(1){
//            wiced_rtos_get_semaphore(&tcpReceptionSemaphore,WICED_WAIT_FOREVER);
//
//
//            /* Connect to the remote TCP server, try several times */
//                        connection_retries = 0;
//                        do
//                        {
//                            result = wiced_tcp_connect( &tcp_client_socket, &server_ip_address, TCP_SERVER_PORT, TCP_CLIENT_CONNECT_TIMEOUT );
//                            connection_retries=connection_retries+1;
//                        }
//                        while( ( result != WICED_SUCCESS ) && ( connection_retries < TCP_CONNECTION_NUMBER_OF_RETRIES ) );
//                        if ( result != WICED_SUCCESS )
//                           {
//                           WPRINT_APP_INFO(("Unable to connect to the server! Halt.\n"));
//                           try_n=try_n+1;
//                           if(try_n==TCP_DOWN_NUMBER){
//               //                check_satat_gpio();
//
//                                   wiced_framework_reboot();
//                           }
//                           wiced_uart_transmit_bytes(WICED_UART_1,TCP_CONNECT_FAILED,sizeof(TCP_CONNECT_FAILED)-1);
//
//                        }
//                        else{
//                            try_n=0;
//                        }
//                        /* Create the TCP packet. Memory for the tx_data is automatically allocated */
//                        if (wiced_packet_create_tcp(&tcp_client_socket, TCP_PACKET_MAX_DATA_LENGTH, &packet, (uint8_t**)&tx_data, &available_data_length) != WICED_SUCCESS)
//                        {
//                            WPRINT_APP_INFO(("TCP packet creation failed\n"));
//                            try_n=try_n+1;
//                            if(try_n==TCP_DOWN_NUMBER){
//               //                 check_satat_gpio();
//
//                                wiced_framework_reboot();
//                            }
//                           wiced_uart_transmit_bytes(WICED_UART_1,CREATE_FAILED,sizeof(CREATE_FAILED)-1);
//
//                        }
//                        else{
//                            try_n=0;
//                        }
//
//                        /* Write the message into sendMessage"  */
//
//                       // Format the data per the specification in section 6
//                       wiced_wifi_get_mac_address(&myMac);                             // Se obtiene la MAC del dispositivo
//                       wiced_ip_get_ipv4_address( WICED_STA_INTERFACE, &myIpAddress);  // Se obtiene la IP del dispositivo
//                       wwd_wifi_get_ap_info(&ap_info_buffer, &ap_security);            // Se obtiene la MAC de la red a la que estamos conectados
//                       /*Ectract data to peripherals*/
//
//
//
//                       sprintf(mac_wifi,"%02X:%02X:%02X:%02X:%02X:%02X",myMac.octet[0],myMac.octet[1],myMac.octet[2],myMac.octet[3],myMac.octet[4],myMac.octet[5]);
//                       sprintf(mac_ap,"%02X:%02X:%02X:%02X:%02X:%02X",ap_info_buffer.BSSID.octet[0], ap_info_buffer.BSSID.octet[1],ap_info_buffer.BSSID.octet[2],ap_info_buffer.BSSID.octet[3],ap_info_buffer.BSSID.octet[4],ap_info_buffer.BSSID.octet[5]);
//                       sprintf(ip,"%u.%u.%u.%u", (uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 24),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress)>> 16),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 8),(uint8_t)(GET_IPV4_ADDRESS(myIpAddress) >> 0));
//
//           //            sprintf(tx_data,"\nL;%s,%d,%17s,%s,%s\r\n",mac_wifi,ADC_VALUE,mcu,mac_ap,ip);
//                                   sprintf(tx_data,"\nL;8C:45:00:D6:39:EC,15963,00000000000000000,62:19:BE:2F:0C:EB,10.86.11.227\r\n");
//
//                       /* Set the end of the data portion */
//                        wiced_packet_set_data_end(packet, (uint8_t*)tx_data + strlen(tx_data));
//                        /* Send the TCP packet */
//                                 if (wiced_tcp_send_packet(&tcp_client_socket, packet) != WICED_SUCCESS)
//                                 {
//                                     /* Delete packet, since the send failed */
//                                     wiced_packet_delete(packet);
//
//                                     /* Close the connection */
//                                     wiced_tcp_disconnect(&tcp_client_socket);
//                                 }
//                                 else{
//                                    wiced_uart_transmit_bytes(WICED_UART_1,(("%s\r\n",tx_data)),strlen(tx_data)); // echo the message so that the user can see something
//
//           //                         Anticolision_pulse();
//
//                                 }
//
//                                 int msm=3;
//                                 do{
//                                 result = wiced_tcp_receive(&tcp_client_socket, &rx_packet, TCP_CLIENT_RECEIVE_TIMEOUT_LAMP);
//                                 if( result != WICED_SUCCESS )
//                                {
//                                   /* Delete packet, since the receive failed */
//                                   wiced_packet_delete(rx_packet);
//
//                                   /* Close the connection */
//                                   wiced_tcp_disconnect(&tcp_client_socket);
//                                }
//                                else{
//                                    WPRINT_APP_INFO((" packets receive\n"));
//
//                                }
//                                /* Get the contents of the received packet */
//                                wiced_packet_get_data(rx_packet, 0, (uint8_t**)&rx_data, &rx_data_length, &available_data_length);
//
//                                if (rx_data_length != available_data_length)
//                                {
//                                       WPRINT_APP_INFO(("Fragmented packets not supported\n"));
//
//                                    /* Delete packet, since the receive failed */
//                                   wiced_packet_delete(rx_packet);
//
//                                    /* Close the connection */
//                                   wiced_tcp_disconnect(&tcp_client_socket);
//                                }
//                                /* Null terminate the received string */
//                               rx_data[rx_data_length] = '\x0';
//                               WPRINT_APP_INFO(("%s\r\n",rx_data));
//
//                               if(strstr(rx_data,"A1")){
//                                   Evacaution=WICED_TRUE;
//                                   WPRINT_APP_INFO((" Evacaucion de vehiculo %d\n",Evacaution));
//                                   wiced_rtos_set_semaphore(&displaySemaphore);
//                               }
//
//
//                                  msm=msm-1;
//                                  wiced_packet_delete(rx_packet);
//
//                                 }  while(msm!=0);
//                        wiced_packet_delete(packet);
//                        wiced_packet_delete(rx_packet);
//                        wiced_tcp_disconnect(&tcp_client_socket);
//
//                        wiced_rtos_set_semaphore(&tcpGatewaySemaphore);
//
//        }
//
//
//
//}


void data_bt_send(unsigned char* buffer_in ){
    unsigned char str_switch[4];
    unsigned char str_split[128];
    unsigned char pem_mac[17];
    unsigned char str_temp[17];


    strncpy(str_switch,buffer_in,4);
    strcpy(str_split,&buffer_in[4]);

    char delim[] = ",";     //establece como  realizara el split
    int x=0;
    if(strstr(str_switch,"BNM:")){

        if(s_count_x==limit_data){
            data_send_bt=s_count_x;
        }
        else if(s_count_x<limit_data){

            unsigned char *cvl1 = strtok(str_split, delim);
            while(cvl1 != NULL){
//                WPRINT_APP_INFO( ("strtok -> %s \r\n",cvl1) );
//                strcpy(pem_mac,cvl1);
//                WPRINT_APP_INFO( ("memecpy -> %s \r\n",pem_mac) );
                switch (x) {
                    case 0:
                        memcpy(data_btt[s_count_x+1].mac_bt,cvl1,17);
                    break;
                    case 1:
//                        strcpy(data_btt[s_count_x+1].type,cvl1);
                        break;
                    case 2:
                        strcpy(data_btt[s_count_x+1].rssi,cvl1);
                        break;
                    case 3:
                        strcpy(data_btt[s_count_x+1].fallen,cvl1);
                        break;
                    default:
                        break;
                }
                x++;
                cvl1=strtok(NULL, delim);
            }
            x=0;
            s_count_x++;
            data_send_bt=s_count_x;
        }

//        unsigned char *cvl1 = strtok(str_split, delim);
//        while(cvl1 != NULL){
//            switch (x) {
//                case 0:
//                break;
//                case 1:
//                    strcpy(str_temp,cvl1);
//                    if((strcmp(str_temp,"BEAC")==0)){
////                        WPRINT_APP_INFO( ("name:=> %s\r\n",str_temp) );
//                        risk_z=WICED_TRUE;
//                    }
//                    break;
//                case 2:
//
//                    break;
//                default:
//                    break;
//            }
//            x++;
//            cvl1=strtok(NULL, delim);
//        }


    }

    wiced_rtos_set_semaphore(&displaySemaphore);

}




#endif  /* stdbool.h */
