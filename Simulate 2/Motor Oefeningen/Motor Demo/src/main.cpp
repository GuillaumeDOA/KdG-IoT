#include <Arduino.h>


int pwmPin = 3;

void setup()
{
  pinMode(pwmPin, OUTPUT);
}

void loop()
{

  // set work duty for the motor
  digitalWrite(pwmPin, HIGH);

  delay(2000);

  // set work duty for the motor to 0 (off)
  digitalWrite(pwmPin, LOW);

  delay(2000);
}