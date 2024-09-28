#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define RED 7
#define GREEN 5
#define BLUE 6

#define DHTTYPE DHT22
int DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);
float Temp;
float Humidity;

long startMillis;
long currentMillis;
long period = 1000;

void setup()
{
  startMillis = millis();
  Serial.begin(9600);
  pinMode(DHTPin, INPUT);
  dht.begin();

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  analogWrite(RED, 255);
  analogWrite(GREEN, 255);
  analogWrite(BLUE, 255);
}

void loop()
{
  currentMillis = millis();
  Temp = dht.readTemperature();
  if (currentMillis - startMillis >= period)
  {
    Serial.print("Current temp: ");
    Serial.println(Temp);
    startMillis += period;
  }

  if (Temp > 24)
  { // rood
    analogWrite(RED, 0);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
  }
  else if (Temp >= 22 && Temp <= 24)
  { // geel
    analogWrite(RED, 0);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 255);
  }
  else if (Temp < 22)
  { // groen
    analogWrite(RED, 255);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 255);
  }
}