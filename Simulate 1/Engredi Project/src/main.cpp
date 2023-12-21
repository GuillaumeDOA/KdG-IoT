#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PM25AQI.h>
#include <WiFiS3.h>
#include <secrets.h>

#define POWERLED 3
#define WIFILED 4
#define DATALED 5

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

long startTime;
long delayTime = 2000;

int status = WL_IDLE_STATUS;

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

  // you're connected now, so print out the data:
  digitalWrite(WIFILED, HIGH);
  Serial.println("You're connected to the network");
}

void SensorSetup()
{
  Serial.println("Adafruit PMSA003I Air Quality Sensor");

  // Wait one second for sensor to boot up!
  delay(2000);

  if (!aqi.begin_I2C())
  { // connect to the sensor over I2C
    Serial.println("Could not find PM 2.5 sensor!");
    while (1)
      delay(10);
  }

  Serial.println("PM25 found!");
}

void readAQI()
{

  if (millis() - startTime < delayTime)
    return; // Read Sensor every 2 seconds

  digitalWrite(DATALED, HIGH);
  PM25_AQI_Data data;

  if (!aqi.read(&data))
  {
    Serial.println("Could not read from AQI"); // Try again
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
  startTime = millis();
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

  startTime = millis();
}

void loop()
{
  readAQI();
}
