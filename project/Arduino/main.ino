/*
*  Import libs with functions of each arduino module 
*  From folder ./lib import each lib
*/

#include "./lib/controler.h"

/*
* Function to get the values from Settings (page 2) 
* @param t_calibrate MFC SCCM value
* @return new float MFC SCCM calibration 
*/
float ReadCalibrate(NexText t_calibrate){

    float Text_Number;
    char bufferCalibrate[10];
    
    // memset is a memory allocation, change it if you need more bytes of memory
    memset(bufferCalibrate, 10, sizeof(bufferCalibrate));
    t_calibrate.getText(bufferCalibrate, sizeof(bufferCalibrate));  // get value from the LCD

    Text_Number = atof(bufferCalibrate); // convert Str->Float
    return Text_Number;
}


void setup() {
  
  Serial1.begin(115200); // Serial to control Display
  Serial2.begin(115200); // Serial to send data to ESP32

  /*
  * @SERIAL DEBUGGING
  * Enable serial debugging when you need to easily find errors in firmware
  * 1. Use 'Find&Replace' tool to uncomment/comment all debugging message
  * 1.1 Find    ---> //Serial.
  * 1.2 Replace ---> Serial.
  * 2. Upload to the hardware
  * 3. Open the serial monitor with USB connected
  * !!You must use debug serial only for debbunging purposes! It can affect the code's performance!!
  */
 
  // SERIAL DEBUGGING
  Serial.begin(9600);

  //Serial.println("[INFO] - Debug serial started");
  if (Serial1.available() > 0) {
    //Serial.println("[INFO]- Display serial started");
  }
  else{
    //Serial.println("[ERROR]- Display serial NOT started");
  }
  if (Serial2.available() > 0) {
    //Serial.println("[INFO] - ESP32 serial started");
  }
  else{
    //Serial.println("[ERROR]- ESP32 serial NOT started");
  }

  // Starts PWM pin - MFC valve control 
  pinMode(PWM_INPUT1, OUTPUT);
  pinMode(PWM_INPUT2, OUTPUT);
  pinMode(PWM_INPUT3, OUTPUT);
  pinMode(PWM_INPUT4, OUTPUT);

  // Set all PWM's pin to 'zero'
  analogWrite(PWM_INPUT1, 0);
  analogWrite(PWM_INPUT2, 0);
  analogWrite(PWM_INPUT3, 0);
  analogWrite(PWM_INPUT4, 0);

  // Starts transistor's pin - MFC close valve control     
  pinMode(Close_MFC1, OUTPUT);
  pinMode(Close_MFC2, OUTPUT);
  pinMode(Close_MFC3, OUTPUT);
  pinMode(Close_MFC4, OUTPUT);

  // Set all transistor's pin to HIGH = it closes MFC valve  
  digitalWrite(Close_MFC1, HIGH);
  digitalWrite(Close_MFC2, HIGH);
  digitalWrite(Close_MFC3, HIGH);
  digitalWrite(Close_MFC4, HIGH);


  pinMode(pinCS, OUTPUT); // Starts SDcard module pin
  
  pinMode(ledPin,OUTPUT); // Inbuilt LED to debugging 
  digitalWrite(ledPin, LOW);
  
  // Press event for LCD button 
  // p --> page
  // b --> button
  p1_b0.attachPop(p1_b0_Press, &p1_b0);
  p2_b1.attachPop(p2_b1_Press, &p2_b1); 
  p0_b1.attachPop(p0_b1_Press, &p0_b1); 
  p0_b2.attachPop(p0_b2_Press, &p0_b2); 

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

  nexLoop(nex_listen_list); // Starts all nextion LCD configuration
  delay(80); // LCD serial needs a delay to read all serial messages

  if(CurrentPage == 1){

    //Serial.println("[INFO] - Current = page 1");

    // Checks if the current page is the page 0 (main menu)
    v0.getValue(&pageVar);
    if (pageVar == 10){
      CurrentPage = 0;
      p0.show(); // change to page 0 on LCD
      pageVar=0; // trigger backs to zero
      delay(500);
    }
    
    /*
    *  Master-key --> bt8
    *  ON == 1 
    *  If it's on, the system not allow to back to main menu (page 0) 
    *  Modules start working 
    *  
    *  OFF == 0
    *  If it's off, nothing works :)
    */

    bt8.getValue(&dual_state);  // Master-key status

    if(dual_state) // Master-key ON
    {
      Pirani_V = PiraniRead(checkbox1,checkbox2,checkbox3,checkbox4,t8,t19,t20,t21,mcfOutput1,mcfOutput2,mcfOutput3,mcfOutput4);
      SendTextT4(Pirani_V); // Print Pirani value on LCD

      // Read current time from DS1307 module
      myRTC.updateTime();
      TimeString = String(myRTC.dayofmonth) + "/" + String(myRTC.month)+ "/" +  String(myRTC.year)  + " " + String(myRTC.hours)  + ":" + String(myRTC.minutes) + ":" + String(myRTC.seconds);
      stamp.setDateTime(myRTC.year, myRTC.month, myRTC.dayofmonth, myRTC.hours, myRTC.minutes, myRTC.seconds);
      
      // Convert time in unix timestamp
      uint32_t unix = stamp.getUnix();

      //Serial.println("[INFO] - MasterKey on");
      //Serial.println("[INFO] - Pirani Torr " + String(Pirani_V));
      //Serial.println("[INFO] - Time " + TimeString);

      uint32_t start_mfc1;
      uint32_t start_mfc1_pulse;
      uint32_t start_mfc2;
      uint32_t start_mfc2_pulse;
      uint32_t start_mfc3;
      uint32_t start_mfc3_pulse;
      uint32_t start_mfc4;
      uint32_t start_mfc4_pulse;

      uint32_t timer_general;

      // Read Pirani and save into a CSV file on SDcard
      Pirani_CSV = SD.open("PIRANI.csv", FILE_WRITE);
      String messagePirani = TimeString + "," + String(PiraniRead(checkbox1,checkbox2,checkbox3,checkbox4,t8,t19,t20,t21,mcfOutput1,mcfOutput2,mcfOutput3,mcfOutput4)); //
      WriteSD(Pirani_CSV,messagePirani);
      
      //if ESP01 enabled on variables.h file, it gets ESP01 time start reference
      if (ESP_ENABLE==0){
        ESP_TIMER=unix;  
        ESP_ENABLE=1;
        //Serial.println("[INFO] - ESP32 Enable " );
      }

      // MFC's check-box -- ON == 1 / OFF == 0
      c0.getValue(&checkbox1);
      c1.getValue(&checkbox2);
      c2.getValue(&checkbox3);
      c3.getValue(&checkbox4);

      if(checkbox1 == 1 || checkbox2 == 1 || checkbox3 == 1 || checkbox4 == 1  ){

      float MFC1_value;
      float MFC2_value;
      float MFC3_value;
      float MFC4_value;

      // MFC 1
      if(checkbox1){
        
        bt2.getValue(&MFC1_mode_linear); // Linear Function ON/OFF
        bt6.getValue(&MFC1_mode_pulse); // Pulse Function ON/OFF
        
        /*
        * MFC - Linear
        * This 'If' starts the Linear function that controls the valve with constant value for a period of time.
        * During the ON period, it saves all measure into a CSV file in micro SDcard 
        */
        if(MFC1_mode_linear==1 && MFC1_mode_pulse==0){
          digitalWrite(Close_MFC1, LOW); // Open valve

          MFC_CSV_1 = SD.open("MFC1.csv", FILE_WRITE); //Create a CSV file

          if(timer_enable == 0){
            n1.getValue(&timerMFC1); // Get time to be Open&Running
            start_mfc1 = unix; // Get time start reference
            timer_enable = 1; 
            //Serial.println("[INFO] - MFC1 - Timer set  = " + String(timerMFC1) );
          }
          
          delta = unix - start_mfc1; 

          if(timerMFC1 >= delta){
            MfcPwmON(PWM_INPUT1,delta,t15,n0,mfcSCCM1);
            MFC1_value = MFCRead(MFC,t11,t8,Calibrated_MFC_1,mfcSCCM1);
            String message1 = TimeString + "," + String(MFC1_value); 
            WriteSD(MFC_CSV_1,message1);
            //Serial.println("[INFO] - MFC1 - Measure = " + message1 );
          }
          else {
            MfcPwmClose(PWM_INPUT1,delta_pulse,t15,Close_MFC1);
            digitalWrite(Close_MFC1, HIGH);
            checkbox1 = 0;
            //Serial.println("[INFO] - MFC1 - Turned OFF");
          }

        }
        
        /*
        * MFC - Pulse
        * This 'If' starts the Pulse function that controls the valve switching from ON and OFF.
        * During the ON/OFF period, it saves all measure into a CSV file in micro SDcard 
        */
        if(MFC1_mode_pulse==1 && MFC1_mode_linear==0){
          
          //OPEN VALVE
          digitalWrite(Close_MFC1, LOW);

          //CREATE SAVE A SD CARD
          MFC_CSV_1 = SD.open("MFC1.csv", FILE_WRITE);

          if(timer_enable_pulse == 0){
            n1.getValue(&timerMFC1_pulse);
            n8.getValue(&timerMFC1_pulse_on); // Get time to open pulse
            n9.getValue(&timerMFC1_pulse_off); // Get time to close pulse
            total1=timerMFC1_pulse_on+timerMFC1_pulse_off; 
            cycle_on=timerMFC1_pulse_on/float(total1);
            start_mfc1_pulse = unix;
            timer_enable_pulse = 1;
            //Serial.println("[INFO] - MFC1 - Turned ON");
          }
          
          delta_pulse = unix - start_mfc1_pulse;
          

          if(timerMFC1_pulse > delta_pulse){
            
            
            if( ((delta_pulse%total1)/float(total1)) < cycle_on ){ // open pulse
              //OPEN VALVE
              digitalWrite(Close_MFC1, LOW);
              MfcPwmON(PWM_INPUT1,delta_pulse,t15,n0,mfcSCCM1);
              MFC1_value = MFCRead(MFC,t11,t8,Calibrated_MFC_1,mfcSCCM1);
              String message1 = TimeString + "," + String(MFC1_value); //
              WriteSD(MFC_CSV_1,message1);
            }
            else{ // close pulse
              //CLOSE VALVE
              digitalWrite(Close_MFC1, HIGH);
              MfcPwmClose(PWM_INPUT1,delta_pulse,t15,Close_MFC1);
              MFC1_value = MFCRead(MFC,t11,t8,Calibrated_MFC_1,mfcSCCM1);
              String message1 = TimeString + "," + String(MFC1_value); //
              WriteSD(MFC_CSV_1,message1);
            }

          }
          
          else {
            MfcPwmClose(PWM_INPUT1,delta_pulse,t15,Close_MFC1);
            checkbox1 = 0;
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
        delta=0;
        delta_pulse= 0;
        int delta = 0;
        int delta_pulse= 0;
        MfcPwmClose(PWM_INPUT1,delta_pulse,t15,Close_MFC1);
        t16.setText("0");
      }


      
     // CHECKBOX 2 - MFC 2 

    if(checkbox2){
      bt3.getValue(&MFC2_mode_linear);
      bt7.getValue(&MFC2_mode_pulse);

      // LINEAR - MFC 2 
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
            
            MfcPwmON(PWM_INPUT2,delta2,t16,n3,mfcSCCM2); 
            MFC2_value = MFCRead(MFC2,t12,t19,Calibrated_MFC_2,mfcSCCM2);
            String message2 = TimeString + "," + String(MFC2_value); 
            WriteSD(MFC_CSV_2,message2); 
          }
          else {
            MfcPwmClose(PWM_INPUT2,delta2_pulse,t16,Close_MFC2); 
            
            checkbox2 = 0;
            timer_enable2 = 0;
            int delta2 = 0;
            timer_enable_pulse2 = 0;
            
            int delta_pulse2= 0;
            
          }  
        } 

      // PULSE - MFC 2
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
            MfcPwmON(PWM_INPUT2,delta2_pulse,t16,n3,mfcSCCM2); 
            MFC2_value = MFCRead(MFC2,t12,t19,Calibrated_MFC_2,mfcSCCM2);
            String message2 = TimeString + "," + String(MFC2_value);
            WriteSD(MFC_CSV_2,message2); 
          }
          else{
            
            //CLOSE VALVE
            digitalWrite(Close_MFC2, HIGH);
            MfcPwmClose(PWM_INPUT2,delta2_pulse,t16,Close_MFC2);  
            MFC2_value = MFCRead(MFC2,t12,t19,Calibrated_MFC_2,mfcSCCM2);
            String message2 = TimeString + "," + String(MFC2_value); 
            WriteSD(MFC_CSV_2,message2); 
          }

        }
        else {
          MfcPwmClose(PWM_INPUT2,delta2_pulse,t16,Close_MFC2);
          
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


      
     // CHECKBOX 3 - MFC 3 
    if(checkbox3){
      
      bt4.getValue(&MFC3_mode_linear);
      bt9.getValue(&MFC3_mode_pulse);

      //LINEAR - MFC 3 
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
         
          MfcPwmON(PWM_INPUT3,delta3,t17,n6,mfcSCCM3);
          MFC3_value = MFCRead(MFC3,t13,t20,Calibrated_MFC_3,mfcSCCM3);
          String message3 = TimeString + "," + String(MFC3_value); //
          WriteSD(MFC_CSV_3,message3); 
        }
        else {
          
          MfcPwmClose(PWM_INPUT3,delta3_pulse,t17,Close_MFC3);
          checkbox3 = 0;
          timer_enable3 = 0;
          int delta3 = 0;
          timer_enable_pulse3 = 0;
          int delta_pulse3= 0;
        }
      }

      // PULSE - MFC 3 
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
            MfcPwmON(PWM_INPUT3,delta3_pulse,t17,n6,mfcSCCM3); 
            MFC3_value = MFCRead(MFC3,t13,t20,Calibrated_MFC_3,mfcSCCM3);
            String message3 = TimeString + "," + String(MFC3_value); //
            WriteSD(MFC_CSV_3,message3); 
          }
          else{
        
            //CLOSE VALVE
            digitalWrite(Close_MFC3, HIGH); 
            MfcPwmClose(PWM_INPUT3,delta3_pulse,t17,Close_MFC3);
            MFC3_value = MFCRead(MFC3,t13,t20,Calibrated_MFC_3,mfcSCCM3);
            String message3 = TimeString + "," + String(MFC3_value); //
            WriteSD(MFC_CSV_3,message3);  
          }

        }
        else {
          MfcPwmClose(PWM_INPUT3,delta3_pulse,t17,Close_MFC3);
          
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

      
    // CHECKBOX 4 - MFC 4
    
     if(checkbox4){
        bt5.getValue(&MFC4_mode_linear);
        bt10.getValue(&MFC4_mode_pulse);
       

       // LINEAR - MFC 4
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
            MfcPwmON(PWM_INPUT4,delta4,t18,n7,mfcSCCM4);
            MFC4_value = MFCRead(MFC4,t14,t21,Calibrated_MFC_4,mfcSCCM4);
            String message4 = TimeString + "," + String(MFC4_value); //
            WriteSD(MFC_CSV_4,message4); 
          }
          else {
            digitalWrite(ledPin,LOW);
            MfcPwmClose(PWM_INPUT4,delta4_pulse,t18,Close_MFC4);
            checkbox4 = 0;
            timer_enable4 = 0;
            delta4 = 0;
            timer_enable_pulse4 = 0;
            start_mfc4_pulse = 0;
          }
       }
      //PULSE - MFC 4 
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
            MfcPwmON(PWM_INPUT4,delta4_pulse,t18,n7,mfcSCCM4);
            MFC4_value = MFCRead(MFC4,t14,t21,Calibrated_MFC_4,mfcSCCM4);
            String message4 = TimeString + "," + String(MFC4_value); //
            WriteSD(MFC_CSV_4,message4); 
          }
          else{
            
            //CLOSE VALVE
            digitalWrite(Close_MFC4, HIGH);
            MfcPwmClose(PWM_INPUT4,delta4_pulse,t18,Close_MFC4);
            MFC4_value = MFCRead(MFC4,t14,t21,Calibrated_MFC_4,mfcSCCM4);
            String message4 = TimeString + "," + String(MFC4_value); 
            WriteSD(MFC_CSV_4,message4); 
          }

        }
        else {
          MfcPwmClose(PWM_INPUT4,delta4_pulse,t18,Close_MFC4);
       
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

  
    // ESP 01 SEND MESSAGE 
    if (unix - ESP_TIMER >= ESP_DELAY){
        ESP_MESSAGE = String(PiraniRead(checkbox1,checkbox2,checkbox3,checkbox4,t8,t19,t20,t21,mcfOutput1,mcfOutput2,mcfOutput3,mcfOutput4)) + "," + String(MFC1_value) + "," + String(MFC2_value) + "," + String(MFC3_value) + "," + String(MFC4_value);
        Serial2.println(ESP_MESSAGE);
        ESP_TIMER = unix; // ESP time reference
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
    uint32_t applyButton_page2;

    bt11.getValue(&applyButton_page2);
    

    if(applyButton_page2==1){ //save configuration if press button "Apply"

      Calibrated_MFC_1 = ReadCalibrate(t1);
      n16.getValue(&mfcSCCM1);
      delay(60);

      Calibrated_MFC_2 = ReadCalibrate(t2);
      n17.getValue(&mfcSCCM2);
      delay(60);

      Calibrated_MFC_3 = ReadCalibrate(t3);
      n18.getValue(&mfcSCCM3);
      delay(60);

      Calibrated_MFC_4 = ReadCalibrate(t4);
      n19.getValue(&mfcSCCM4);
      delay(60);
      
      bt11.setValue(0);
    }

    v0.getValue(&pageVar);

    if (pageVar == 10){
      CurrentPage = 0;
      p0.show();
      pageVar=0;
      delay(500);
    }

  }
}



