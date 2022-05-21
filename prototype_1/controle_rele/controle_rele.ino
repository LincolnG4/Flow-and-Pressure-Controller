
int relayPin2 = 23;  //Pino Arduino Rele #2

int analogPin = A8;
int val = 0;


void setup() {
  Serial.begin(9600);
  pinMode(relayPin2, OUTPUT);

}

void loop() {
  digitalWrite(relayPin2, LOW);    //Switch Relay #1 ON
  delay(5000);                //Wait 1 Second
  digitalWrite(relayPin2, HIGH);     //Switch Relay #1 OFF
  delay(5000);                      //Wait 1 Second
  

}
