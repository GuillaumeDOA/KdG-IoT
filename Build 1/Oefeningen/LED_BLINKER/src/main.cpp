#include <Arduino.h>

#define LED 3
#define button 4
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  digitalWrite(3,HIGH);
  delay(1000);
  digitalWrite(3,LOW);
  delay(1000);
}