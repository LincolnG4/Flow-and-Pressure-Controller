#ifndef VARIABLES_H
#define VARIABLES_H




#include <virtuabotixRTC.h>
#include <TimeLib.h>

#include <UnixTime.h>
UnixTime stamp(-3); 

#include <Arduino.h>

#include <SD.h>
#include <SPI.h>
#include <stdlib.h>


#include "./config/mcu_conf.h"
#include "./config/mfc_conf.h"
#include "./config/pirani_conf.h"
#include "./config/sdcard_conf.h"
#include "touch.h"

#define ledPin 13



/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - RTC ///////////////////////////
///////////////////////////////////////////////////////////////

// Creation of the Real Time Clock Object
#define Clk 8  // Pino Clk
#define Dat 22 // Pino Dat
#define Rst 23 // Pino Rst

tmElements_t my_time;  // time elements structure
time_t unix_timestamp; // a timestamp

virtuabotixRTC myRTC(Clk, Dat, Rst);

String TimeString;
String ESP_MESSAGE = "0,0,0,0,0"; //PIRANI,MFC1,MFC2,MFC3,MFC4




/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - ESP01 /////////////////////////
///////////////////////////////////////////////////////////////

int ESP_ENABLE=0;
int ESP_TIMER=0;
int ESP_DELAY=4;


/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - MFC ///////////////////////////
///////////////////////////////////////////////////////////////


float MFC_V = 0;
char buff[10];

uint32_t number2 = 0;  // Create variable to store value of slider

int mcfOutput1 = 1 ;
int mcfOutput2 = 1 ;
int mcfOutput3 = 1 ;
int mcfOutput4 = 1 ;




#endif