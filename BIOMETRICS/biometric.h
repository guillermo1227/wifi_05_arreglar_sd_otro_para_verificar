#ifndef _BIOMETRIC_H
#define _BIOMETRIC_H


#include <stdio.h>
#include "wiced.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#define HEATLH_THERMOMETER  1
#define HEART_RATE          2
#define POSITION_FILTER_DEV 4
#define FILTER_BIO          "BIO#"
#define FILTER_HT           "HT"
#define FILTER_HR           "HR"


 struct data_status
{
    unsigned char mac[18];
    float   h_temperature;
    int     h_rate;
};

struct  data_status data_adq;


/* Define Functions */
void save_data( char* data, struct  data_status* data_struct ,int type_data );
int identify_device( char *data );
char* define_variable( struct  data_status* data_struct, int type_data );


/* Identify the device type ( Health Thermometer / Heart Rate ) */
int identify_device( char *data )
{
    // Filter to search only in the string that starts with "BIO#"
    if( memcmp( &data[0], FILTER_BIO, sizeof(FILTER_BIO)-1 ) == 0 )
    {
        // Identify the device type
        if( !memcmp( &data[POSITION_FILTER_DEV], FILTER_HT, sizeof(FILTER_HT)-1 ) )
        {
            printf("Data from Health Thermometer\n");
            return HEATLH_THERMOMETER;  // Found data from Health Thermometer
        }
        else if( !memcmp( &data[POSITION_FILTER_DEV], FILTER_HR, sizeof(FILTER_HR)-1 ) )
        {
            printf("Data from Heart Rate\n");
            return HEART_RATE;          // Found data from Hear Rate
        }
        else
        {
            return 0;                   // Found data from Unknown Device
        }
    }
    else
    {
        return 0;                       // Data not found
    }
}


/* Function to save data from the devices */
void save_data( char* data, struct  data_status* data_struct, int type_data )
{
//    printf("%s",&data);
    if( memcmp( &data[0], FILTER_BIO, sizeof(FILTER_BIO)-1 ) == 0 )
        {
        switch(type_data)
        {
            case HEATLH_THERMOMETER:
                data_struct->h_temperature = atof(&data[7]);
            break;

            case HEART_RATE:

//                data_struct->h_rate =  atoi(&data[7]);
                data_struct->h_rate = 8;

            break;

            default:
            break;
        }
        }
}


char* define_variable(  struct  data_status* data_struct, int type_data )
{

    char* result;
    int length;

    switch (type_data)
    {
        case HEATLH_THERMOMETER:
            // Define the size for the array
            length = snprintf(NULL, 0, "BM:{\"SensorType\":%d,\"value\":%.2f,\"ID\":\"%s\"}",
                     HEATLH_THERMOMETER, data_struct->h_temperature, data_struct->mac);

            result = (char*)malloc(length+1); // se asigna a result la dirección de un bloque de memoria del tamaño de un char

            // Re-write the array result with the size obtained
            snprintf(result, length+1, "BM:{\"SensorType\":%d,\"value\":%.2f,\"ID\":\"%s\"}",
                     HEATLH_THERMOMETER, data_struct->h_temperature, data_struct->mac);

//            printf("%s\n", result);

            break;

        case HEART_RATE:
//            length = snprintf(NULL, 0, "BM:{\"SensorType\":%d,\"value\":%d,\"ID\":\"%s\"}",
//                     HEART_RATE, data_struct->h_rate, data_struct->mac);

            result = (char*)malloc(60+1); // se asigna a result la dirección de un bloque de memoria del tamaño de un char

            // Re-write the array result with the size obtained
            sprintf(result, "BM:{\"SensorType\":%d,\"value\":%d,\"ID\":\"%s\"}",
                     HEART_RATE, data_struct->h_rate, data_struct->mac);

//            printf("%s\n", result);
            break;

        default:
            result = NULL;
            break;
    }

    return result;
}



#endif  /* stdbool.h */
