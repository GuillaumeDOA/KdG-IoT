#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DFRobot_PN532.h>
#include <config.h>
#include <secret.h>

// WiFi and MQTT Client objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// NFC object
DFRobot_PN532_IIC nfc(PN532_IRQ, POLLING);
uint8_t dataRead[16] = {0};

// Initialise WiFi
void initWiFi()
{
  Serial.println("Starting WiFi Connection");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PASSWORD);

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
    if (mqttClient.connect("SmartLock", MQTT_USERNAME, MQTT_PASSWORD))
    {
      Serial.println("connected");
      //mqttClient.subscribe(MQTT_TEST);
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

// Initialise the NFC module
void initNFC()
{
  Serial.print("Initializiing NFC\n");
  while (!nfc.begin())
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" NFC ready to accept cards");
}

// Function to read NFC card
void readNFC()
{
  if (nfc.scan())
  {
    if (nfc.readData(dataRead, READ_BLOCK_NO) != 1)
    {
      Serial.println(" read failure!");
    }
    else
    {
      Serial.print("NFC string: ");
      Serial.println((char *)dataRead);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  initWiFi();
  initNFC();

  mqttClient.setServer(MQTT_ADDRESS, MQTT_PORT);
  mqttClient.setCallback(callback);
}

void loop()
{
  // Start MQTT Connection
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();

  // TODO Lock logic
  // Read NFC card and send data to MQTT broker
  // Receieve MQTT message and check if it is a valid NFC card
  // Open lock if NFC card is valid
  //
  // TODO Hall effect sensor logic
  // Read hall effect sensor and send data to MQTT broker  

}
