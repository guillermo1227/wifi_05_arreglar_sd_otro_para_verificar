#ifndef _LASEC_API_H
#define __LASEC_API_H


void  send_get(){

    wiced_rtos_delay_milliseconds(10);

    /* Check to see if the server has disconnected. If so, we need to re-connect */
    /* Usually the server won't disconnect between requests since the requests happen quickly. */

    /* Send a GET to resource /html */
       http_request_init( &request_get, &client_get, HTTP_GET, URL_API_GET, HTTP_1_1 );
       http_request_write_header( &request_get, &header_get[0], 1 ); // Header 1 is Host
       http_request_write_end_header( &request_get );
       http_request_flush( &request_get );

}


static char json_len[5]; /* This holds the length of the JSON message as a sting containing the decimal value */

void send_post(http_header_field_t * header,unsigned char * JSON_MSG){

//    for(int d=0;d<=3;d++){
        wiced_rtos_delay_microseconds(1000);
       /* Header 2 is the content length. In this case, it is the length of the JSON message */
       sprintf(json_len,"%d", strlen(JSON_MSG)); /* Calculate the length of the JSON message and store the value as a string */
       header[2].field        = HTTP_HEADER_CONTENT_LENGTH;
       header[2].field_length = strlen( HTTP_HEADER_CONTENT_LENGTH );
       header[2].value        = json_len; // This holds the length of the JSON message as a sting containing the decimal value
       header[2].value_length = strlen( json_len ); // This is the length of the string that holds the JSON message size. For example, if the JSON is 12 characters, this would be "2" because the string "12" is 2 characters long.
//       if(!connected)
//       {
//        /* Connect to the server */
//           if ( ( result = http_client_connect( &client_post, (const wiced_ip_address_t*)&ip_address, SERVER_PORT, HTTP_USE_TLS, CONNECT_TIMEOUT_MS ) ) == WICED_SUCCESS )
//           {
//               connected = WICED_TRUE;
//               WPRINT_APP_INFO( ( "Connected to %s\n", SERVER_HOST ) );
//           }
//           else
//           {
//               WPRINT_APP_INFO( ( "Error: failed to connect to server: %u\n", result) );
//               wiced_rtos_set_semaphore(&httpWait); // Set semaphore that allows the next request to start
//               return; /* Connection failed - exit program */
//
//           }
//       }
       /* Send a POST to resource /anything */
       http_request_init( &request_post, &client_post, HTTP_POST, URL_API_POST, HTTP_1_1 );
       http_request_write_header( &request_post, &header[0], 3 ); // We need 3 headers
       http_request_write_end_header( &request_post );
       http_request_write( &request_post, (uint8_t* ) JSON_MSG, strlen(JSON_MSG)); /* Write the content */
       http_request_flush( &request_post );

//       printf("%d",d);
//    }
//    wiced_rtos_set_semaphore(&httpWait); // Set semaphore that allows the next request to start

}

#endif
