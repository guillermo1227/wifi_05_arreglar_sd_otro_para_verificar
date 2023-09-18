/* Connect to httpbin.org and send GET requests to /anything and /html */
#include <stdlib.h>
#include "wiced.h"

//177.241.233.66
#include "http_client.h"
//wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, MAKE_IPV4_ADDRESS(10,90,120,10) );

wiced_ip_address_t  ip_address;
wiced_result_t      result;
/* We need only 1 header to specify the host */
http_header_field_t header_get[1];
http_header_field_t header_post[3];

#include "api_manager.h"
#include "lasec_api.h"



#define JSON_MSG  "[{\"EventDateFormatted\":\"24/02/2022-13:10:10\",\"LogId\":22,\"ProximityEventDevices\":[{\"DeviceId\":\"00:00:00:00:00:00\",\"EventType\":2,\"StatusType\":3},{\"DeviceId\":\"00:00:00:00:00:01\",\"EventType\":2,\"StatusType\":2}]}]"


static wiced_timed_event_t guardian2;



void application_start( void )
{
//    wiced_ip_address_t INITIALISER_IPV4_ADDRESS( ip_address, MAKE_IPV4_ADDRESS(10,174,109,30) );


    wiced_init( );

    printf("%s\n",JSON_MSG);
    wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);



    init_endpoint();


//    wiced_rtos_register_timed_event( &guardian2, WICED_NETWORKING_WORKER_THREAD, &send_post, 1000, 0 );

//    while(1){
//        http_request_init( &request_get, &client_get, HTTP_GET, URL_API_GET, HTTP_1_1 );
//        http_request_init( &request_post, &client_post, HTTP_POST, URL_API_POST, HTTP_1_1 );

            wiced_rtos_delay_milliseconds(1000);
            send_post(&header_post,JSON_MSG);
            wiced_rtos_get_semaphore(&httpWait, WICED_WAIT_FOREVER); /* Wait for this request to complete before going on */
            wiced_rtos_delay_milliseconds(1000);
                   send_post(&header_post,JSON_MSG);
                   wiced_rtos_get_semaphore(&httpWait, WICED_WAIT_FOREVER);
            wiced_rtos_delay_milliseconds(1000);
            send_get();
//            wiced_rtos_delay_milliseconds(1000);
            wiced_rtos_get_semaphore(&httpWait, WICED_WAIT_FOREVER); /* Wait for this request to complete before going on */

//        }
}



