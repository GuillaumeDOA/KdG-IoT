#include <Arduino.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT22
int DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);
float Temp;
float Humidity;
long delayTime;

long startMillis;
long currentMillis;
const long period = 1000;
int seconds = 0;

void setup()
{
  Serial.begin(9660);
  startMillis = millis();
  Serial.begin(9600);
  pinMode(DHTPin, INPUT);
  dht.begin();
}

void loop()
{
  currentMillis = millis();

  Humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  Temp = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (currentMillis - startMillis >= period)
  {
    startMillis += period;

    if (isnan(Humidity) || isnan(Temp))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.print("Temperatuur: ");
    Serial.println(Temp);
    Serial.print("Humidity: ");
    Serial.println(Humidity);
  }
}