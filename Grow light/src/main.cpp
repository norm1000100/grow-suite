#include <Arduino.h>
const int relay = 0;
const int builtin = 2;
const int baud_rate = 9600;
void setup() {
  Serial.begin(baud_rate);
  pinMode(builtin, OUTPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(relay, HIGH);
  
}

void loop() {
  digitalWrite(builtin, HIGH);
  digitalWrite(relay,LOW);
  delay(1000);
  digitalWrite(builtin, HIGH);
  digitalWrite(relay,LOW);
  delay(4000);
}
