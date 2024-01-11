#include <Arduino.h>
#include <WiFiS3.h>

const char *ssid = "IoT";
const char *password = "KdGIoT84!";
const char *scriptUrl = "https://script.google.com/macros/s/1nMw2fjOdXdewwhxSBdz0Y5QBbdtndqma6GTQxRGw62Y3Xq9adKfFqjC4/exec";

void connectToWiFi()
{
  Serial.println("Connecting to WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
}

void sendToGoogleScript(String data)
{
  Serial.println("Sending data to Google Script...");

  WiFiClient client;

  if (client.connect(scriptUrl, 443))
  {
    Serial.println("Connected to server");

    // Make a POST request
    client.print("POST " + String(scriptUrl) + " HTTP/1.1\r\n");
    client.print("Host: " + String(scriptUrl) + "\r\n");
    client.print("Content-Type: application/json\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);

    // Read the response from the server
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        Serial.print(c);
      }
    }

    Serial.println();
    Serial.println("Request complete");

    client.stop();
  }
  else
  {
    Serial.println("Connection to server failed");
  }
}

void setup()
{
  Serial.begin(115200);
  connectToWiFi();
}

void loop()
{
  // Your data to be sent in JSON format
  String dataToSend = "{\"pm2\": 1, \"pm10\": 45}";

  // Send data to Google Apps Script
  sendToGoogleScript(dataToSend);

  // Delay between subsequent requests
  delay(5000);
}
