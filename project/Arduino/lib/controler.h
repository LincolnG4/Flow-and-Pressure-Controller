#ifndef CONTROLER_H
#define CONTROLER_H

#include <Arduino.h>
#include <stdlib.h>

/*
*  Import libs with configurations of each arduino module 
*  From folder ./config import each lib
*/

#include "./config/mcu_conf.h"
#include "./config/mfc_conf.h"
#include "./config/pirani_conf.h"
#include "./config/sdcard_conf.h"
#include "./config/clock_conf.h"
#include "./config/wifi_conf.h"
#include "touch.h"
#include "pirani.h"
#include "mfc.h"
#include "sdcard.h"

#define ledPin 13 //debug LED

#endif