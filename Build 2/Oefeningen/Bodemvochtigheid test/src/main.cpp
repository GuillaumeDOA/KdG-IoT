#include <Arduino.h>

#define LED 3
#define SENSOR A0

long startTime;



void setup() {
  Serial.begin(9600);

  pinMode(SENSOR, INPUT);
  pinMode(LED, OUTPUT);

  startTime = millis();
}

void loop() {
  if(millis() - startTime < 3000)
    return;

  int sensorVal = analogRead(SENSOR);
  int percentage = map(sensorVal, 0, 1023, 0, 100);

  Serial.print("Sensor waarde: ");
  Serial.println(sensorVal);
  Serial.print("Percentage: ");
  Serial.println(percentage);
  Serial.println();
  startTime = millis();
}
