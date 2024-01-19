#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PM25AQI.h>
#include <WiFiS3.h>
#include <secrets.h>

// Define your WiFi credentials and Google Sheets information
const char *ssid = SSID;
const char *password = PASSWORD;
const char *host = "script.google.com";                                                                     // This should be the URL to your Google Script
const String googleScriptId = "AKfycbx6OLzjwRdO3pryYKfyD15jw_PQAdihov3ChnbnDKqyPrer2xs6rsyr7AwVs98RyxCLIQ"; // EIGEN SCRIPT

// Define LED pins
#define POWERLED 4
#define WIFILED 3
#define DATALED 5

// Initialize the PM2.5 sensor and WiFi client
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
PM25_AQI_Data data;

int status = WL_IDLE_STATUS;
int previousMinute = -1;
int startTime;

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
  if (!aqi.read(&data))
  {
    Serial.println("Could not read from AQI");
    delay(500);
    return false;
  }
  Serial.println("AQI reading success");
  Serial.println(data.pm25_env);
  Serial.println(data.pm100_env);
  return true;
}

void sendToGoogleSheets()
{
  WiFiSSLClient client;

  Serial.println("Attempting to connect to sheet");
  if (client.connect(host, 443))
  {
    String url = "/macros/s/" + googleScriptId + "/exec?";
    url += "pm2=" + String(data.pm25_env);
    url += "&pm10=" + String(data.pm100_env);
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");


    // Read and print the server's response
    Serial.println("Server response:");
    while (client.available() ||client.connected()) {
      char c = client.read();
      Serial.print(c);
    }

    // Wait for server response and read it
    client.stop();
  }
  else
  {
    Serial.println("Upload to Sheet failed");
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

  startTime = millis();
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

  if (millis() - startTime >= 10000)
  {
    digitalWrite(DATALED, HIGH);
    startTime = millis();
    if (readAQI())
    {
      sendToGoogleSheets();
    }
    digitalWrite(DATALED, LOW);
  }
}
