#include <Arduino.h>

#define HALL_SENSOR A0

unsigned long startTime = 0; // Variable to store the last time the sensor was read

void setup()
{
  Serial.begin(115200);
  pinMode(HALL_SENSOR, INPUT);
  startTime = millis(); // Initialize startTime to the current time
}

void loop()
{
  if (millis() - startTime > 100){
    startTime = millis();
    int hallValue = analogRead(HALL_SENSOR);
    Serial.print("Hall Sensor Value: ");
    Serial.println(hallValue);
  }
}
