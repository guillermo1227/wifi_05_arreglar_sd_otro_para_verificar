
#include "sd_manager.h"
#include "rtc_manager.h"



//#define SF_DIR         "/SF/"

wiced_filesystem_t fs_handle;
wiced_i2c_device_t i2c_rtc;

char current_file[200];
void application_start( )
{

    wiced_init();
    init_rtc(&i2c_rtc);
    init_sd(&fs_handle);

    dir_verify("/",&fs_handle,"/",TELE);
    dir_verify("/",&fs_handle,"/",SF);

    file_verify(SF_ROOT,&fs_handle,SF_ROOT,date_get(&i2c_rtc));
    file_verify(TELE_ROOT,&fs_handle,TELE_ROOT,date_get(&i2c_rtc));

    list_files(&fs_handle,"/");
    list_files(&fs_handle,SF_ROOT);
    list_files(&fs_handle,TELE_ROOT);



//    file_log_buffer=malloc(sizeof(char)*LOCAL_BUFFER_SIZE);
//    memset(file_log_buffer,NULL,LOCAL_BUFFER_SIZE);
//    WPRINT_APP_INFO( ( "FECHA: \t %s , %d\n" ,date_get(&i2c_rtc),sizeof(file_log_buffer)) );

    read_data(TELE_ROOT,"ff_ff_20ff",&fs_handle);



//
//        char src_pathbuf[PATH_BUFFER_SIZE];
//        strcpy(src_pathbuf,"/tgt/intento3.txt");
//        if(list_files(&fs_handle,"/tgt/","27_06_2022.txt")==NO_FILE){
//
//        }
//        read_data("/tgt/intento3.txt",&fs_handle);
//
//    int g=1;
//        while(g!=10){
//            sprintf(current_file,"{data:%s,time:%s,mac:70:74:14:c0:8d:9e}\n",date_get(&i2c_rtc),time_get(&i2c_rtc));
//            write_data(SF_ROOT,date_get(&i2c_rtc),current_file,&fs_handle);
//            g++;
//        }
//    while(1){
//
//        read_data(SF_ROOT,date_get(&i2c_rtc),&fs_handle);
//        WPRINT_APP_INFO( ( "TIME: \t %s \n" ,time_get(&i2c_rtc)) );
////        WPRINT_APP_INFO( ( "cache: \t , %d\n" ,sizeof(file_log_buffer)) );
//
//                    wiced_rtos_delay_milliseconds(1000);
//
//    }
//
//        read_data(SF_ROOT,date_get(&i2c_rtc),&fs_handle);


    printf( "\nSD Card driver test done successfully.\n\n" );

}

