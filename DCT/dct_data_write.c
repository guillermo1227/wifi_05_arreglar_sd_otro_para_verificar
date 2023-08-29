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


#include "dct_data_write.h"
#include "wiced_framework.h"


/******************************************************
 *               Variable Definitions
 ******************************************************/

DEFINE_APP_DCT(dct_read_write_app_dct_t)
{
    .MASK               = "131.131.131.131",
    .GATE               = "121.121.121.121",
    .IP                 ="101.101.101.101",
    .SERVER             ="111.111.111.111",
    .MAC_BT             ="12:15:14:16:18:11",
    .F_config           =0,
    .F_save             =0,
    .enable_rx          =0,
    .gpio_aux           =0,
    .gpio_recib         =0,
    .gpio_evac          =0,
    .gpio_fall          =0,
    .gpio_inacty        =0,
    .acc_xy             =0,
    .acc_z_min          =0,
    .acc_z_plus         =0,
    .blink_tcp          =0,
    .blink_awake        =0,
    .F_charger          =0,
    .F_chr              =0,
    .F_enable_btn       =0,
    .F_Sate             =0,
    .DCT0               =0,
    .DCT1               =0,
    .DCT2               =0,
    .DCT3               =0,
    .DCT4               =0,
    .DCT5               =NULL,
    .DCT6               =NULL,
    .DCT7               =NULL,
    .DCT8               =NULL,
    .DCT9               =NULL,
    .TxPower            =32,



};

/******************************************************
 *               Function Definitions
 ******************************************************/
