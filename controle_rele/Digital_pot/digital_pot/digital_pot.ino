/*
  Exercise X9C103S Digital Potentiometer
  Uses Serial Monitor window to issue commands for controlling the pot and
  also observing the results.

  The 3 control lines are attached to any digital Pins pins on uC.  We
  are using pins 8,9 and 10.

  Pot connections:  VL = ground, VH = 5V, VW wiper is connected to analog input A0

  Commands
  --------
  U = Increase wiper setting
  D = Decrease wiper setting
  S = Save current setting into memory for recall after power cycle
*/
const float V_REF = 5.0;          // Change if using different Vref
const int UP = 0;
const int DOWN = 1;
const int UD_PIN = 28;            // Goes to X9C103P U/D pin - Up = HIGH, Down = LOW
const int INC_PIN = 29;           // Goes to X9C103P INC pin - active falling edge
const int CS_PIN = 30;           // Goes to X9C103P CS pin - active LOW
const int WIPER_PIN = A8;        // Goes to X9C103P VW pin - Analog voltage output of pot
float voltage = 0;
//===============================================================================
//  Initialization
//===============================================================================
void setup() {
  pinMode (CS_PIN, OUTPUT);
  pinMode (UD_PIN, OUTPUT);
  pinMode (INC_PIN, OUTPUT);
  Serial.begin(9600);

  digitalWrite(INC_PIN, HIGH);
  digitalWrite(CS_PIN, LOW);                  // Enable the X9C103P chip
  Serial.print ("Initial Voltage Setting: ");
  PrintVoltage();                             // Print X9C103P power up value
}
//===============================================================================
//  Main
//===============================================================================
void loop() {
  if (Serial.available()) DoSerial();  // Just loop looking for user input
 
}
//===============================================================================
//  Subroutine to handle characters typed via Serial Monitor Window
//===============================================================================
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
      PrintVoltage();
      break;
    case 'D':                         // Decrement setting
      Move_Wiper(DOWN);
      Serial.print("Decrementing Value");
      PrintVoltage();
      break;
    default:
      break;
  }
}

//===============================================================================
//  Subroutine to read ADC and print to the Serial Monitor Window
//===============================================================================
void PrintVoltage()
{
  int sampleADC = analogRead(WIPER_PIN);      // Take reading on wiper pin
  float volts = (sampleADC * V_REF) / 1023.0; // Convert to voltage
  Serial.print("   ADC = ");
  Serial.print(sampleADC);
  Serial.print("  Voltage = ");
  Serial.println(volts, 3);
}

//===============================================================================
//  Subroutine to move the wiper UP or DOWN
//===============================================================================
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
