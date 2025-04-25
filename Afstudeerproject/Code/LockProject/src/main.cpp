#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DFRobot_PN532.h>
#include <config.h>
#include <secret.h>

// Pin Definitions
#define LOCK_PIN 12

// WiFi and MQTT Client objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// NFC object
DFRobot_PN532_IIC nfc(PN532_IRQ, POLLING);
uint8_t dataRead[16] = {0};

// Global Variables
unsigned long nfcTimer;
bool lockActive = false;
unsigned long lockTimer = 0;

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
  Serial.print("Mssage arrived on topic: ");
  Serial.println(topic);

  // Check if the message is on the MQTT_RESULT topic
  if (strcmp(topic, MQTT_RESULT) == 0)
  {
    // Convert the payload to a string
    String message;
    for (unsigned int i = 0; i < length; i++)
    {
      message += (char)payload[i];
    }

    Serial.print("Message: ");
    Serial.println(message);

    // Check if the result is "true"
    if (message == "true")
    {
      Serial.println("Valid card detected. Opening lock...");
      digitalWrite(LOCK_PIN, HIGH); // Open the lock
      lockActive = true;
      lockTimer = millis(); // Start the lock timer
    }
    else
    {
      Serial.println("Invalid card. Access denied.");
    }
  }
  else
  {
    Serial.print("Message: ");
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
    }
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
      mqttClient.subscribe(MQTT_RESULT);
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
      Serial.println("Read failure!");
    }
    else
    {
      Serial.print("NFC string: ");
      Serial.println((char *)dataRead);

      // Sending Json over MQTT
      String payload = "{\"tag_secret\":\"" + String((char *)dataRead) + "\",\"door_name\":\"" + DEVICE_NAME + "\"}";
      mqttClient.publish(MQTT_READ, payload.c_str());
    }
    nfcTimer = millis(); // Reset the timer to avoid reading the same card multiple times
  }
}

void setup()
{
  // Start Serial Communication
  Serial.begin(115200);

  // Initialise Wifi and MQTT
  initWiFi();
  initNFC();

  // Set MQTT server and callback
  mqttClient.setServer(MQTT_ADDRESS, MQTT_PORT);
  mqttClient.setCallback(callback);

  // Set Pin Modes
  pinMode(LOCK_PIN, OUTPUT);

  digitalWrite(LOCK_PIN, LOW); // Lock the door by default

  // Start the NFC timer
  nfcTimer = millis();
}

void loop()
{
  // Start MQTT Connection
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();


  //
  // TODO Hall effect sensor logic
  // Read hall effect sensor and send data to MQTT broker

  // NFC Logic
  if (millis() - nfcTimer >= NFC_DELAY)
  {
    readNFC();
  }

  if (lockActive && millis() - lockTimer >= 5000)
  {
    Serial.println("Locking the door...");
    digitalWrite(LOCK_PIN, LOW); // Lock the door
    lockActive = false;
  }
}
