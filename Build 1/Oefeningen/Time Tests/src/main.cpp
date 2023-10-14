#include <Arduino.h>

long startMillis;
long currentMillis;
const long period = 1000;

void setup()
{
  Serial.begin(9660);
  startMillis = millis();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  currentMillis = millis();

  if (currentMillis - startMillis >= period)
  {
    startMillis += period;
    Serial.print("Seconde: ");
    Serial.println(currentMillis / 1000);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
