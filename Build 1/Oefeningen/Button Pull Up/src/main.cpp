#include <Arduino.h>

#define ON_BUTTON 3
#define OFF_BUTTON 4
#define LED 2

void setup() {
  pinMode(ON_BUTTON, INPUT_PULLUP);
  pinMode(OFF_BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  if (digitalRead(ON_BUTTON) == LOW) {
    digitalWrite(LED, HIGH);
    Serial.write("Drukknop 1");
  }
  if (digitalRead(OFF_BUTTON) == LOW){
    digitalWrite(LED, LOW);
    Serial.write("Drukknop 2");
  }
}
