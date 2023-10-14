#include <Arduino.h>

#define POT A0

void setup()
{
  Serial.begin(9600);
  pinMode(POT, INPUT);
}

void loop()
{
  Serial.println(analogRead(POT));
}