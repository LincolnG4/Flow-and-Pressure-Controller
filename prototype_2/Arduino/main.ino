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
String ESP_MESSAGE = "0,0,0,0,0"; //PIRANI,MFC1,MFC2,MFC3,MFC4

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
//////////////////// CONFIGURE - ESP01 /////////////////////////
///////////////////////////////////////////////////////////////


int ESP_ENABLE=0;
int ESP_TIMER=0;
int ESP_DELAY=4;

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

int mfcSCCM1 = 500 ;
int mfcSCCM2 = 500 ;
int mfcSCCM3 = 500 ;
int mfcSCCM4 = 500 ;

float Calibrated_MFC_1 = 1;
float Calibrated_MFC_2 = 1;
float Calibrated_MFC_3 = 1;
float Calibrated_MFC_4 = 1;


/////////////////////////////////////////////////////////////////
//////////////////// CONFIGURE - NEXTION ///////////////////////
///////////////////////////////////////////////////////////////

long randNumber;
char buf[10];

int CurrentPage = 0;  // Create a variable to store which page is currently loaded

//NEXTION
NexPage p0 = NexPage(0,0,"page0");
NexPage p1 = NexPage(1,0,"page1");
NexPage p2 = NexPage(2,0,"page2");
NexButton p0_b1 = NexButton(0,1,"b1");
NexButton p0_b2 = NexButton(0,2,"b2");
NexButton p1_b2 = NexButton(2,6,"b2");
NexButton p2_b1 = NexButton(1,1,"b0");
NexButton p1_b0 = NexButton(2,44,"b0");
NexDSButton bt2 = NexDSButton(2, 1, "bt2");
NexDSButton bt3 = NexDSButton(2, 5, "bt3");
NexDSButton bt4 = NexDSButton(2, 12, "bt0");
NexDSButton bt5 = NexDSButton(2, 13, "bt1");
NexDSButton bt6 = NexDSButton(2, 2, "bt6");
NexDSButton bt7 = NexDSButton(2, 16, "bt7");
NexDSButton bt9 = NexDSButton(2, 14, "bt4");
NexDSButton bt10 = NexDSButton(2, 15, "bt5");
NexDSButton bt8 = NexDSButton(2, 39, "bt8");
NexText t1 = NexText(1, 2, "t0");
NexText t2 = NexText(1, 3, "t1");
NexText t3 = NexText(1, 4, "t2");
NexText t4 = NexText(1, 5, "t3");
NexText t8 = NexText(2, 7, "t0");
NexText t9 = NexText(3, 26, "t4");
NexText t10 = NexText(2, 34, "t25");
NexText t11 = NexText(2, 35, "t26");
NexText t12 = NexText(2, 36, "t27");
NexText t13 = NexText(2, 37, "t28");
NexText t14 = NexText(2, 38, "t29");
NexText t15 = NexText(2, 40, "t30");
NexText t16 = NexText(2, 41, "t31");
NexText t17 = NexText(2, 42, "t32");
NexText t18 = NexText(2, 43, "t33");
NexText t19 = NexText(2, 4, "t1");
NexText t20 = NexText(2, 10, "t2");
NexText t21 = NexText(2, 11, "t3");
NexNumber n0 = NexNumber(2, 17, "n0");
NexNumber n1 = NexNumber(2, 18, "n1");
NexNumber n2 = NexNumber(2, 23, "n5");
NexNumber n3 = NexNumber(2, 21, "n2");
NexNumber n4 = NexNumber(2, 30, "n13");
NexNumber n5 = NexNumber(2, 27, "n10");
NexNumber n6 = NexNumber(2, 22, "n8");
NexNumber n7 = NexNumber(2, 26, "n9");
NexNumber n8 = NexNumber(2, 19, "n3");
NexNumber n9 = NexNumber(2, 20, "n4");
NexNumber n10 = NexNumber(2, 24, "n6");
NexNumber n11 = NexNumber(2, 25, "n7");
NexNumber n12 = NexNumber(2, 31, "n14");
NexNumber n13 = NexNumber(2, 32, "n15");
NexNumber n14 = NexNumber(2, 28, "n11");
NexNumber n15 = NexNumber(2, 29, "n12");
NexCheckbox c0 = NexCheckbox(2,6,"c0");
NexCheckbox c1 = NexCheckbox(2,3,"c1"); 
NexCheckbox c2 = NexCheckbox(2,8,"c2");
NexCheckbox c3 = NexCheckbox(2,9,"c3");

NexVariable v0 = NexVariable(0,4,"va0");
uint32_t pageVar=0;


NexTouch *nex_listen_list[] = {
  &v0,&p0,&p1,&p2,&p0_b1,&p0_b2,&p1_b0,&p2_b1,&p1_b2,&t1,&t2,&t3,&t4,&t8,&t19,&t20,&t21,&bt2,&bt3,&bt4,&bt5,&bt6,&bt7,&bt9,&bt10,&bt8,&c0,&c1,&c2,&c3,&n0,&n3,&n6,&n8,&n9,&n10,&n11,&n12,&n13,&n14,&n15, NULL
};

uint32_t next, myInt = 0;
uint32_t dual_state; 
uint32_t dual_state2; 
int s;
uint32_t number = 0;
char temp[10] = {0};

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


uint32_t numberu1;
uint32_t numberu2;
uint32_t numberu3;
uint32_t numberu4;


/////////////////////////////////////////////////////////////////
//////////////////// FUNCTIONS /////////////////////////////////
///////////////////////////////////////////////////////////////

float readEnable(NexText t_calibrate){
      float Text2Number;
      char bufferCalibrateTest[10];

      memset(bufferCalibrateTest, 10, sizeof(bufferCalibrateTest));
      t_calibrate.getText(bufferCalibrateTest, sizeof(bufferCalibrateTest));

      Text2Number = atof(bufferCalibrateTest);
      return Text2Number;
}

void p0_b1_Press(void *ptr) {
  CurrentPage = 1;

  p2.show();
}

void p2_b1_Press(void *ptr) {
  CurrentPage = 0;

  p0.show();
}
void p0_b2_Press(void *ptr) {
  CurrentPage = 2;

  p1.show();
}
void p1_b0_Press(void *ptr) {
  CurrentPage = 0;

  p0.show();
}


char buffer[100] = {0};
char buffer2[100] = {0};
char buffer3[100] = {0};
char buffer4[100] = {0};


float PiraniRead(uint32_t checkbox1,uint32_t checkbox2,uint32_t checkbox3,uint32_t checkbox4,NexText t_gas1,NexText t_gas2,NexText t_gas3,NexText t_gas4,int mcfOutput1,int mcfOutput2,int mcfOutput3,int mcfOutput4){
  float totalFlow=0;
  float correctionFactor=1;
  char bufferText[10];
  char bufferText2[10];
  char bufferText3[10];
  char bufferText4[10];
  float gas_factor_1 = 1;
  float gas_factor_2 = 1;
  float gas_factor_3 = 1;
  float gas_factor_4 = 1;

  Pirani_V = (sinalPirani/SinalMCU)*((SinalMCU*(analogRead(Read_Pirani)))/bitsMCU); 
  

  if(checkbox1==0 && checkbox2==0 && checkbox3==0 && checkbox4==0){
      Pirani_Torr = 10000.00*(pow(10,(Pirani_V-6.125)));
      return Pirani_Torr;
  } 
  else{
    if(checkbox1==1){
      
      totalFlow = totalFlow + mcfOutput1;
    
      memset(bufferText, 10, sizeof(bufferText));
      t8.getText(bufferText, sizeof(bufferText));
      gas_factor_1 = atof(bufferText);
    }
    if(checkbox2==1){
      
      totalFlow = totalFlow + mcfOutput2;

      memset(bufferText2, 10, sizeof(bufferText2));
      t19.getText(bufferText2, sizeof(bufferText2));
      gas_factor_2 = atof(bufferText2);
    }
    if(checkbox3==1){
      
      totalFlow = totalFlow + mcfOutput3;
      
      memset(bufferText3, 10, sizeof(bufferText3));
      t20.getText(bufferText3, sizeof(bufferText3));
      gas_factor_3 = atof(bufferText3);
    }
    if(checkbox4==1){
      
      totalFlow = totalFlow + mcfOutput4;

      memset(bufferText4, 10, sizeof(bufferText4));
      t21.getText(bufferText4, sizeof(bufferText4));
      gas_factor_4 = atof(bufferText4);
    }
    
    correctionFactor = checkbox1*(mcfOutput1/totalFlow)*gas_factor_1 + checkbox2*(mcfOutput2/totalFlow)*gas_factor_2 + checkbox3*(mcfOutput3/totalFlow)*gas_factor_3+ checkbox4*(mcfOutput4/totalFlow)*gas_factor_4;
    Pirani_Torr = 10000.00*(pow(10,((Pirani_V*correctionFactor)-6.125)));
    return Pirani_Torr;
  }

  
} 

float MFCRead(int MFC,char* mfcOutputString,NexText t,NexText t_out, float Calibrated_MFC,int mfcSCCM){
  float gas_factor_in;
  float output_MFC_factor;
  char bufferText[10];

  MFC_V = (mfcSCCM*(sinalMKS/SinalMCU)*((SinalMCU*(analogRead(MFC)))/bitsMCU))/SinalMCU; 
  
  memset(bufferText, 10, sizeof(bufferText));
  t_out.getText(bufferText, sizeof(bufferText));

  gas_factor_in = atof(bufferText);
  
  output_MFC_factor = float(MFC_V)*(gas_factor_in/Calibrated_MFC);

  dtostrf(output_MFC_factor, 1, 2, mfcOutputString); 
  //itoa(output_MFC_factor,mfcOutputString, 10);
  t.setText(mfcOutputString);

  return output_MFC_factor;
}



void SendTextT4(float Pirani_V){
  dtostrf(Pirani_V, 4, 3, buff);
  t10.setText(buff);
}

void WriteSD(File myFile,String message){
  if (myFile) {
    Serial.print(String(myFile) + " Writing ...");
    myFile.println(message);
    // close the file:
    myFile.close();
    //Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
}

float ReadCalibrate(NexText t_calibrate){
    float Text_Number;
    char bufferCalibrate[10];

    memset(bufferCalibrate, 10, sizeof(bufferCalibrate));
    t_calibrate.getText(bufferCalibrate, sizeof(bufferCalibrate));

    Text_Number = atof(bufferCalibrate);
    return Text_Number;
}


void MfcPwmON(int PWM_INPUT,int delta,char* buffer,NexText t,NexNumber n){
  uint32_t val;

  n.getValue(&val);
  
  val = map(val, 0, 500, 0, 255);

  
  analogWrite(PWM_INPUT, val);
  
  itoa(delta,buffer, 10);
  t.setText(buffer);
}

void MfcPwmClose(int PWM_INPUT,int delta,char* buffer,NexText t){

  analogWrite(PWM_INPUT, 0);
  
  itoa(delta,buffer, 10);
  t.setText(buffer);
}

void MfcPwmOFF(int PWM_INPUT,NexCheckbox c){
  analogWrite(PWM_INPUT, 0);
  c.setValue(0);
}

void setup() {
  Serial1.begin(115200);
  Serial2.begin(115200);

  //DEBUG SERIAL
  Serial.begin(9600);

  //Serial.println("[INFO] - Debug serial started");

  if (Serial1.available() > 0) {
    //Serial.println("[INFO]- LCD serial started");
  }
  else{
    //Serial.println("[ERROR]- LCD serial NOT started");
  }
  if (Serial2.available() > 0) {
    //Serial.println("[INFO] - ESP32 serial started");
  }
  else{
    //Serial.println("[ERROR]- ESP32 serial NOT started");
  }
  //PINOUT MFC-PWM 
  pinMode(PWM_INPUT1, OUTPUT);
  pinMode(PWM_INPUT2, OUTPUT);
  pinMode(PWM_INPUT3, OUTPUT);
  pinMode(PWM_INPUT4, OUTPUT);

  pinMode(pinCS, OUTPUT);
  
  //CLOSE VALVE      
  pinMode(Close_MFC1, OUTPUT);
  pinMode(Close_MFC2, OUTPUT);
  pinMode(Close_MFC3, OUTPUT);
  pinMode(Close_MFC4, OUTPUT);
 


  digitalWrite(Close_MFC1, HIGH);
  digitalWrite(Close_MFC2, HIGH);
  digitalWrite(Close_MFC3, HIGH);
  digitalWrite(Close_MFC4, HIGH);

  pinMode(ledPin,OUTPUT);

  analogWrite(PWM_INPUT1, 0);
  analogWrite(PWM_INPUT2, 0);
  analogWrite(PWM_INPUT3, 0);
  analogWrite(PWM_INPUT4, 0);

  p1_b0.attachPop(p1_b0_Press, &p1_b0);
  p2_b1.attachPop(p2_b1_Press, &p2_b1);
  p0_b1.attachPop(p0_b1_Press, &p0_b1);
  p0_b2.attachPop(p0_b2_Press, &p0_b2);

  digitalWrite(ledPin, LOW);

  /////////////////////////////////////////////////////////////////
 ///////////////////// SD - CARD  ////////////////////////////////
/////////////////////////////////////////////////////////////////

  // SD Card Initialization
  if (SD.begin())
  {
    //Serial.println("[INFO] - SD card is ready to use.");
    boolean SD_Status= true;
  } else
  {
    //Serial.println("[ERROR] - SD card initialization failed");
    return;
  }

  //Serial.println("[INFO]- Setup is done!");
}



void loop() {

  // put your main code here, to run repeatedly:
  
  
  nexLoop(nex_listen_list); // NEXTION STARTS
  delay(200);
  

  
/*
v0.getValue(&pageVar);
  if (pageVar == 10){
   
    CurrentPage = 0;
    p0.show();
    pageVar=0;
    delay(500);
   
  }
*/

    /////////////////////////////////////////////////////////////////
   //////////////////// PAGINA 1- MAIN PAGE ////////////////////////
  /////////////////////////////////////////////////////////////////

  if(CurrentPage == 1){
    //Serial.println("[INFO] - Current = page 1");
    CurrentPage = 1;
    //DS BUTTON
    bt8.getValue(&dual_state);
    
    v0.getValue(&pageVar);

    if (pageVar == 10){
     
      CurrentPage = 0;
      p0.show();
      pageVar=0;
      delay(500);
        
    }

    //IF BOTAO MFC TURNED ON
    if(dual_state) 
    {
      //Serial.println("[INFO] - MasterKey on");
    
      //GET STATE MFC CHECKBOX
      c0.getValue(&checkbox1);
      c1.getValue(&checkbox2);
      c2.getValue(&checkbox3);
      c3.getValue(&checkbox4);

      //READ MFC AND PRINT ON SCREEN
      char mfcOutputString[10];

      //PRINT PIRANI VALUES 
      Pirani_V = PiraniRead(checkbox1,checkbox2,checkbox3,checkbox4,t8,t19,t20,t21,mcfOutput1,mcfOutput2,mcfOutput3,mcfOutput4);
      SendTextT4(Pirani_V);
      //Serial.println("[INFO] - Pirani Torr " + String(Pirani_V));

      myRTC.updateTime();

      TimeString = String(myRTC.dayofmonth) + "/" + String(myRTC.month)+ "/" +  String(myRTC.year)  + " " + String(myRTC.hours)  + ":" + String(myRTC.minutes) + ":" + String(myRTC.seconds);
      stamp.setDateTime(myRTC.year, myRTC.month, myRTC.dayofmonth, myRTC.hours, myRTC.minutes, myRTC.seconds);
      //Serial.println("[INFO] - Time " + TimeString);
      uint32_t unix = stamp.getUnix();

      uint32_t start_mfc1;
      uint32_t start_mfc1_pulse;
      
      uint32_t start_mfc2;
      uint32_t start_mfc2_pulse;

      uint32_t start_mfc3;
      uint32_t start_mfc3_pulse;

      uint32_t start_mfc4;
      uint32_t start_mfc4_pulse;

      uint32_t timer_general;

      //CREATE SAVE A SD CARD
      Pirani_CSV = SD.open("PIRANI.csv", FILE_WRITE);
      String messagePirani = TimeString + "," + String(PiraniRead(checkbox1,checkbox2,checkbox3,checkbox4,t8,t19,t20,t21,mcfOutput1,mcfOutput2,mcfOutput3,mcfOutput4)); //
      WriteSD(Pirani_CSV,messagePirani);
      

     
      if (ESP_ENABLE==0){
        ESP_TIMER=unix;  
        ESP_ENABLE=1;
        //Serial.println("[INFO] - ESP32 Enable " );
      }

    if(checkbox1 == 1 || checkbox2 == 1 || checkbox3 == 1 || checkbox4 == 1  ){

    float MFC1_value;
    float MFC2_value;
    float MFC3_value;
    float MFC4_value;

      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 1 - MFC 1 /////////////////////////
    /////////////////////////////////////////////////////////////////

    if(checkbox1){
      bt2.getValue(&MFC1_mode_linear);  
      bt6.getValue(&MFC1_mode_pulse);
      //////////////////// LINEAR - MFC 1 /////////////////////////
      if(MFC1_mode_linear==1 && MFC1_mode_pulse==0){

        //OPEN VALVE
        digitalWrite(Close_MFC1, LOW);

        //CREATE SAVE A SD CARD
        MFC_CSV_1 = SD.open("MFC1.csv", FILE_WRITE);

        if(timer_enable == 0){
          n1.getValue(&timerMFC1);
          //Serial.println("[INFO] - MFC1 - Timer set  = " + String(timerMFC1) );
          start_mfc1 = unix;
          timer_enable = 1;
        }
        
        delta = unix - start_mfc1;

        if(timerMFC1 >= delta){
          MfcPwmON(PWM_INPUT1,delta,buffer,t15,n0);
          MFC1_value = MFCRead(MFC,mfcOutputString,t11,t8,Calibrated_MFC_1,mfcSCCM1);
          String message1 = TimeString + "," + String(MFC1_value); //
          WriteSD(MFC_CSV_1,message1);
          //Serial.println("[INFO] - MFC1 - Measure = " + message1 );

        }
        else {
          MfcPwmOFF(PWM_INPUT1,c0);
          digitalWrite(Close_MFC1, HIGH);
          //Serial.println("[INFO] - MFC1 - Turned OFF");
          checkbox1 = 0;

          delta=0;
          delta_pulse= 0;
          timer_enable = 0;
          timer_enable_pulse = 0;
        }

      }
      
      //////////////////// PULSE - MFC 1 /////////////////////////
      if(MFC1_mode_pulse==1 && MFC1_mode_linear==0){
        
        //OPEN VALVE
        digitalWrite(Close_MFC1, LOW);

        //CREATE SAVE A SD CARD
        MFC_CSV_1 = SD.open("MFC1.csv", FILE_WRITE);

        if(timer_enable_pulse == 0){
          //Serial.println("[INFO] - MFC1 - Turned ON");
          n1.getValue(&timerMFC1_pulse);
          //Serial.println("[INFO] - MFC1 - Turned ON");
          n8.getValue(&timerMFC1_pulse_on);
          //Serial.println("[INFO] - MFC1 - Turned ON");
          n9.getValue(&timerMFC1_pulse_off);
          //Serial.println("[INFO] - MFC1 - Turned ON");
          total1=timerMFC1_pulse_on+timerMFC1_pulse_off;

          cycle_on=timerMFC1_pulse_on/float(total1);
          start_mfc1_pulse = unix;
          timer_enable_pulse = 1;
          
        }
        
        delta_pulse = unix - start_mfc1_pulse;
        

        if(timerMFC1_pulse > delta_pulse){

          if( ((delta_pulse%total1)/float(total1)) < cycle_on ){
            //OPEN VALVE
            digitalWrite(Close_MFC1, LOW);
            MfcPwmON(PWM_INPUT1,delta_pulse,buffer,t15,n0);
            MFC1_value = MFCRead(MFC,mfcOutputString,t11,t8,Calibrated_MFC_1,mfcSCCM1);
            String message1 = TimeString + "," + String(MFC1_value); //
            WriteSD(MFC_CSV_1,message1);
          }
          else{
            //CLOSE VALVE
            digitalWrite(Close_MFC1, HIGH);
            MfcPwmClose(PWM_INPUT1,delta_pulse,buffer,t15);
            MFC1_value = MFCRead(MFC,mfcOutputString,t11,t8,Calibrated_MFC_1,mfcSCCM1);
            String message1 = TimeString + "," + String(MFC1_value); //
            WriteSD(MFC_CSV_1,message1);
          }

        }
        
        else {
          MfcPwmOFF(PWM_INPUT1,c0);
          checkbox1 = 0;
          digitalWrite(Close_MFC1, HIGH);
          delta=0;
          delta_pulse= 0;
          timer_enable = 0;
          timer_enable_pulse = 0;
        }
        
      }
    }
    else {
      timer_enable = 0;
      timer_enable_pulse = 0;
      int delta = 0;
      int delta_pulse= 0;
      
      //CLOSE VALVE
      digitalWrite(Close_MFC1, HIGH);
      
      t16.setText("0");
    }


      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 2 - MFC 2 /////////////////////////
    /////////////////////////////////////////////////////////////////

    if(checkbox2){
      bt3.getValue(&MFC2_mode_linear);
      bt7.getValue(&MFC2_mode_pulse);

      //////////////////// LINEAR - MFC 2 /////////////////////////
      if(MFC2_mode_linear==1 && MFC2_mode_pulse==0){

          //OPEN VALVE
          digitalWrite(Close_MFC2, LOW);

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

          if(timerMFC2 >= delta2){
            
            MfcPwmON(PWM_INPUT2,delta2,buffer2,t16,n3); 
            MFC2_value = MFCRead(MFC2,mfcOutputString,t12,t19,Calibrated_MFC_2,mfcSCCM2);
            String message2 = TimeString + "," + String(MFC2_value); 
            WriteSD(MFC_CSV_2,message2); 
          }
          else {
            MfcPwmOFF(PWM_INPUT2,c1);
            digitalWrite(Close_MFC2, HIGH);
            
            checkbox2 = 0;
            timer_enable2 = 0;
            int delta2 = 0;
            timer_enable_pulse2 = 0;
            
            int delta_pulse2= 0;
            
          }  
        } 

      //////////////////// PULSE - MFC 2 /////////////////////////
      if(MFC2_mode_pulse==1 && MFC2_mode_linear==0){
      
        //OPEN VALVE
        digitalWrite(Close_MFC2, LOW);

        //CREATE SAVE A SD CARD
        MFC_CSV_2 = SD.open("MFC2.csv", FILE_WRITE);

        if(timer_enable_pulse2 == 0){
          n2.getValue(&timerMFC2_pulse);
          n10.getValue(&timerMFC2_pulse_on);
          n11.getValue(&timerMFC2_pulse_off);
          total2=timerMFC2_pulse_on+timerMFC2_pulse_off;

          
          cycle2_on=timerMFC2_pulse_on/float(total2);
          
          start_mfc2_pulse = unix;
          timer_enable_pulse2 = 1;
          
        }
        
        delta2_pulse = unix - start_mfc2_pulse;
        

        if(timerMFC2_pulse > delta2_pulse){

          if( ((delta2_pulse%total2)/float(total2)) < cycle2_on ){
            
            //OPEN VALVE
            digitalWrite(Close_MFC2, LOW);
            MfcPwmON(PWM_INPUT2,delta2_pulse,buffer2,t16,n3); 
            MFC2_value = MFCRead(MFC2,mfcOutputString,t12,t19,Calibrated_MFC_2,mfcSCCM2);
            String message2 = TimeString + "," + String(MFC2_value);
            WriteSD(MFC_CSV_2,message2); 
          }
          else{
            
            //CLOSE VALVE
            digitalWrite(Close_MFC2, HIGH);
            MfcPwmClose(PWM_INPUT2,delta2_pulse,buffer2,t16);  
            MFC2_value = MFCRead(MFC2,mfcOutputString,t12,t19,Calibrated_MFC_2,mfcSCCM2);
            String message2 = TimeString + "," + String(MFC2_value); 
            WriteSD(MFC_CSV_2,message2); 
          }

        }
        else {
          digitalWrite(Close_MFC2, HIGH);
          MfcPwmOFF(PWM_INPUT2,c1);
          
          checkbox2 = 0;
          timer_enable2 = 0;
          int delta2 = 0;
          timer_enable_pulse2 = 0;
          
          int delta_pulse2= 0;
        }
      }



    }
    else {
      timer_enable2 = 0;
      int delta2 = 0;
      timer_enable_pulse2 = 0;
      
      int delta_pulse2= 0;
      //CLOSE VALVE
      digitalWrite(Close_MFC2, HIGH);
    }


      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 3 - MFC 3 /////////////////////////
    /////////////////////////////////////////////////////////////////
    if(checkbox3){
      
      bt4.getValue(&MFC3_mode_linear);
      bt9.getValue(&MFC3_mode_pulse);

      //////////////////// LINEAR - MFC 3 /////////////////////////
      if(MFC3_mode_linear==1 && MFC3_mode_pulse==0){
    
        //OPEN VALVE
        digitalWrite(Close_MFC3, LOW);

        //CREATE SAVE A SD CARD
        MFC_CSV_3 = SD.open("MFC3.csv", FILE_WRITE);

        if(timer_enable3 == 0){
          n4.getValue(&timerMFC3);
          start_mfc3 = unix;
          timer_enable3 = 1;
          
        }
        
        // unsigned long currentMillis = millis()/1000;
        delta3 = unix - start_mfc3;
        
        if(timerMFC3 >= delta3){
         
          MfcPwmON(PWM_INPUT3,delta3,buffer3,t17,n6);
          MFC3_value = MFCRead(MFC3,mfcOutputString,t13,t20,Calibrated_MFC_3,mfcSCCM3);
          String message3 = TimeString + "," + String(MFC3_value); //
          WriteSD(MFC_CSV_3,message3); 
        }
        else {
          
          digitalWrite(Close_MFC3, HIGH);
          MfcPwmOFF(PWM_INPUT3,c2);
          checkbox3 = 0;
          timer_enable3 = 0;
          int delta3 = 0;
          timer_enable_pulse3 = 0;
          int delta_pulse3= 0;
        }
      }

        //////////////////// PULSE - MFC 3 /////////////////////////
      if(MFC3_mode_pulse==1 && MFC3_mode_linear==0){
      
        //OPEN VALVE
        digitalWrite(Close_MFC3, LOW);

        //CREATE SAVE A SD CARD
        MFC_CSV_3 = SD.open("MFC3.csv", FILE_WRITE);

        if(timer_enable_pulse3 == 0){
          n4.getValue(&timerMFC3_pulse);
          n12.getValue(&timerMFC3_pulse_on);
          n13.getValue(&timerMFC3_pulse_off);
          total3=timerMFC3_pulse_on+timerMFC3_pulse_off;

          cycle3_on=timerMFC3_pulse_on/float(total3);
          
          start_mfc3_pulse = unix;
          timer_enable_pulse3= 1;
          
        }
        
        delta3_pulse = unix - start_mfc3_pulse;
        

        if(timerMFC3_pulse > delta3_pulse){

          if( ((delta3_pulse%total3)/float(total3)) < cycle3_on ){
           
            //OPEN VALVE
            digitalWrite(Close_MFC3, LOW);
            MfcPwmON(PWM_INPUT3,delta3_pulse,buffer3,t17,n6); 
            MFC3_value = MFCRead(MFC3,mfcOutputString,t13,t20,Calibrated_MFC_3,mfcSCCM3);
            String message3 = TimeString + "," + String(MFC3_value); //
            WriteSD(MFC_CSV_3,message3); 
          }
          else{
        
            //CLOSE VALVE
            digitalWrite(Close_MFC3, HIGH); 
            MfcPwmClose(PWM_INPUT3,delta3_pulse,buffer3,t17);
            MFC3_value = MFCRead(MFC3,mfcOutputString,t13,t20,Calibrated_MFC_3,mfcSCCM3);
            String message3 = TimeString + "," + String(MFC3_value); //
            WriteSD(MFC_CSV_3,message3);  
          }

        }
        else {
          MfcPwmOFF(PWM_INPUT3,c2);
          digitalWrite(Close_MFC3, HIGH);
          
          checkbox3 = 0;
          timer_enable3 = 0;
          int delta3 = 0;
          timer_enable_pulse3 = 0;
          int delta_pulse3= 0;
        }
      }

    }
    else {
      timer_enable3 = 0;
      int delta3 = 0;
      timer_enable_pulse3 = 0;
      int delta_pulse3= 0;
      //CLOSE VALVE
      digitalWrite(Close_MFC3, HIGH);
      
    }
/*
    v0.getValue(&pageVar);


    if (pageVar == 10){
     
      CurrentPage = 0;
      p0.show();
      pageVar=0;
      delay(500);
        
      }
*/
      
      /////////////////////////////////////////////////////////////////
     //////////////////// CHECKBOX 4 - MFC 4 /////////////////////////
    /////////////////////////////////////////////////////////////////
      
    
     if(checkbox4){
        bt5.getValue(&MFC4_mode_linear);
        bt10.getValue(&MFC4_mode_pulse);
       

       //////////////////// LINEAR - MFC 4 /////////////////////////
       if(MFC4_mode_linear==1 && MFC4_mode_pulse==0){
        //OPEN VALVE
          digitalWrite(Close_MFC4, LOW);
        //CREATE SAVE A SD CARD
          MFC_CSV_4 = SD.open("MFC4.csv", FILE_WRITE);
            
        if(timer_enable4 == 0){
            n5.getValue(&timerMFC4);
            start_mfc4 = unix;
            
            timer_enable4 = 1;
          }
          
          // unsigned long currentMillis = millis()/1000;
          delta4 = unix - start_mfc4;
          
          if(timerMFC4 >= delta4){
            digitalWrite(ledPin,HIGH);
            MfcPwmON(PWM_INPUT4,delta4,buffer4,t18,n7);
            MFC4_value = MFCRead(MFC4,mfcOutputString,t14,t21,Calibrated_MFC_4,mfcSCCM4);
            String message4 = TimeString + "," + String(MFC4_value); //
            WriteSD(MFC_CSV_4,message4); 
          }
          else {
            digitalWrite(ledPin,LOW);
            MfcPwmOFF(PWM_INPUT4,c3);
            checkbox4 = 0;
            timer_enable4 = 0;
            delta4 = 0;
            timer_enable_pulse4 = 0;
            start_mfc4_pulse = 0;
          }
       }
      //////////////////// PULSE - MFC 4 /////////////////////////
      if(MFC4_mode_pulse==1 && MFC4_mode_linear==0){
      
        //OPEN VALVE
        digitalWrite(Close_MFC4, LOW);

        //CREATE SAVE A SD CARD
        MFC_CSV_4 = SD.open("MFC4.csv", FILE_WRITE);

        if(timer_enable_pulse4 == 0){
          n5.getValue(&timerMFC4_pulse);
          n14.getValue(&timerMFC4_pulse_on);
          n15.getValue(&timerMFC4_pulse_off);
          total4=timerMFC4_pulse_on+timerMFC4_pulse_off;

          cycle4_on=timerMFC4_pulse_on/float(total4);
          
          start_mfc4_pulse = unix;
          timer_enable_pulse4 = 1;
          
        }
        
        delta4_pulse = unix - start_mfc4_pulse;
        

        if(timerMFC4_pulse > delta4_pulse){

          if( ((delta4_pulse%total4)/float(total4)) < cycle4_on ){
            
            //OPEN VALVE
            digitalWrite(Close_MFC4, LOW);
            MfcPwmON(PWM_INPUT4,delta4_pulse,buffer4,t18,n7);
            MFC4_value = MFCRead(MFC4,mfcOutputString,t14,t21,Calibrated_MFC_4,mfcSCCM4);
            String message4 = TimeString + "," + String(MFC4_value); //
            WriteSD(MFC_CSV_4,message4); 
          }
          else{
            
            //CLOSE VALVE
            digitalWrite(Close_MFC4, HIGH);
            MfcPwmClose(PWM_INPUT4,delta4_pulse,buffer4,t18);
            MFC4_value = MFCRead(MFC4,mfcOutputString,t14,t21,Calibrated_MFC_4,mfcSCCM4);
            String message4 = TimeString + "," + String(MFC4_value); 
            WriteSD(MFC_CSV_4,message4); 
          }

        }
        else {
          MfcPwmOFF(PWM_INPUT4,c3);
       
          checkbox4 = 0;
          timer_enable_pulse4 = 0;
          start_mfc4_pulse = 0;
          digitalWrite(Close_MFC4, HIGH);
          timer_enable4 = 0;
          delta4 = 0;

        }
      }

      }
      else {
        timer_enable4 = 0;
        delta4 = 0;
        timer_enable_pulse4 = 0;
        start_mfc4_pulse = 0;
        //CLOSE VALVE
        digitalWrite(Close_MFC4, HIGH);
      }  

  
    ///// ESP 01 SEND MESSAGE 
    if (unix - ESP_TIMER >= ESP_DELAY){
      
        ESP_MESSAGE = String(PiraniRead(checkbox1,checkbox2,checkbox3,checkbox4,t8,t19,t20,t21,mcfOutput1,mcfOutput2,mcfOutput3,mcfOutput4)) + "," + String(MFC1_value) + "," + String(MFC2_value) + "," + String(MFC3_value) + "," + String(MFC4_value);
        Serial2.println(ESP_MESSAGE);
        ESP_TIMER = unix;
        //delay(300);
     }

    }
  
  } 
  else{
    
    checkbox1 = 0;
    delta=0;
    delta_pulse= 0;
    //CLOSE VALVE
    digitalWrite(Close_MFC1, HIGH);
    checkbox2 = 0;
    delta2=0;
    delta2_pulse= 0;
    digitalWrite(Close_MFC2, HIGH);

    checkbox3 = 0;
    delta3=0;
    delta3_pulse= 0;
    digitalWrite(Close_MFC3, HIGH);

    checkbox4 = 0; 
    delta4=0;
    delta4_pulse= 0;
    digitalWrite(Close_MFC4, HIGH);
  }
  

  }

  if(CurrentPage == 2){

    float tmfc1;
    float tmfc2;
    float tmfc3;
    float tmfc4;

  
    if(ReadCalibrate(t1)>0){
      Serial.println(ReadCalibrate(t1));
      Calibrated_MFC_1 = ReadCalibrate(t1);
      Serial.println(Calibrated_MFC_1);
      Serial.println(Calibrated_MFC_1);

    }
    if(ReadCalibrate(t2)>0){
      Calibrated_MFC_2 = ReadCalibrate(t2);
    }
    if(ReadCalibrate(t3)>0){
      Calibrated_MFC_3 = ReadCalibrate(t3);
    }
    if(ReadCalibrate(t4)>0){
      Calibrated_MFC_4 = ReadCalibrate(t4);
    }

    

    v0.getValue(&pageVar);

    if (pageVar == 10){
     
      CurrentPage = 0;
      p0.show();
      pageVar=0;
      delay(500);
        
    }

    delay(200);
  }
}



