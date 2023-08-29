#include "i2c.c"
#include <math.h>
#include <malloc.h>
#include "wiced.h"
#include "resources.h"
#include "wiced_management.h"
#include "wiced_usb.h"



#define RTC_DEVICE_ADDR        (0x68)
#define RTC_WOAMI_REG         (0x00)//(0x02 | 0x80)
#define SEGREG_X8                (0x00)
#define MINREG_X8                (0x01)
#define HREG_X8                  (0x02)
#define YEARREG_X8                (0x06)
#define MONTHREG_X8               (0x05)
#define DAYREG_X8                 (0x04)





#define NUM_I2C_MESSAGE_RETRIES   (3)
    int ConvertExa(int numberDec);
    int powInt(int x, int y);
    int parseInt(char* chars);





static wiced_i2c_device_t i2c_device_rtc =
{
        .port = WICED_I2C_2,  //I2C_1
        .address = RTC_DEVICE_ADDR,
        .address_width = I2C_ADDRESS_WIDTH_7BIT,
        .speed_mode = I2C_STANDARD_SPEED_MODE,
};


//char *todo

int time_set(char *enter)
{
    //char enter[9]="59:15:32";
    //time_set(enter);
int chourtwodigits;
int cmintwodigits;
int csegtwodigits;
    uint8_t *wbuf;
    uint8_t *rbuf;
    //printf("size %zu \r\n", strlen(enter));
    if(strlen(enter) == 8){
        printf("Numero aceptado: %s \r\n", enter);
        char * token = strtok(enter, ":");
        int hourtwodigits = parseInt(token);
        printf("First %d \r\n", hourtwodigits);
        chourtwodigits=ConvertExa(hourtwodigits);

        token = strtok(NULL, ":");
        int mintwodigits = parseInt(token);
        printf("Second %d \r\n", mintwodigits);
        cmintwodigits = ConvertExa(mintwodigits);

        token = strtok(NULL, ":");
        int sectwodigits = parseInt(token);
        printf("Third %d \r\n", sectwodigits);
        csegtwodigits = ConvertExa(sectwodigits);
    }else{
        printf("ERROR LA CADENA NO SE INGRESO CORRECTAMENTE\r\n--------------\r\n--------------\r\n--------------\r\n--------------\r\n");
    }
    //WPRINT_APP_INFO(("Numero  %d \r\n", strlen(enter)));
    wbuf = malloc(sizeof(uint8_t)*4);
    if (wbuf == NULL)
    {
        WPRINT_APP_INFO( ("Unable to allocate wbuf\n" ));
        //return result;
    }

    rbuf = malloc(sizeof(uint8_t)*4);
    if (rbuf == NULL)
    {
        WPRINT_APP_INFO( ("Unable to allocate rbuf\n" ));
        free( wbuf );
        //return result;
    }

    memset(wbuf, 0, (sizeof(uint8_t)*4));
    memset(rbuf, 0, (sizeof(uint8_t)*4));

    /* Initialize I2C */
    if ( wiced_i2c_init( &i2c_device_rtc ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ( "I2C Initialization Failed\n" ) );
        free( rbuf );
        free( wbuf );
        //return WICED_ERROR;
    }

    /* Probe I2C bus for temperature sensor */
    if( wiced_i2c_probe_device( &i2c_device_rtc, NUM_I2C_MESSAGE_RETRIES ) != WICED_TRUE )
    {
        WPRINT_APP_INFO( ( "Failed to connect to temperature device; addr 0x%x\n", i2c_device_rtc.address ) );
        free( rbuf );
        free( wbuf );
        //return WICED_ERROR;
    }

//    wbuf[0] = HTS221_WOAMI_REG;
//    wiced_i2c_write( &i2c_device_temperature, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
//    wiced_i2c_read( &i2c_device_temperature, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
//    if( rbuf[0] != 0xbc )
//    {
//        WPRINT_APP_INFO( ( "Failed to read WHOAMI from temperature device; addr 0x%x\n", i2c_device_temperature.address ) );
//        free( rbuf );
//        free( wbuf );
//        return WICED_ERROR;
//    }
    //WPRINT_APP_INFO( ( "HTS221 device (0x%x) at address 0x%x\n", rbuf[0], i2c_device_temperature.address ) );
    //WPRINT_APP_INFO( ( "Order 2:  %x\n" , rbuf[0]) );

    /* Power-up the device */
    wbuf[0] = HREG_X8;
    //wbuf[1] = (HTS221_CTRL1_PD | HTS221_CTRL1_BDU);
    wbuf[1] = chourtwodigits;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2);
    WPRINT_APP_INFO( ( "Writing HOURS: %x\n", wbuf[1]) );
    free( wbuf );
    free( rbuf );
    wbuf[0] = MINREG_X8;
    //wbuf[1] = (HTS221_CTRL1_PD | HTS221_CTRL1_BDU);
    wbuf[1] = cmintwodigits;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2);
    WPRINT_APP_INFO( ( "Writing MINUTES: %x\n", wbuf[1]) );
    free( wbuf );
    free( rbuf );
    wbuf[0] = SEGREG_X8;
    //wbuf[1] = (HTS221_CTRL1_PD | HTS221_CTRL1_BDU);
    wbuf[1] = csegtwodigits;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2);
    WPRINT_APP_INFO( ( "Writing SECONDS: %x\n", wbuf[1]) );
    free( wbuf );
    free( rbuf );
    wiced_rtos_delay_milliseconds( 1000 );
    //return WICED_SUCCESS;
}
int date_set(char *enter)
{

    //char enter[9]="59:15:32";
    //time_set(enter);

    //time_set(enter);
    int cdaytwodigits;
    int cmonthtwodigits;
    int cbyearfourdigits;


    uint8_t *wbuf;
    uint8_t *rbuf;
    //printf("size %zu \r\n", strlen(enter));
    if(strlen(enter) == 8){
        //printf("Fecha aceptada: %s \r\n", enter);  //day declaration
        char * token = strtok(enter, "/");
        int daytwodigits = parseInt(token);
        cdaytwodigits=ConvertExa(daytwodigits);
        //printf("First %d \r\n", cdaytwodigits);


        token = strtok(NULL, "/");
        int monthtwodigits = parseInt(token);
        //printf("Second %d \r\n", monthtwodigits);
        cmonthtwodigits = ConvertExa(monthtwodigits);
        //printf("b %d \r\n", cmonthtwodigits);

        token = strtok(NULL, "/");
        int yearfourdigits = parseInt(token);
        //printf("Third %d \r\n", yearfourdigits);
        //int module = yearfourdigits % 2000;
        cbyearfourdigits = ConvertExa(yearfourdigits);
        //int cbyearfourdigits = moduleexa + 8192;
        //printf("Third %d \r\n", cbyearfourdigits);
    }else{
        printf("ERROR LA CADENA NO SE INGRESO CORRECTAMENTE\r\n--------------\r\n--------------\r\n--------------\r\n--------------\r\n");
    }
    //WPRINT_APP_INFO(("Numero  %d \r\n", strlen(enter)));
    wbuf = malloc(sizeof(uint8_t)*4);
    if (wbuf == NULL)
    {
        WPRINT_APP_INFO( ("Unable to allocate wbuf\n" ));
        //return result;
    }

    rbuf = malloc(sizeof(uint8_t)*4);
    if (rbuf == NULL)
    {
        WPRINT_APP_INFO( ("Unable to allocate rbuf\n" ));
        free( wbuf );
        //return result;
    }

    memset(wbuf, 0, (sizeof(uint8_t)*4));
    memset(rbuf, 0, (sizeof(uint8_t)*4));

    /* Initialize I2C */
    if ( wiced_i2c_init( &i2c_device_rtc ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ( "I2C Initialization Failed\n" ) );
        free( rbuf );
        free( wbuf );
        //return WICED_ERROR;
    }

    /* Power-up the device */
    wbuf[0] = DAYREG_X8;
    //wbuf[1] = (HTS221_CTRL1_PD | HTS221_CTRL1_BDU);
    wbuf[1] = cdaytwodigits;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2);
    WPRINT_APP_INFO( ( "Writing DAY: %x\n", wbuf[1]) );
    free( wbuf );
    free( rbuf );
    wbuf[0] = MONTHREG_X8;
    //wbuf[1] = (HTS221_CTRL1_PD | HTS221_CTRL1_BDU);
    wbuf[1] = cmonthtwodigits;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2);
    WPRINT_APP_INFO( ( "Writing MONTHS: %x\n", wbuf[1]) );
    free( wbuf );
    free( rbuf );
    wbuf[0] = YEARREG_X8;
    //wbuf[1] = (HTS221_CTRL1_PD | HTS221_CTRL1_BDU);
    wbuf[1] = cbyearfourdigits;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 2);
    WPRINT_APP_INFO( ( "Writing YEAR: %x\n", wbuf[1]) );
    free( wbuf );
    free( rbuf );
    wiced_rtos_delay_milliseconds( 1000 );
    //return WICED_SUCCESS;
}


char * time_get(){
    uint8_t *wbuf;
    uint8_t *rbuf;
    int16_t T0, T1, T2;
    wbuf = malloc(sizeof(uint8_t)*4);

    rbuf = malloc(sizeof(uint8_t)*4);

    memset(wbuf, 0, (sizeof(uint8_t)*4));
    memset(rbuf, 0, (sizeof(uint8_t)*4));

    // Leemos aqui los segundos
//    //wbuf[0] = HTS221_T0_DEGC_X8;
    wbuf[0] = SEGREG_X8;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    //wiced_i2c_read( &i2c_device_temperature, WICED_I2C_START_FLAG  | WICED_I2C_STOP_FLAG, rbuf, 1 );
    wiced_i2c_read( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T0 = rbuf[0];
    //WPRINT_APP_INFO( ( "Segundos:  %02x\n" , T0) );
//    int nDigits = floor(log10(abs(T0))) + 1;
//    if (nDigits==1){
//
//    }
    //WPRINT_APP_INFO(("DIGITS:%x\n" ,nDigits));

    // Read 1 byte of data from address 0x33(51)
    wbuf[0] = MINREG_X8;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T1 = rbuf[0];
    //WPRINT_APP_INFO( ( "Minutos:  %02x\n" , T1) );

    // Read 1 byte of data from address 0x35(53)
    wbuf[0] = HREG_X8;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T2 = rbuf[0];
    //WPRINT_APP_INFO( ( "Horas:  %02x\n" , T2) );


    free( rbuf );
    free( wbuf );

    char *todo1;
    char *todo2;

//    datos="EMPYT CHAjkhjhkjR";
//    WPRINT_APP_INFO( ( "XXXXXXXX:  %s\n" , datos) );

    //WPRINT_APP_INFO(("Sin errores\r\n"));
    //WPRINT_APP_INFO(datos);
    todo1 = malloc(sizeof(char)*10);
    todo2 = malloc(sizeof(char)*10);

    sprintf(todo1, "%02x:%02x:%02x", T2, T1, T0);
    //sprintf(todo2, "%x Tracking chain \r\n", T1);
    WPRINT_APP_INFO( ( "TIEMPO REAL======>  %s\n" , todo1) );
    return todo1;
}
char * date_get(){
    uint8_t *wbuf;
    uint8_t *rbuf;
    int16_t T0, T1, T2;
    wbuf = malloc(sizeof(uint8_t)*4);
    if (wbuf == NULL)
    {
        WPRINT_APP_INFO( ("Unable to allocate wbuf\n" ));
        //return result;
    }

    rbuf = malloc(sizeof(uint8_t)*4);
    if (rbuf == NULL)
    {
        WPRINT_APP_INFO( ("Unable to allocate rbuf\n" ));
        free( wbuf );
        //return result;
    }

    memset(wbuf, 0, (sizeof(uint8_t)*4));
    memset(rbuf, 0, (sizeof(uint8_t)*4));

    // Temperature Calibration values
    // Read 1 byte of data from address 0x32(50)
    // Leemos aqui los segundos
//    //wbuf[0] = HTS221_T0_DEGC_X8;
    wbuf[0] = DAYREG_X8;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    //wiced_i2c_read( &i2c_device_temperature, WICED_I2C_START_FLAG  | WICED_I2C_STOP_FLAG, rbuf, 1 );
    wiced_i2c_read( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T0 = rbuf[0];
    //WPRINT_APP_INFO( ( "Day:  %02x\n" , T0) );
//    int nDigits = floor(log10(abs(T0))) + 1;
//    if (nDigits==1){
//
//    }
    //WPRINT_APP_INFO(("DIGITS:%x\n" ,nDigits));

    // Read 1 byte of data from address 0x33(51)
    wbuf[0] = MONTHREG_X8;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T1 = rbuf[0];
    //WPRINT_APP_INFO( ( "Month:  %02x\n" , T1) );

    // Read 1 byte of data from address 0x35(53)
    wbuf[0] = YEARREG_X8;
    wiced_i2c_write( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, wbuf, 1 );
    wiced_i2c_read( &i2c_device_rtc, WICED_I2C_START_FLAG | WICED_I2C_STOP_FLAG, rbuf, 1 );
    T2 = rbuf[0];
    //WPRINT_APP_INFO( ( "Year:  %02x\n" , T2) );



    //WPRINT_APP_INFO( ( "HTS221 temperature %.1f C, %.1f F\n", tempC, tempF ) );

    free( rbuf );
    free( wbuf );

    char *todo1;
    char *todo2;

//    datos="EMPYT CHAjkhjhkjR";
//    WPRINT_APP_INFO( ( "XXXXXXXX:  %s\n" , datos) );

    //WPRINT_APP_INFO(("Sin errores\r\n"));
    //WPRINT_APP_INFO(datos);
    todo1 = malloc(sizeof(char)*40);
    if (todo1 == NULL)
    {
        WPRINT_APP_INFO( ("Unable to allocate wbuf\n" ));
        //return 0;
    }
    todo2 = malloc(sizeof(char)*40);
        if (todo2 == NULL)
        {
            WPRINT_APP_INFO( ("Unable to allocate wbuf\n" ));
            //return 0;
        }

    sprintf(todo1, "%02x/%02x/20%02x", T0, T1, T2);
    //sprintf(todo2, "%x Tracking chain \r\n", T1);
    WPRINT_APP_INFO( ( "FECHA======>  %s\n" , todo1) );
//    wiced_rtos_delay_milliseconds( 1000 );
//    strcpy(*todo,todo1);
    return todo1;
}


//char *datosss;
//char *dato;
//
//    char tiempoentrada[9]="18:52:10";
//    char fechaentrada[9]="24/06/22";
//
//    time_set(tiempoentrada);        ESTO ESTA AQUI COMENTADO PARA SER UN EJEMPLO DE COMO UTILIZARLO
//    date_set(fechaentrada);
//
//    char[9] tiemporeal=time_get();
//    char[9] fechareal=date_get()));



/*
 * Initializes I2C, probes for temperature device
 */
int powInt(int x, int y)
    {
        for (int i = 0; i < y; i++)
        {
            x *= 10;
        }
        return x;
    }
int parseInt(char* chars)
    {
        int sum = 0;
        int len = strlen(chars);
        for (int x = 0; x < len; x++)
        {
            int n = chars[len - (x + 1)] - '0';
            sum = sum + powInt(n, x);
        }
        return sum;
    }
int ConvertExa(int numberDec)
    {
        int quotient, module;
        long int temp;
        quotient = numberDec / 10;
        module = numberDec % 10;
        //printf("quoting %d: ", quotient);
        //printf("quoting %d: ", module);
        if( quotient == 1){
            temp =numberDec + 6;

        }else if( quotient == 2){
            temp =numberDec + 12;

        }else if( quotient == 3){
            temp =numberDec + 18;

        }else if( quotient == 4){
            temp =numberDec + 24;

        }else if( quotient == 5){
            temp =numberDec + 30;

        }else{
            temp = numberDec;
        }
        //printf("RESULTADO %d \r\n", temp);
        //printf("EXADECIMAL %x\r\n", temp);
        return temp;
    }

