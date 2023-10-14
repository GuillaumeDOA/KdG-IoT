#include <Arduino.h>


void setup() {
  for (size_t i = 3; i < 7; i++)
  {
    pinMode(i, OUTPUT);
  };
}

void loop() {
  for (int i = 3; i < 7; i++)
  {
    digitalWrite(i, LOW);
  };
  delay(500);
  for (int i = 3; i < 7; i++)
  {
    digitalWrite(i, HIGH);
    delay(500);
  }
  
}
