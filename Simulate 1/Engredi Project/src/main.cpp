#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PM25AQI.h>
#include <WiFiS3.h>
#include <secrets.h>
#include <RTC.h>

// Define your WiFi credentials and Google Sheets information
const char *ssid = SSID;
const char *password = PASSWORD;
const char *host = "script.google.com";                                                                     // This should be the URL to your Google Script
const String googleScriptId = "AKfycbzs-YDE4Dnn6vaN3Xv3a1AXaMbwJzHwR1fbxS2ITaf6bE8rLDRSKXShrVSUrrTfS6bRtA"; // EIGEN SCRIPT

// Define LED pins
#define POWERLED 4
#define WIFILED 3
#define DATALED 5

// Initialize the PM2.5 sensor and WiFi client
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
PM25_AQI_Data data;
WiFiSSLClient client;

int status = WL_IDLE_STATUS;
int previousMinute = -1;

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
      delay(5000);
    }
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, password);

    // wait 10 seconds for connection:
    delay(10000);

    if (status == WL_CONNECTED)
    {
      Serial.println("Connected to the network");
      digitalWrite(WIFILED, HIGH);
      break;
    }
    else
    {
      Serial.println("Connection failed, retrying...");
    }
  }
}

void SensorSetup()
{
  Serial.println("Adafruit PMSA003I Air Quality Sensor");

  // Wait two seconds for the sensor to boot up
  delay(2000);

  // Attempt to connect to the sensor over I2C
  while (!aqi.begin_I2C())
  {
    Serial.println("Could not find PM 2.5 sensor!");
    delay(2000);
  }

  Serial.println("PM25 found!");
}

bool readAQI()
{
  digitalWrite(DATALED, HIGH);
  if (!aqi.read(&data))
  {
    Serial.println("Could not read from AQI");
    digitalWrite(DATALED, LOW);
    delay(500);
    return false;
  }
  Serial.println("AQI reading success");
  Serial.println(data.pm25_env);
  Serial.println(data.pm100_env);
  Serial.println(data.pm25_standard);
  Serial.println(data.pm100_standard);
  digitalWrite(DATALED, LOW);
  return true;
}

void sendToGoogleSheets()
{
  if (client.connect(host, 443))
  {
    String url = "/macros/s/" + googleScriptId + "/exec?";
    url += "pm2=" + String(data.pm25_env);
    url += "&pm10=" + String(data.pm100_env);
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    // Wait for server response and read it
  }
}

void setup()
{
  pinMode(POWERLED, OUTPUT);
  pinMode(WIFILED, OUTPUT);
  pinMode(DATALED, OUTPUT);
  digitalWrite(POWERLED, HIGH);

  Serial.begin(115200);
  while (!Serial)
    delay(10);

  SensorSetup();
  WifiSetup();

  // Setup RTC (Real Time Clock)
  RTC.begin();
  // Set your desired start time here
  RTCTime startTime(17, Month::JANUARY, 2024, 15, 59, 50, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(startTime);
}

void loop()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(WIFILED, LOW);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, password);

    // wait 10 seconds for connection:
    delay(10000);

    if (status == WL_CONNECTED)
    {
      Serial.println("Connected to the network");
      digitalWrite(WIFILED, HIGH);
      break;
    }
    else
      Serial.println("Connection failed, retrying...");
  }

  RTCTime currentTime;
  RTC.getTime(currentTime);

  if (currentTime.getMinutes() != previousMinute)
  {
    previousMinute = currentTime.getMinutes();
    readAQI();
    if (currentTime.getMinutes() % 15 == 0)
    {
      if (readAQI())
      {
        sendToGoogleSheets();
      }
    }
  }
}
