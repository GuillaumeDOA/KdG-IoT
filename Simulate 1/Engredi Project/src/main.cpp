#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PM25AQI.h>
#include <WiFiS3.h>
#include <secrets.h>
#include <RTC.h>

#define POWERLED 3
#define WIFILED 4
#define DATALED 5

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

int status = WL_IDLE_STATUS;

int previousQuarter = -1;

void WifiSetup()
{
  Serial.println("Wifi setup:");
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
    {
      Serial.println("Problem with Wifi module");
      delay(5000);
    }
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  for (int i = 0; i < sizeof(SSIDS); i++)
  {
    //  Get SSID en passwords from secrets.h file
    char *ssid = SSIDS[i];
    char *pass = PASSWORDS[i];

    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);

    if (status == WL_CONNECTED)
      break;
    Serial.println("Connection failed. Trying next SSID");
  }

  // you're connected now
  digitalWrite(WIFILED, HIGH);
  Serial.println("You're connected to the network");
}

void SensorSetup()
{
  Serial.println("Adafruit PMSA003I Air Quality Sensor");

  // Wait one second for sensor to boot up!
  delay(2000);

  while (!aqi.begin_I2C())
  { // connect to the sensor over I2C
    Serial.println("Could not find PM 2.5 sensor!");
    delay(2000);
  }

  Serial.println("PM25 found!");
}

void readAQI()
{
  digitalWrite(DATALED, HIGH);
  PM25_AQI_Data data;

  if (!aqi.read(&data))
  {
    Serial.println("Could not read from AQI"); // Try again
    digitalWrite(DATALED, LOW);
    return;
  }
  Serial.println("AQI reading success");

  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 2.5: "));
  Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10: "));
  Serial.println(data.pm100_standard);
  Serial.println();
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 2.5: "));
  Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: "));
  Serial.println(data.pm100_env);
  Serial.println();
  digitalWrite(DATALED, LOW);
}

void setup()
{
  pinMode(POWERLED, OUTPUT);
  pinMode(WIFILED, OUTPUT);
  pinMode(DATALED, OUTPUT);

  // Powerled high as soon as program starts
  digitalWrite(POWERLED, HIGH);

  // Wait for serial monitor to open
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  SensorSetup();
  WifiSetup();

  // Setup RTC (Real Time Clock)
  RTC.begin();
  RTCTime startTime(22, Month::DECEMBER, 2024, 1, 1, 00, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(startTime);
}

void loop()
{
  RTCTime currentTime;
  RTC.getTime(currentTime);

  if(currentTime.getMinutes() != previousQuarter)
    readAQI();
  previousQuarter = currentTime.getMinutes();
}
