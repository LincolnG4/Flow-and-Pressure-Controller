#include <SD.h>
#include <SPI.h>
#include <stdlib.h>
#include "Nextion.h"
#define ledPin 13
#define MFC A0
#define PWM_INPUT1 2
#define pinCS 53
#define Read_Pirani A2

//RTC Real time clock
#include <virtuabotixRTC.h>
#include "TimeLib.h"
// Creation of the Real Time Clock Object
#define Clk 8 // Pino Clk
#define Dat 22 // Pino Dat
#define Rst 23 // Pino Rst

tmElements_t my_time;  // time elements structure
time_t unix_timestamp; // a timestamp

virtuabotixRTC myRTC(Clk, Dat, Rst);

//SD CARD
File Pirani;


//PIRANI
  
const float SinalMCU = 5;
const int bitsMCU = 1023.0000;
const int sinalPirani = 10; 

float Pirani_V = 0;
double Pirani_Torr = 0.000;
String piraniReadPrint;


//MFC
const int sinalMKS = 5; 
float MFC_V = 0;
char buff[10];
uint32_t number2 = 0;  // Create variable to store value of slider


//NEXTION
long randNumber;
char buf[10];

int CurrentPage = 0;  // Create a variable to store which page is currently loaded

//NEXTION
NexPage p0 = NexPage(0,0,"page0");
NexButton p0_b0 = NexButton(0,1,"b0");
NexButton p0_b1 = NexButton(0,2,"b1");
NexButton p0_b2 = NexButton(2,6,"b0");
NexDSButton bt0 = NexDSButton(1, 1, "bt0");
NexDSButton bt1 = NexDSButton(1, 3, "bt1");
NexDSButton bt8 = NexDSButton(2, 59, "bt8");
NexText t0 = NexText(1, 2, "t0");
NexText t1 = NexText(1, 5, "t1");
NexText t3 = NexText(1, 7, "t3");
NexText t4 = NexText(1, 7, "t4");
NexText t6 = NexText(1, 10, "t6");
NexText t7 = NexText(1, 11, "t7");
NexText t8 = NexText(2, 2, "t0");
NexText t9 = NexText(3, 26, "t4");
NexText t10 = NexText(2, 55, "t25");
NexText t11 = NexText(2, 56, "t26");
NexText t12 = NexText(2, 57, "t27");
NexText t13 = NexText(2, 58, "t28");
NexText t14 = NexText(2, 59, "t29");
NexNumber n0 = NexNumber(2, 18, "n0");
NexNumber n1 = NexNumber(2, 19, "n1");
NexSlider h0 = NexSlider(1, 5, "h0");
NexCheckbox c0 = NexCheckbox(2,6,"c0");
NexCheckbox c1 = NexCheckbox(2,3,"c1"); 
NexCheckbox c2 = NexCheckbox(2,8,"c2");
NexCheckbox c3 = NexCheckbox(2,9,"c3");


NexTouch *nex_listen_list[] = {
  &p0_b0,&p0_b1,&p0_b2,&t0, &bt0,&bt1,&bt8,&h0,&c0,&c1,&c2,&c3,&n0, NULL
};

uint32_t next, myInt = 0;
uint32_t dual_state; 
uint32_t dual_state2; 
int s;
uint32_t number = 0;
char temp[10] = {0};

uint32_t checkbox1;
int timer_enable = 0;
unsigned long timerMFC1 = 0;
long delta=0;


uint32_t checkbox2; 
uint32_t checkbox3; 
uint32_t checkbox4; 

void p0_b0_Press(void *ptr) {
  CurrentPage = 1;
}
void p0_b1_Press(void *ptr) {
  CurrentPage = 2;
}
void p0_b4_Press(void *ptr) {
  c0.getValue(&checkbox1);
  c1.getValue(&checkbox2);
  c2.getValue(&checkbox3);
  c3.getValue(&checkbox4);
}

/*
void bt4_Press(void *ptr) {
  digitalWrite(ledPin, LOW);
}

void bt0_Press(void *ptr) {
  digitalWrite(ledPin, LOW);
}
*/
char buffer[100] = {0};

float PiradaRead(){
  Pirani_V = (sinalPirani/SinalMCU)*((SinalMCU*(analogRead(Read_Pirani)))/bitsMCU); 
  //Pirani_Torr = 10000.0000*(pow(10,(Pirani_V-6.125)));
  return Pirani_V;
}

float MFCRead(){
  MFC_V = (sinalMKS/SinalMCU)*((SinalMCU*(analogRead(MFC)))/bitsMCU); 
  //Pirani_Torr = 10000.0000*(pow(10,(Pirani_V-6.125)));
  return MFC_V;
}

void SendTextT(float Pirani_V){
    dtostrf(Pirani_V, 4, 3, buff);
    t1.setText(buff);
}

void SendTextT3(float Pirani_V){
    dtostrf(Pirani_V, 4, 3, buff);
    t3.setText(buff);
}

void SendTextT4(float Pirani_V){
    dtostrf(Pirani_V, 4, 3, buff);
    t10.setText(buff);
}

void WriteSD(File myFile){
    if (myFile) {
    //Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    //Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
}


int TimestampConverter(int s,int h,int m,int d,int mon,int y){
  my_time.Second = s;
  my_time.Hour = h;
  my_time.Minute = m;
  my_time.Day = d;
  my_time.Month = mon ;      // months start from 0, so deduct 1
  my_time.Year = y - 1970; // years since 1970, so deduct 1970

  unix_timestamp =  makeTime(my_time);
  return unix_timestamp;
}

void setup() {
  Serial2.begin(115200);
  Serial.begin(9600);

  pinMode(PWM_INPUT1, OUTPUT);

  pinMode(pinCS, OUTPUT);
 
  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(10, 17, 3, 3, 29, 3, 2022);

 
  pinMode(ledPin,OUTPUT);
  analogWrite(PWM_INPUT1, 0);
  p0_b0.attachPop(p0_b0_Press, &p0_b0);
  p0_b1.attachPop(p0_b1_Press, &p0_b1);
  //p0_b4.attachPop(p0_b4_Press, &p0_b4);
  //bt0.attachPop(bt0_Press, &bt0);
  //bt1.attachPop(bt1_Press, &bt1);

  // SD Card Initialization
  if (SD.begin())
  {
    //Serial.println("SD card is ready to use.");
    boolean SD_Status= true;
  } else
  {
    //Serial.println("SD card initialization failed");
    return;
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  delay(30);
  
  nexLoop(nex_listen_list); // NEXTION STARTS


  if(CurrentPage == 1){
        // PIRANI BUTTON
        bt0.getValue(&dual_state);
        
        if(dual_state) 
        {
             
          Pirani_V = PiradaRead();
          SendTextT(Pirani_V);
          String dataString1 = " 1 , " + String(Pirani_V) ;
          Pirani = SD.open("Pirani.csv", FILE_WRITE);
          if (Pirani)                                                                       
               {                                                                                
               Pirani.println(dataString1);
               Pirani.close();                                                   
               }    
        }
      
      
      
        // MKS BUTTON
        bt1.getValue(&dual_state2);
        if(dual_state2) 
        {
          h0.getValue(&number);
          int val = number;
          val = map(val, 0, 100, 0, 255);
          analogWrite(PWM_INPUT1, val);
          
          
          float number3 = number/20.00;
          dtostrf(number3, 4, 2, temp);
          t3.setText(temp);
          uint32_t number4 = number*5;
          dtostrf(number4, 4, 0, temp);
          t4.setText(temp);
          
          float number5 = MFCRead();
          dtostrf(number5, 4, 2, temp);
          t6.setText(temp);
          
          uint32_t number6 = number5*100;
          dtostrf(number6, 4, 0, temp);
          t7.setText(temp);
      
        }
  }
  
  if(CurrentPage == 2){
    myRTC.updateTime();
    
    Pirani_V = PiradaRead();
    SendTextT4(Pirani_V);
    //DS BUTTON
    bt8.getValue(&dual_state);
    
    if(dual_state) 
    {
      
      c0.getValue(&checkbox1);
      c1.getValue(&checkbox2);
      c2.getValue(&checkbox3);
      c3.getValue(&checkbox4);
         
      if(checkbox1){
        
        unsigned long start_mfc1;
        
        if(timer_enable == 0){
          n1.getValue(&timerMFC1);
          start_mfc1 = millis()/1000;
          timer_enable = 1;
        }
        
        unsigned long currentMillis = millis()/1000;
        delta = currentMillis-start_mfc1;
        
        if(timerMFC1 >= delta){
           digitalWrite(ledPin, HIGH);
           dtostrf(delta, 4, 2, temp);
           t11.setText(temp);
        }
        else{
           digitalWrite(ledPin, LOW);
        }

        /*
        digitalWrite(ledPin, HIGH);
        uint32_t number;
        n0.getValue(&number);
        int val = number;
        val = map(val, 0, 100, 0, 255);
        analogWrite(PWM_INPUT1, val);
        int number5 = MFCRead();
        dtostrf(number5, 4, 2, temp);
        t11.setText(temp);
        */
       }
     if(checkbox1 == 0){
      timer_enable = 0;
      digitalWrite(ledPin, LOW);
      delta=0;
     }
       if(checkbox2){
            
            
           }
       if(checkbox3){
            
            
           }
       if(checkbox4){
            
            
           }   
      
        }
  }
}
