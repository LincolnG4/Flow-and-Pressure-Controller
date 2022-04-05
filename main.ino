#include <SD.h>
#include <SPI.h>
#include <stdlib.h>
#include "Nextion.h"
#define ledPin 13


/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - RTC ///////////////////////////
///////////////////////////////////////////////////////////////

#include <virtuabotixRTC.h>
#include "TimeLib.h"

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

/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - SDCARD ////////////////////////
///////////////////////////////////////////////////////////////

#define pinCS 53

File Pirani_CSV;
File MFC_CSV_1;
File MFC_CSV_2;
File MFC_CSV_3;
File MFC_CSV_4;



/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - PIRANI ////////////////////////
///////////////////////////////////////////////////////////////

#define Read_Pirani A5

const float SinalMCU = 5;
const int bitsMCU = 1023.0000;
const int sinalPirani = 10; 

float Pirani_V = 0;
double Pirani_Torr = 0.000;
String piraniReadPrint;


/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - MFC ///////////////////////////
///////////////////////////////////////////////////////////////

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

int mcfOutput1 ;
int mcfOutput2 ;
int mcfOutput3 ;
int mcfOutput4 ;
/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - NEXTION ///////////////////////
///////////////////////////////////////////////////////////////

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
NexText t10 = NexText(2, 54, "t25");
NexText t11 = NexText(2, 55, "t26");
NexText t12 = NexText(2, 56, "t27");
NexText t13 = NexText(2, 57, "t28");
NexText t14 = NexText(2, 58, "t29");
NexText t15 = NexText(2, 60, "t30");
NexText t16 = NexText(2, 61, "t31");
NexText t17 = NexText(2, 62, "t32");
NexText t18 = NexText(2, 63, "t33");
NexNumber n0 = NexNumber(2, 18, "n0");
NexNumber n1 = NexNumber(2, 19, "n1");
NexNumber n2 = NexNumber(2, 30, "n5");
NexNumber n3 = NexNumber(2, 26, "n2");
NexNumber n4 = NexNumber(2, 44, "n13");
NexNumber n5 = NexNumber(2, 38, "n10");
NexNumber n6 = NexNumber(2, 28, "n8");
NexNumber n7 = NexNumber(2, 36, "n9");
NexSlider h0 = NexSlider(1, 5, "h0");
NexCheckbox c0 = NexCheckbox(2,6,"c0");
NexCheckbox c1 = NexCheckbox(2,3,"c1"); 
NexCheckbox c2 = NexCheckbox(2,8,"c2");
NexCheckbox c3 = NexCheckbox(2,9,"c3");


NexTouch *nex_listen_list[] = {
  &p0_b0,&p0_b1,&p0_b2,&t0, &bt0,&bt1,&bt8,&h0,&c0,&c1,&c2,&c3,&n0,&n3,&n6, NULL
};

uint32_t next, myInt = 0;
uint32_t dual_state; 
uint32_t dual_state2; 
int s;
uint32_t number = 0;
char temp[10] = {0};

uint32_t checkbox1;
int timer_enable = 0;
uint32_t timerMFC1 = 0;
int delta=0;


uint32_t checkbox2;
int timer_enable2 = 0;
uint32_t timerMFC2 = 0;
int delta2=0;

uint32_t checkbox3;
int timer_enable3 = 0;
uint32_t timerMFC3 = 0;
int delta3=0;

uint32_t checkbox4; 
int timer_enable4 = 0;
uint32_t timerMFC4 = 0;
int delta4=0;

uint32_t numberu1;
uint32_t numberu2;
uint32_t numberu3;
uint32_t numberu4;


/////////////////////////////////////////////////////////////////
//////////////////// FUNCTIONS /////////////////////////////////
///////////////////////////////////////////////////////////////


void p0_b0_Press(void *ptr) {
  CurrentPage = 13;
}
void p0_b1_Press(void *ptr) {
  CurrentPage = 1;
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
char buffer2[100] = {0};
char buffer3[100] = {0};
char buffer4[100] = {0};


float PiradaRead(){
  Pirani_V = (sinalPirani/SinalMCU)*((SinalMCU*(analogRead(Read_Pirani)))/bitsMCU); 
  //Pirani_Torr = 10000.0000*(pow(10,(Pirani_V-6.125)));
  return Pirani_V;
}

float MFCRead(int MFC){
  MFC_V = (sinalMKS/SinalMCU)*((SinalMCU*(analogRead(MFC)))/bitsMCU); 
  //Pirani_Torr = 10000.0000*(pow(10,(Pirani_V-6.125)));
  return 100*MFC_V;
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

void WriteSD(File myFile,String message){
  if (myFile) {
    //Serial.print("Writing to test.txt...");
    myFile.println(message);
    // close the file:
    myFile.close();
    //Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
}



void MfcOutputWrite(int mcfOutput,char* mfcOutputString,NexText t){
  itoa(mcfOutput,mfcOutputString, 10);     
  t.setText(mfcOutputString);
}

void MfcPwmON(int PWM_INPUT,int delta,char* buffer,NexText t,NexNumber n,uint32_t number){
  n.getValue(&number);
  int val = number;
  val = map(val, 0, 500, 0, 255);
  analogWrite(PWM_INPUT, val);
  
  itoa(delta,buffer, 10);
  t.setText(buffer);
}

void MfcPwmOFF(int PWM_INPUT,NexCheckbox c){
  analogWrite(PWM_INPUT, 0);
  c.setValue(0);
}

void setup() {
  Serial2.begin(115200);
  Serial.begin(115200);

  //PINOUT MFC-PWM 
  pinMode(PWM_INPUT1, OUTPUT);
  pinMode(PWM_INPUT2, OUTPUT);
  pinMode(PWM_INPUT3, OUTPUT);
  pinMode(PWM_INPUT4, OUTPUT);


  pinMode(pinCS, OUTPUT);
 
  // Set the current date, and time in the following format:
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(10, 17, 3, 3, 29, 3, 2022);

 
  pinMode(ledPin,OUTPUT);

  analogWrite(PWM_INPUT1, 0);
  analogWrite(PWM_INPUT2, 0);
  analogWrite(PWM_INPUT3, 0);
  analogWrite(PWM_INPUT4, 0);

  p0_b0.attachPop(p0_b0_Press, &p0_b0);
  p0_b1.attachPop(p0_b1_Press, &p0_b1);

  
  /////////////////////////////////////////////////////////////////
 ///////////////////// SD - CARD  ////////////////////////////////
/////////////////////////////////////////////////////////////////

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


    /////////////////////////////////////////////////////////////////
   //////////////////// PAGINA 1- MAIN PAGE ////////////////////////
  /////////////////////////////////////////////////////////////////

  if(CurrentPage == 1){

    CurrentPage = 1;
    //PRINT PIRANI VALUES 
    Pirani_V = PiradaRead();
    SendTextT4(Pirani_V);

    //DS BUTTON
    bt8.getValue(&dual_state);
    
    //IF BOTAO MFC TURNED ON
    if(dual_state) 
    {
      
      //GET STATE MFC CHECKBOX
      c0.getValue(&checkbox1);
      c1.getValue(&checkbox2);
      c2.getValue(&checkbox3);
      c3.getValue(&checkbox4);


      //READ MFC AND PRINT ON SCREEN
      char mfcOutputString[7];

      //MFC 1
      mcfOutput1 = MFCRead(MFC);
      MfcOutputWrite(mcfOutput1,mfcOutputString,t11);
      //MFC 2
      mcfOutput2 = MFCRead(MFC2);
      MfcOutputWrite(mcfOutput2,mfcOutputString,t12);
      //MFC 3
      mcfOutput3 = MFCRead(MFC3);
      MfcOutputWrite(mcfOutput3,mfcOutputString,t13);
      //MFC 4
      mcfOutput4 = MFCRead(MFC4);
      MfcOutputWrite(mcfOutput4,mfcOutputString,t14);

      myRTC.updateTime();
      TimeString = String(myRTC.dayofmonth) + "/" + String(myRTC.month)+ "/" +  String(myRTC.year)  + " " + String(myRTC.hours)  + ":" + String(myRTC.minutes) + ":" + String(myRTC.seconds);
      stamp.setDateTime(myRTC.year, myRTC.month, myRTC.dayofmonth, myRTC.hours, myRTC.minutes, myRTC.seconds);
      uint32_t unix = stamp.getUnix();

      uint32_t start_mfc1;
      uint32_t start_mfc2;
      uint32_t start_mfc3;
      uint32_t start_mfc4;

      //CREATE SAVE A SD CARD
      Pirani_CSV = SD.open("PIRANI.csv", FILE_WRITE);
      String messagePirani = TimeString + "," + String(PiradaRead()); //
      WriteSD(Pirani_CSV,messagePirani);
      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 1 - MFC 1 /////////////////////////
    /////////////////////////////////////////////////////////////////
      if(checkbox1){
        
        //CREATE SAVE A SD CARD
        MFC_CSV_1 = SD.open("MFC1.csv", FILE_WRITE);

        if(timer_enable == 0){
          n1.getValue(&timerMFC1);
          start_mfc1 = unix;
          timer_enable = 1;
        }
        
        delta = unix - start_mfc1;

        if(timerMFC1+1 > delta){

          digitalWrite(ledPin, HIGH);
          // uint32_t number;

          MfcPwmON(PWM_INPUT1,delta,buffer,t15,n0,numberu1);
          String message1 = TimeString + "," + String(MFCRead(MFC)); //
          WriteSD(MFC_CSV_1,message1);
          // n0.getValue(&number);
          // int val = number;
          // val = map(val, 0, 500, 0, 255);
          // analogWrite(PWM_INPUT1, val);
          
          // itoa(delta,buffer, 10);
          // t15.setText(buffer);

        }
        else {
          MfcPwmOFF(PWM_INPUT1,c0);
          digitalWrite(ledPin, LOW);
          checkbox1 = 0;
          // c0.setValue(0);
          
          // analogWrite(PWM_INPUT1, 0);  
        }

      }
      else {
        timer_enable = 0;
        digitalWrite(ledPin, LOW);
        int delta = 0;
      }


      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 2 - MFC 2 /////////////////////////
    /////////////////////////////////////////////////////////////////

      if(checkbox2){

        //CREATE SAVE A SD CARD
        MFC_CSV_2 = SD.open("MFC2.csv", FILE_WRITE);

        if(timer_enable2 == 0){
          //MFC_CSV_2 = SD.open("MFC2.csv", FILE_WRITE);
          n2.getValue(&timerMFC2);
          start_mfc2 = unix;
          timer_enable2 = 1;
        }
        // unsigned long currentMillis = millis()/1000;
        delta2 = unix - start_mfc2; 

        if(timerMFC2 +1 > delta2){
          MfcPwmON(PWM_INPUT2,delta2,buffer2,t16,n3,numberu2); 
          String message2 = TimeString + "," + String(MFCRead(MFC2)); //
          WriteSD(MFC_CSV_2,message2); 
        }
        else {
          MfcPwmOFF(PWM_INPUT2,c1);
          checkbox2 = 0;
        }  
      } 

      else {
        timer_enable2 = 0;
        delta2 = 0;
      }


      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 3 - MFC 3 /////////////////////////
    /////////////////////////////////////////////////////////////////
      if(checkbox3){
         
        //CREATE SAVE A SD CARD
        MFC_CSV_3 = SD.open("MFC3.csv", FILE_WRITE);

        if(timer_enable3 == 0){
          n4.getValue(&timerMFC3);
          start_mfc3 = unix;
          timer_enable3 = 1;
        }
        
        // unsigned long currentMillis = millis()/1000;
        delta3 = unix - start_mfc3;
        
        if(timerMFC3 + 1  > delta3){
          MfcPwmON(PWM_INPUT3,delta3,buffer3,t17,n6,numberu3);
          String message3 = TimeString + "," + String(MFCRead(MFC3)); //
          WriteSD(MFC_CSV_3,message3); 
        }
        else {
          MfcPwmOFF(PWM_INPUT3,c2);
          checkbox3 = 0;
        }

      }
      else {
        timer_enable3 = 0;
        delta3 = 0;
      
      }


      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 4 - MFC 4 /////////////////////////
    /////////////////////////////////////////////////////////////////
     if(checkbox4){
       //CREATE SAVE A SD CARD
        MFC_CSV_4 = SD.open("MFC4.csv", FILE_WRITE);
          
      if(timer_enable4 == 0){
          n5.getValue(&timerMFC4);
          start_mfc4 = unix;
          timer_enable4 = 1;
        }
        
        // unsigned long currentMillis = millis()/1000;
        delta4 = unix - start_mfc4;
        
        if(timerMFC4 + 1  > delta4){
          MfcPwmON(PWM_INPUT4,delta4,buffer4,t18,n7,numberu4);
          String message4 = TimeString + "," + String(MFCRead(MFC4)); //
          WriteSD(MFC_CSV_4,message4); 
        }
        else {
          MfcPwmOFF(PWM_INPUT4,c3);
          checkbox4 = 0;
        }

      }
      else {
        timer_enable4 = 0;
        delta4 = 0;
      }  
      
  } 
  else{
    
    checkbox1 = 0;
    delta=0;

    checkbox2 = 0;
    delta2=0;

    checkbox3 = 0;
    delta3=0;

    checkbox4 = 0; 
    delta4=0;
  }
  
  }
}