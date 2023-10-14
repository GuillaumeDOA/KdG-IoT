#include <Arduino.h>

#define GREEN 3
#define YELLOW 4
#define RED 5

void setup() {
pinMode(GREEN, OUTPUT);
pinMode(YELLOW, OUTPUT);
pinMode(RED, OUTPUT);  
}

void loop() {
  digitalWrite(GREEN, HIGH);
  delay(5000);
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW,HIGH);
  delay(1000);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);
  delay(5000);
  digitalWrite(RED,LOW);
}