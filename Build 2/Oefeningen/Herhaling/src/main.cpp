#include <Arduino.h>

#define sensorPin A0
#define RED 5
#define GREEN 3
#define BLUE 6

long startTime;

void setup()
{
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(GREEN, HIGH);

  startTime = millis();
}

float readTemp()
{
  // Get a reading from the temperature sensor:
  int reading = analogRead(sensorPin);

  // Convert the reading into voltage:
  float voltage = reading * (5000 / 1024.0);

  // Convert the voltage into the temperature in degree Celsius:
  float temperature = voltage / 10;

  // Print the temperature in the Serial Monitor:
  Serial.print(temperature);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("C");

  return temperature;
}

void loop()
{
  if (millis() - startTime < 3000)
    return;

  float temp = readTemp();
  if (temp >= 21.0)
  {
    // make led red
  }
  else if (temp < 21.0 && temp >= 18)
  {
    // make led yellow
  }
  else
  {
    // make led green
  }
}