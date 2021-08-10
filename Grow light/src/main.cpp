#include <Arduino.h>
const int relay = 0;
void setup() {
  pinMode(relay,OUTPUT);
  digitalWrite(relay, HIGH);
}

void loop() {
  digitalWrite(relay,LOW);
  delay(1000);
  digitalWrite(relay,HIGH);
  delay(4000);
}
