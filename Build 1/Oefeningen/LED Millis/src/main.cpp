#include <Arduino.h>

#define LED1 2
#define LED2 4

long led1StartTime;
long led2StartTime;
long currentTimme;

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
}

void loop()
{
  currentTimme = millis();

  if (currentTimme - led1StartTime >= 2000 && digitalRead(LED1))
  {
    digitalWrite(LED1, LOW);
    led1StartTime = millis();
  }

  if (currentTimme - led1StartTime >= 3000 && !digitalRead(LED1))
  {
    digitalWrite(LED1, HIGH);
    led1StartTime = millis();
  }

  if (currentTimme - led2StartTime >= 4000 && digitalRead(LED2))
  {
    digitalWrite(LED2, LOW);
    led2StartTime = millis();
  }

  if (currentTimme - led2StartTime >= 5000 && !digitalRead(LED2))
  {
    digitalWrite(LED2, HIGH);
    led2StartTime = millis();
  }
}