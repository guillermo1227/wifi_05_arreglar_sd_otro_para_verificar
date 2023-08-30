################################################################################
#
# Copyright (C) 2018-2021 LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
# All rights reserved.
#
# This document is the property of LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
# It is considered confidential and proprietary.
#
# This document may not be reproduced or transmitted in any form,
# in whole or in part, without the express written permission of
# LASECÂ®ï¸� Telecomunicaciones S.A.P.I. de C.V.
#
################################################################################

NAME := apps_SmartFlow_SmartTorch_tag_vh4

$(NAME)_SOURCES := tag_vh4.c
#$(NAME)_SOURCES    += i2c.c

WIFI_CONFIG_DCT_H := wifi_config_dct.h

APPLICATION_DCT := DCT/dct_data_write.c


$(NAME)_COMPONENTS := graphics/u8g

GLOBAL_DEFINES += WICED_DISABLE_STDIO
