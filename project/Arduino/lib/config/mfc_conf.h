#ifndef MFC_CONFIG_H
#define MFC_CONFIG_H

#define Close_MFC1 26
#define Close_MFC2 27
#define Close_MFC3 28
#define Close_MFC4 29

#define MFC A0
#define PWM_INPUT1 2

#define MFC2 A1
#define PWM_INPUT2 3

#define MFC3 A2
#define PWM_INPUT3 4

#define MFC4 A3
#define PWM_INPUT4 5

const int sinalMKS = 5; 

float MFC_V = 0;
char buff[10];

uint32_t number2 = 0;  // Create variable to store value of slider

int mcfOutput1 = 1 ;
int mcfOutput2 = 1 ;
int mcfOutput3 = 1 ;
int mcfOutput4 = 1 ;


uint32_t mfcSCCM1 = 500 ;
uint32_t mfcSCCM2 = 500 ;
uint32_t mfcSCCM3 = 500 ;
uint32_t mfcSCCM4 = 500 ;

float Calibrated_MFC_1 = 1;
float Calibrated_MFC_2 = 1;
float Calibrated_MFC_3 = 1;
float Calibrated_MFC_4 = 1;

uint32_t checkbox1=0;
int timer_enable = 0;
int timer_enable_pulse = 0;
uint32_t timerMFC1 = 0;
uint32_t timerMFC1_pulse = 0;
uint32_t timerMFC1_pulse_on = 0;
uint32_t timerMFC1_pulse_off = 0;
int delta=0;
int delta_pulse=0;
uint32_t MFC1_mode_linear;
uint32_t MFC1_mode_pulse;
int total1;
float cycle_on;


uint32_t checkbox2=0;
int timer_enable2 = 0;
int timer_enable_pulse2 = 0;
uint32_t timerMFC2 = 0;
uint32_t timerMFC2_pulse = 0;
uint32_t timerMFC2_pulse_on = 0;
uint32_t timerMFC2_pulse_off = 0;
int delta2=0;
int delta2_pulse=0;
uint32_t MFC2_mode_linear;
uint32_t MFC2_mode_pulse;
int total2;
float cycle2_on;

uint32_t checkbox3=0;
int timer_enable3 = 0;
int timer_enable_pulse3 = 0;
uint32_t timerMFC3 = 0;
uint32_t timerMFC3_pulse = 0;
uint32_t timerMFC3_pulse_on = 0;
uint32_t timerMFC3_pulse_off = 0;
int delta3=0;
int delta3_pulse=0;
uint32_t MFC3_mode_linear;
uint32_t MFC3_mode_pulse;
int total3;
float cycle3_on;

uint32_t checkbox4=0;
int timer_enable4 = 0;
int timer_enable_pulse4 = 0;
uint32_t timerMFC4 = 0;
uint32_t timerMFC4_pulse = 0;
uint32_t timerMFC4_pulse_on = 0;
uint32_t timerMFC4_pulse_off = 0;
int delta4=0;
int delta4_pulse=0;
uint32_t MFC4_mode_linear;
uint32_t MFC4_mode_pulse;
int total4;
float cycle4_on;




#endif