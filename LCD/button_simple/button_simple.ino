
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>


// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP=7,XM=A1,YP=A2,YM=6; //240x320 ID=0x9341
const int TS_MAXX=183,TS_MINX=920,TS_MAXY=192,TS_MINY=914;

int relayPin1 = 22;
int relayPin2 = 26;


int Read_Pirani = A12;
int Read_MKS1 = A15;

float Pirani_V = 0;
float MKS1_V = 0;

//float aPirani_V = 0;
float Pirani_Torr = 0.000;
float Pot = 0.00;
//float media = 0.00;


const int UP = 0;
const int DOWN = 1;
const int UD_PIN = 28;            // Goes to X9C103P U/D pin - Up = HIGH, Down = LOW
const int INC_PIN = 29;           // Goes to X9C103P INC pin - active falling edge
const int CS_PIN = 30;           // Goes to X9C103P CS pin - active LOW




#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define BOXSIZE 40
#define PENRADIUS 3

#define MINPRESSURE 10
#define MAXPRESSURE 1000



bool change_mks1 = 0;
bool touch_mks1 = 0;

bool change_pirani = 0;
bool touch_pirani = 0;

bool change_plus1 = 0;
bool touch_plus1 = 0;

void setup() {
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  
  Serial.begin(9600);
  tft.reset();
  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  if (ID == 0xD3D3) ID = 0x9486; // write-only shield
  tft.begin(ID);

  
  tft.setRotation(0);            //PORTRAIT
  tft.setTextColor(BLACK);
  tft.fillScreen(BLACK);

  
  tft.fillRect(130, 90, 40, 30, RED);
  tft.setCursor(140,95);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("+");
  tft.fillRect(190, 90, 40, 30, RED);
  tft.setCursor(200,95);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("-");
  
  
  tft.setCursor(130,40);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  MKS1_V = 5*analogRead(Read_MKS1)/1023.00;
  tft.print(String(MKS1_V+0.10));
  tft.setCursor(220,40);
  tft.print("V");  

  pinMode (CS_PIN, OUTPUT);
  pinMode (UD_PIN, OUTPUT);
  pinMode (INC_PIN, OUTPUT);

  digitalWrite(INC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);                  // Enable the X9C103P chip


}





void loop() {
  if (change_mks1 == 0 && touch_mks1 == 0) {
    digitalWrite(relayPin1, LOW);
    tft.fillCircle(70, 70, 50, RED);
    tft.setCursor(40,60);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.println("MKS1");
    change_mks1 = 1;
    //delay(200);
  }

  if (change_mks1 == 0 && touch_mks1 == 1) {
    digitalWrite(relayPin1, HIGH);
    tft.fillCircle(70, 70, 50, GREEN);
    tft.setCursor(40,60);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.println("MKS1");
    change_mks1 = 1;
    //delay(200);
  }

  if (change_pirani == 0 && touch_pirani == 0) {
    digitalWrite(relayPin2, LOW);
    tft.fillCircle(70, 250, 50, RED);
    tft.setCursor(40,240);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.println("PIRANI");
    change_pirani = 1;
    //delay(200);
  }

  if (change_pirani == 0 && touch_pirani == 1) {
    digitalWrite(relayPin2, HIGH);
    tft.fillCircle(70, 250, 50, GREEN);
    tft.setCursor(40,240);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.println("PIRANI");
    change_pirani = 1;
    
  }

  
   if (touch_pirani == 1) {
      tft.setCursor(130,240);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      Pirani_V = (5*(analogRead(Read_Pirani)))/1023.00; 
      //aPirani_V = aPirani_V + Pirani_V ;
      Pirani_Torr = 10000.00*(pow(10,(Pirani_V-6.125)));
      //i = i + 1 ;
      //media = aPirani_V / i;
      //Serial.println(Torr); 
      
      tft.fillRect(125, 230, 60, 30, BLACK);
      tft.print(String(Pirani_Torr));
      tft.setCursor(200,240);
      tft.print("E+4");
      tft.setCursor(130,280);
      tft.print("Torr");
      delay(50);


  }

  
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    //Serial.print("("); Serial.print(p.x);
    //Serial.print(", "); Serial.print(p.y);
    //Serial.println(")");

    if (p.x > 30 && p.x < 110) {
      if (p.y > 30 && p.y < 110) {
        change_mks1 = 0;
        touch_mks1 = !touch_mks1;
        delay(150);
      }
    }

    
    if (p.x > 120 && p.x < 165) {
      if (p.y > 70 && p.y < 105) {
        
        tft.setCursor(130,40);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);
        
        Move_Wiper(UP);
        
        delay(200);
        MKS1_V = 5*((analogRead(Read_MKS1))/1023.00);  

        
        tft.fillRect(125,35, 80, 30, BLACK);
        tft.print(String(MKS1_V));
        tft.setCursor(220,40);
        tft.print("V");        
        MKS1_V = 0;
      }
    }
    
    if (p.x > 180 && p.x < 225) {
      if (p.y > 70 && p.y < 105) {


        
        tft.setCursor(130,40);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);

        Move_Wiper(DOWN);
        delay(200); 
        MKS1_V = 5*analogRead(Read_MKS1)/1023.00;

        tft.fillRect(125,35, 80, 30, BLACK);
        tft.print(String(MKS1_V));
        tft.setCursor(220,40);
        tft.print("V");
        
      }
    }


    
    if (p.x > 30 && p.x < 110) {
      if (p.y > 210 && p.y < 290) {
        change_pirani = 0;
        touch_pirani = !touch_pirani;
        delay(150);
      }
    }

  
  }
  delay(1000);
  MKS1_V = 5*analogRead(Read_MKS1)/1023.00;
  Serial.println(MKS1_V);
}

void Move_Wiper(int direction)
{
  switch (direction) {
    case UP:
      digitalWrite(UD_PIN, HIGH); delayMicroseconds(5);  // Set to increment
      digitalWrite(INC_PIN, LOW); delayMicroseconds(5);  // Pulse INC pin low
      digitalWrite(INC_PIN, HIGH);
      break;
    case DOWN:
      digitalWrite(UD_PIN, LOW); delayMicroseconds(5);   // Set to decrement
      digitalWrite(INC_PIN, LOW); delayMicroseconds(5);  // Pulse INC pin low
      digitalWrite(INC_PIN, HIGH);
      break;
    default:
      break;
  }

}
