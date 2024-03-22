#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <ezButton.h>

#define RX_PIN 16
#define TX_PIN 17
int leds[] = {0, 14, 13, 2};
ezButton button(12);

// Serial1 on most ESP32 boards
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

const double ANT_LAT = 51.217007817428936;
const double ANT_LON = 4.421252725653194;

double WAYPOINT_LAT, WAYPOINT_LNG;

void displayInfo()
{
  for (int led : leds)
  {
    digitalWrite(led, LOW);
  }

  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    double lat = gps.location.lat();
    double lng = gps.location.lng();
    Serial.printf("%f, %f", lat, lng);
/* 
    double distance = TinyGPSPlus::distanceBetween(ANT_LAT, ANT_LON, lat, lng);
    Serial.printf("  Distance to Antwerpen Centraal: %.2f km (%f m)", distance / 1000, distance); */

    double waypointDistance = TinyGPSPlus::distanceBetween(WAYPOINT_LAT, WAYPOINT_LNG, lat, lng);
    int number = 0;
    if (waypointDistance == 0)
      number = 4;
    else if (waypointDistance < 5)
      number = 3;
    else if (waypointDistance < 10)
      number = 2;
    else if (waypointDistance < 15)
      number = 1;
    else
      number = 0;

    Serial.printf("  Distance to WayPoint: %.2f m  leds: %d", waypointDistance, number);
    Serial.printf("  Waypoint: %.6f, %.6f", WAYPOINT_LAT, WAYPOINT_LNG);

    for (int i = 0; i < number; i++)
    {
      digitalWrite(leds[i], HIGH);
    }
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  if (gps.speed.isValid())
  {
    Serial.printf("  Huidige Snelheid: %.2f km/u (%.2f m/s)", gps.speed.kmph(), gps.speed.mps());
  }

  /*   Serial.print(F("  Date/Time: "));
    if (gps.date.isValid()) {
      Serial.print(gps.date.month());
      Serial.print(F("/"));
      Serial.print(gps.date.day());
      Serial.print(F("/"));
      Serial.print(gps.date.year());
    } else {
      Serial.print(F("INVALID"));
    }
   */
  /*   Serial.print(F(" "));
    if (gps.time.isValid()) {
      if (gps.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gps.time.hour());
      Serial.print(F(":"));
      if (gps.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gps.time.minute());
      Serial.print(F(":"));
      if (gps.time.second() < 10) Serial.print(F("0"));
      Serial.print(gps.time.second());
      Serial.print(F("."));
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      Serial.print(gps.time.centisecond());
    } else {
      Serial.print(F("INVALID"));
    } */

  Serial.println();
}

void setup()
{
  Serial.begin(115200);

  // Initialise gpsSerial
  gpsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  for (int i : leds)
  {
    pinMode(i, OUTPUT);
  }

  for (int i : leds)
  {
    digitalWrite(i, HIGH);
    delay(500);
  }
  for (int i : leds)
  {
    digitalWrite(i, LOW);
  }
}

void loop()
{
  button.loop();
  // while data is ready to be read
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      if (gps.location.isUpdated())
      {
        displayInfo();
      }
    }
  }
  if (button.isPressed())
  {
    Serial.println("Button pressed");
    WAYPOINT_LAT = gps.location.lat();
    WAYPOINT_LNG = gps.location.lng();
  }
}