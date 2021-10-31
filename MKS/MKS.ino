
int relayPin2 = 23;  //Pino Arduino Rele #2

int Read_MKS= A9; //Pino Arduino MKS 
int Read_POT= A8; //Pino Arduino Potenciometro #2

float MKS_V = 0;
float POT_V = 0;

float MKS_sccm = 0;
float POT_sccm = 0;

double MKS = 0.000;

const int UP = 0;
const int DOWN = 1;
const int UD_PIN = 26;           // Goes to X9C103P U/D pin - Up = HIGH, Down = LOW
const int INC_PIN = 27;         // Goes to X9C103P INC pin - active falling edge
const int CS_PIN = 28;         // Goes to X9C103P CS pin - active 

void setup() {
  pinMode (CS_PIN, OUTPUT);
  pinMode (UD_PIN, OUTPUT);
  pinMode (INC_PIN, OUTPUT);
  
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin2, LOW);    //Switch Relay #1 OFF
  
  digitalWrite(INC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);                  // Enable the X9C103P chip  
  
  Serial.begin(9600);

  delay(8000);
  digitalWrite(relayPin2, HIGH);     //Switch Relay #1 OFF
}

void loop() {
  
  DoSerial();
  
  MKS_V = (5*(analogRead(Read_MKS)))/1023.0000; 
  MKS_sccm = 100.00*MKS_V ; 
  POT_V = (5*(analogRead(Read_POT)))/1023.0000; 
  POT_sccm = 100.00*POT_V ; 
  
  Serial.print(" MKS V = ");
  Serial.print(MKS_V);

   
  Serial.print(" MKS sccm = ");
  Serial.print(MKS_sccm);
  
  Serial.print(" POT V = ");
  Serial.print(POT_V);

  Serial.print(" POT sccm = ");
  Serial.println(POT_sccm);

}

void DoSerial()
{
  char ch = toupper(Serial.read());   // Read the character we received
  // and convert to upper case
  switch (ch) {
    case 'S':                         // Save settings
      digitalWrite(INC_PIN, HIGH);
      digitalWrite(CS_PIN, HIGH);
      delay(100);
      digitalWrite(CS_PIN, LOW);
      Serial.println("Setting Saved");
      break;
    case 'U':                         // Increment setting
      Move_Wiper(UP);
      Serial.print("Incrementing Value");

      break;
    case 'D':                         // Decrement setting
      Move_Wiper(DOWN);
      Serial.print("Decrementing Value");

      break;
    default:
      break;
  }
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
