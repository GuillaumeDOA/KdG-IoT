#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT Client objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// WiFi credentials
const char *SSID = "Guillaume";
const char *PASSWORD = "wachtwoord";

// MQTT Broker address
const char *MQTT_ADDRESS = "172.201.156.113";
const int MQTT_PORT = 1883;

// MQTT Credentials
const char *Username = "itsybitsy";
const char *Password = "StrongP@ssword";

// MQTT Topics
const char *MQTT_TEST = "test";

// Initialise WiFi
void initWiFi()
{
  Serial.println("Starting WiFi Connection");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" WiFi connected!");
}

// MQTT Callback funtion
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
}

// MQTT Reconnect funtion
void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("SmartLock", Username, Password))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  initWiFi();

  mqttClient.setServer(MQTT_ADDRESS, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop()
{
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();

  // Publish a message to the MQTT topic
  mqttClient.publish(MQTT_TEST, "Hello from ESP32! ");

  // Wait for a while before sending the next message
  delay(2000);
}
