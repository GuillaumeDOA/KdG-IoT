#include <Arduino.h>

void setup()
{
  pinMode(8, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  tone(8, 440, 500);
  delay(500);
}
