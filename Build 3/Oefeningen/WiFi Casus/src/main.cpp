/*
Voor dit project heb ik gebruikt gemaakt van ChatGPT voor het generen van de html webpagina
 en het arduino forum voor het gebruik van de strcmp functie
  - Arduino forum - https://forum.arduino.cc/t/comparing-a-char-array-to-a-string/270392/4
  - OpenAI - ChatGPT https://chatgpt.com
*/

#include <Arduino.h>
#include <WiFi.h>
#include <sectrets.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

HTTPClient http;
AsyncWebServer server(80);

String currentJoke = "";

void initWiFi()
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.print(" Connected to network! ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void stopProgramma()
{
  while (true)
  {
    Serial.println("Failed to reconnect. Check connection details and restart");
    delay(1000);
  }
}

void reconnectWiFi()
{
  for (int i = 0; i < 10; i++)
  {
    Serial.println("Attempting to reconnect");
    WiFi.reconnect();
    delay(1000);
    if (WiFi.status() == WL_CONNECTED)
      return;
  }

  stopProgramma();
}

void nextJoke()
{
  if (WiFi.status() != WL_CONNECTED)
    reconnectWiFi();

  http.begin(URL);
  int responseCode = http.GET();
  if (responseCode > 0)
  {
    String JSON_data = http.getString();
    // Retrieve the joke from the JSON format
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, JSON_data);
    JsonObject obj = doc.as<JsonObject>();

    const char *jokeType = obj["type"].as<const char *>();

    if (strcmp(jokeType, "twopart") == 0) // strcmp() function van deze vraag op arduino forum: https://forum.arduino.cc/t/comparing-a-char-array-to-a-string/270392/4
    {
      const char *setup = obj["setup"].as<const char *>();
      const char *delivery = obj["delivery"].as<const char *>();
      currentJoke = setup;
      currentJoke += " ";
      currentJoke += delivery;
      Serial.println(setup);
      Serial.println(delivery);
    }
    else
    {
      const char *joke = obj["joke"].as<const char *>();
      currentJoke = joke;
      Serial.println(joke);
    }
  }
  http.end();

  http.end(); // End the HTTP connection
}

void setup()
{
  Serial.begin(115200);
  initWiFi();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });

  server.on("/nextjoke", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    nextJoke();
    request->send(200, "text/plain", currentJoke); });

  server.begin();
  Serial.println("Webserver Started");
}

void loop()
{
}
