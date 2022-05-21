#include <stdio.h>
#include <string.h>

String incomingByte;
int sender;
char* randNumber;

char *strings[30]; 
char *ptr = NULL;

int Pirani_ESP = 0;
int MFC1_ESP = 0 ;
int MFC2_ESP = 0 ;
int MFC3_ESP = 0 ;
int MFC4_ESP = 0;

char buf[30];

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.

String GAS_ID = "AKfycbymvhmaNGcsCbmoSvHS21fdstKLAxjjKjei01xuxWgsh7kf64kI70l-GkexULhxIHkPRQ"; //--> spreadsheet script ID

void setup() {
  WiFi.mode(WIFI_STA);
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  WiFiManager wm;


  bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    wm.setAPStaticIPConfig(IPAddress(10,0,1,32), IPAddress(10,0,1,32), IPAddress(255,255,255,0));
    res = wm.autoConnect("UTFPR_IOT_10.0.1.32","UTFPRIoTLAB"); // password protected ap
    
    if(!res) {
        Serial.println("Failed to connect");
        digitalWrite(ON_Board_LED, HIGH);
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        digitalWrite(ON_Board_LED, LOW);
        delay(250);
        digitalWrite(ON_Board_LED, HIGH);
        delay(250);
        digitalWrite(ON_Board_LED, LOW);
        delay(250);
        digitalWrite(ON_Board_LED, HIGH);
        delay(1000);
        digitalWrite(ON_Board_LED, LOW);
    }
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
        // read the incoming byte:
        incomingByte = Serial.readString();
        incomingByte.toCharArray(buf,30);
        
        byte index = 0;
         
        ptr = strtok(buf, ",");  // delimiter
        while (ptr != NULL)
         {
            strings[index] = ptr;
            index++;
            ptr = strtok(NULL, ",");
         }
        
        Pirani_ESP = atoi(strings[0]); 
        MFC1_ESP = atoi(strings[1]);
        MFC2_ESP = atoi(strings[2]);
        MFC3_ESP = atoi(strings[3]);
        MFC4_ESP = atoi(strings[4]);

        sendData(Pirani_ESP,MFC1_ESP,MFC2_ESP,MFC3_ESP,MFC4_ESP);
        
 
        digitalWrite(ON_Board_LED, HIGH);
        delay(3000);
        digitalWrite(ON_Board_LED, LOW);
        
        }
}

// Subroutine for sending data to Google Sheets
void sendData(int Pirani_ESP, int MFC1_ESP,int MFC2_ESP,int MFC3_ESP,int MFC4_ESP) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Processing data and sending data
  String string_Pirani_ESP =  String(Pirani_ESP,DEC);
  String string_MFC1_ESP =  String(MFC1_ESP,DEC);
  String string_MFC2_ESP =  String(MFC2_ESP,DEC);
  String string_MFC3_ESP =  String(MFC3_ESP,DEC);
  String string_MFC4_ESP =  String(MFC4_ESP,DEC);
  
  String url = "/macros/s/" + GAS_ID + "/exec?Pirani=" + string_Pirani_ESP + "&MFC1=" + string_MFC1_ESP+ "&MFC2=" + string_MFC2_ESP + "&MFC3=" + string_MFC3_ESP + "&MFC4=" + string_MFC4_ESP;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
} 
