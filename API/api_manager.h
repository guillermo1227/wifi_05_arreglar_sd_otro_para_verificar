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


#ifndef _API_MANAGER_H
#define _API_MANAGER_H

#include <math.h>
#include <malloc.h>
#include "wiced.h"

#define SERVER_HOST         "security-4.smartflow.com.mx"

#define URL_API_GET             "/service/persistent/api/v1/Proximity/Time"
#define URL_API_POST            "/service/persistent/api/v1/Proximity/ProximityEvent/EventDevices"

/* non-secure HTTP is port 80 */
#define SERVER_PORT_http        ( 443 )
#define DNS_TIMEOUT_MS     ( 5000 )
#define CONNECT_TIMEOUT_MS ( 3000 )

static void  event_handler_post( http_client_t* client, http_event_t event, http_response_t* response );
static void  event_handler( http_client_t* client, http_event_t event, http_response_t* response );
static void  print_data   ( char* data, uint32_t length );

static wiced_semaphore_t httpWait_post;
static wiced_semaphore_t httpWait_get;

static http_client_t  client_post;
static http_client_t  client_get;

static http_request_t request_get;
static http_request_t request_post;

static wiced_bool_t connected_g = WICED_FALSE;
static wiced_bool_t connected_p = WICED_FALSE;

/******************************************************
 *               Function Definitions
 ******************************************************/
char aux_date_y[13];
char split_date_api[40];
char aux_time[8];



void  split_date_endpoint(unsigned char* buffer_in,int end_length){
    char _txt_date_aux[end_length+1];


//    _txt_date_aux = malloc(sizeof(char)*end_length);
    strncpy(_txt_date_aux,&buffer_in[2],end_length);

    char delim[2] = "\"";
    int x=0;
//    unsigned char *token = strtok(buffer_in, delim);
//    while(token != NULL){
////        printf("\t>>>>>>>%s\n",token);
//
//        switch (x) {
//          case 3:
////              printf("\t>>>>>>>>>>>>>>>>>%s\n",token);
//              memcpy(split_date_api,token,strlen(token));
//
//          break;
//        }
//        x++;
//        token=strtok(NULL, delim);
//    }
//    printf("%s\n",split_date_api);
//    x=0;

    char * token1 = strtok(_txt_date_aux, "-");
    while(token1 != NULL){
//        printf("\t>>>>>>>%s\n",token1);

        switch (x) {
          case 0:
            printf("\t>>>>>>>>>>>>>>>>>%s\n",token1);
            sprintf(aux_date_y,"%s",token1);
            sprintf(aux_date_y,"%c%c/%c%c/%c%c",aux_date_y[0],aux_date_y[1],aux_date_y[3],aux_date_y[4],aux_date_y[8],aux_date_y[9]);

          break;
          case 1:
            printf("\t>>>>>>>>>>>>>>>>>%s\n",token1);
            sprintf(aux_time,"%s",token1);

          break;
        }
        x++;
        token1=strtok(NULL, "-");
    }

//    printf("%s\n",aux_date_y);


//    return _txt_date_aux;
}


/* Helper function to print data in the response to the UART */
static void print_data_body( char* data, uint32_t length )
{
    uint32_t a;

    for ( a = 0; a < length; a++ )
    {
//        WPRINT_APP_INFO( ( "%c", data[a] ) );
        printf("%c",data[a]);

    }
    printf("\n");

}


/* This is the callback event for HTTP events */
static void event_handler( http_client_t* client, http_event_t event, http_response_t* response )
{
    static uint8_t count = 1; // Keep track of how many responses we have received

    switch( event )
    {
        //case HTTP_CONNECTED:
            /* This state is never called */
            break;

        /* This is called when we are disconnected by the server */
        case HTTP_DISCONNECTED:
        {
            connected_g = WICED_FALSE;
            http_client_disconnect( client ); /* Need to keep client connection state synchronized with the server */
            WPRINT_APP_INFO(( "Disconnected from %s\n", SERVER_HOST ));
            wiced_rtos_set_semaphore(&httpWait_get); // Set semaphore that allows the next request to start

            break;
        }

        /* This is called when new data is received (header, or payload) */
        case HTTP_DATA_RECEIVED:
        {


            /* Print Response Payload  */
//            WPRINT_APP_INFO( ("\n----- Response Payload: -----\n" ) );
//            print_data_body( (char*) response->payload, response->payload_data_length );

            if((strstr(response->payload,"DateTimeFormatted"))&&(response->payload_data_length!=NULL)){
//                printf("Hay informacion");
                split_date_endpoint((unsigned char*)response->payload,response->payload_data_length);
//                printf(">>%s<<",split_date_endpoint((unsigned char*)response->payload,response->payload_data_length-1));
            }
            http_request_deinit( (http_request_t*) &(response->request) );

            wiced_rtos_set_semaphore(&httpWait_get); // Set semaphore that allows the next request to start

//            http_client_disconnect( &client );
//            http_client_deinit( &client );
            break;
        }
        default:
        break;
    }
}

/* This is the callback event for HTTP events */
static void event_handler_post( http_client_t* client, http_event_t event, http_response_t* response )
{
    static uint8_t count = 1; // Keep track of how many responses we have received

    switch( event )
    {
//        case HTTP_CONNECTED:
//            /* This state is never called */
//            break;

        /* This is called when we are disconnected by the server */
        case HTTP_DISCONNECTED:
        {
            connected_p = WICED_FALSE;
            http_client_disconnect( client );
            WPRINT_APP_INFO(( "Disconnected from %s\n", SERVER_HOST ));
            wiced_rtos_set_semaphore(&httpWait_post);
            break;
        }

        /* This is called when new data is received (header, or payload) */
        case HTTP_DATA_RECEIVED:
        {
//            WPRINT_APP_INFO( ( "------------------ Received response: %d ------------------\n", count ) );

//            /* Print Response Header */
//            if(response->response_hdr != NULL)
//            {
//                WPRINT_APP_INFO( ( "----- Response Header: -----\n " ) );
//                print_data_body( (char*) response->response_hdr, response->response_hdr_length );
//            }
//
//            /* Print Response Payload  */
//            WPRINT_APP_INFO( ("\n----- Response Payload: -----\n" ) );
            print_data_body( (char*) response->payload, response->payload_data_length );

            if(response->remaining_length == 0)
            {
//               WPRINT_APP_INFO( ("\n------------------ End Response %d ------------------\n", count ) );
               http_request_deinit( (http_request_t*) &(response->request) );
            }

            count++;

            wiced_rtos_set_semaphore(&httpWait_post);
            break;
        }
        default:
        break;
    }
}


void init_http_get(http_header_field_t * header){


    /* Header 0 is the Host header */
     header[0].field        = HTTP_HEADER_HOST;
     header[0].field_length = strlen( HTTP_HEADER_HOST );
     header[0].value        = SERVER_HOST;
     header[0].value_length = strlen( SERVER_HOST );
//     http_client_init( &client_get, WICED_STA_INTERFACE, event_handler, NULL );
//     client_get.peer_cn = (uint8_t*) SERVER_HOST;

}




void init_http_post(http_header_field_t * header){
/* We need three headers - host, content type, and content length */
       /* Header 0 is the Host header */
       header[0].field        = HTTP_HEADER_HOST;
       header[0].field_length = strlen( HTTP_HEADER_HOST );
       header[0].value        = SERVER_HOST;
       header[0].value_length = strlen( SERVER_HOST );
       /* Header 1 is the content type (JSON) */
       header[1].field        =  HTTP_HEADER_CONTENT_TYPE;
       header[1].field_length = strlen( HTTP_HEADER_CONTENT_TYPE );
       header[1].value        = "application/json";
       header[1].value_length = strlen( "application/json" );

//       http_client_init( &client_post, WICED_STA_INTERFACE, event_handler_post, NULL );
//       client_post.peer_cn = (uint8_t*) SERVER_HOST;

}

void init_endpoint(){

        init_http_post(&header_post);
        init_http_get(&header_get);

//        wiced_hostname_lookup( SERVER_HOST, &ip_address, DNS_TIMEOUT_MS, WICED_STA_INTERFACE );

        http_client_init( &client_post, WICED_STA_INTERFACE, event_handler_post, NULL );
        client_get.peer_cn = (uint8_t*) SERVER_HOST;

        http_client_init( &client_get, WICED_STA_INTERFACE, event_handler, NULL );
        client_post.peer_cn = (uint8_t*) SERVER_HOST;


        wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, s1);

//        if(!connected_p)
//        {
//        /* Connect to the server */
//          if ( ( result = http_client_connect( &client_post, (const wiced_ip_address_t*)&ip_address, SERVER_PORT, HTTP_USE_TLS, CONNECT_TIMEOUT_MS ) ) == WICED_SUCCESS )
//          {
//              connected_p = WICED_TRUE;
//              WPRINT_APP_INFO( ( "Connected to %s\n", SERVER_HOST ) );
//          }
//          else
//          {
//              WPRINT_APP_INFO( ( "Error: failed to connect to post server: %u\n", result) );
//              wiced_rtos_set_semaphore(&httpWait_post); // Set semaphore that allows the next request to start
////              return; /* Connection failed - exit program */
//
//          }
//        }
//        if(!connected_g)
//        {
//          if ( ( result = http_client_connect( &client_get, (const wiced_ip_address_t*)&ip_address, SERVER_PORT, HTTP_USE_TLS, CONNECT_TIMEOUT_MS ) ) == WICED_SUCCESS )
//          {
//              connected_g = WICED_TRUE;
//              WPRINT_APP_INFO( ( "Connected to %s\n", SERVER_HOST ) );
//          }
//          else
//          {
//              WPRINT_APP_INFO( ( "Error: failed to connect to get server: %u\n", result) );
//              wiced_rtos_set_semaphore(&httpWait_get); // Set semaphore that allows the next request to start
////              return; /* Connection failed - exit program */
//          }
//
//        }
}


#endif  /* stdbool.h */



