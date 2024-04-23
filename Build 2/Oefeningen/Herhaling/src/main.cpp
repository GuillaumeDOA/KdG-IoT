#include <Arduino.h>

#define POT A0

void setup()
{
  pinMode(POT, INPUT);
  Serial.begin(9600);
}

void loop()
{
  Serial.println(analogRead(POT));
}