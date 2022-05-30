#ifndef CLOCK_CONF_H
#define CLOCK_CONF_H

#include <virtuabotixRTC.h>
#include <TimeLib.h>

#include <UnixTime.h>
UnixTime stamp(-3);


// Creation of the Real Time Clock Object
#define Clk 8  // Pino Clk
#define Dat 22 // Pino Dat
#define Rst 23 // Pino Rst

tmElements_t my_time;  // time elements structure
time_t unix_timestamp; // a timestamp

virtuabotixRTC myRTC(Clk, Dat, Rst);

String TimeString;


#endif