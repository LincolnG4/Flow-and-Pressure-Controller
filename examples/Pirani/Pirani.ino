
int relayPin2 = 23;  //Pino Arduino Rele #2

int Read_Pirani = A8; //Pino Arduino Pirani #2

float Pirani_V = 0;
double Pirani_Torr = 0.000;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin2, HIGH);    //Switch Relay #1 OFF
  delay(8000);
  digitalWrite(relayPin2, LOW);     //Switch Relay #1 OFF
}

void loop() {
  
  Pirani_V = 2*((5*(analogRead(Read_Pirani)))/1023.0000); 
  Pirani_Torr = 10000.0000*(pow(10,(Pirani_V-6.125)));
  
  Serial.print("  V = ");
  Serial.print(Pirani_V);
  Serial.print("  Torr = ");
  Serial.print(Pirani_Torr,5);
  Serial.println("  E-4");

}
