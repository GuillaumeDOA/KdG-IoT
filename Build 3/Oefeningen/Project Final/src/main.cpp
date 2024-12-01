/*  Voor dit project heb ik gebruik gemaakt van RandomNerdTutorials voor het maken en begrijpen van de node-red functionaliteiten
    - RandomNerdTutorials - https://randomnerdtutorials.com/esp32-mqtt-publish-bme680-arduino/ (geraadpleegd op 21/11/2024)
 */
#include <Arduino.h>
#include <ezButton.h>
#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <secrets.h>
extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}

#define RED D3
#define GREEN D6
#define TEMPSENSOR A1

ezButton button(D2);

unsigned long startTime;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void connectToWifi()
{
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(SSID, PASSWORD);
}

void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event)
{
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event)
  {
  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    connectToMqtt();
    break;
  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    Serial.println("WiFi lost connection");
    xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    xTimerStart(wifiReconnectTimer, 0);
    break;
  }
}

void onMqttConnect(bool sessionPresent)
{
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  // Subscribe to our LED topic to get updates
  mqttClient.subscribe(MQTT_LED_TOPIC, 1);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected())
  {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttPublish(uint16_t packetId)
{
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  Serial.print("Message received on topic ");
  Serial.print(topic);
  Serial.print(": ");
  for (size_t i = 0; i < len; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Convert payload to string and control LED
  String message = String((char *)payload).substring(0, len);
  if (message == "ON")
  {
    digitalWrite(GREEN, HIGH);
  }
  else if (message == "OFF")
  {
    digitalWrite(GREEN, LOW);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(TEMPSENSOR, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  button.setDebounceTime(100);

  // Timers for reconnecting to WiFi and MQTT Server
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  // Built-in functions of the MQTTClient library to handle WiFi events
  WiFi.onEvent(WiFiEvent);

  // MQTT setup
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(RPI_ADDRESS, MQTT_PORT);

  connectToWifi();

  startTime = millis();
}

// Function to read temperatuse sensor
float ReadTempSensor()
{
  int analogValue = analogRead(TEMPSENSOR);
  float voltage = (analogValue / 4095.0) * 3.3 * 1.1;
  float temp = voltage * 100.0 + 5.0;

  return temp;
}

// Function to write temperature to serial monitor and publish to MQTT server
void WriteTemp()
{
  int temp = ReadTempSensor();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println("°C");

  // Publish an MQTT message on topic esp/LM35/temperature
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_TEMP_TOPIC, 1, true, String(temp).c_str());
  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_TEMP_TOPIC, packetIdPub1);
  Serial.printf("Message: %i \n", temp);

  if (temp >= 25)
  {
    digitalWrite(RED, HIGH);
  }
  else
  {
    digitalWrite(RED, LOW);
  }
}

void loop()
{
  button.loop();

  // On button press do something
  if (button.isReleased())
  {
    WriteTemp();
  }

  // Publish Temperature every 10 seconds
  if (millis() - startTime >= 10000)
  {
    WriteTemp();
    startTime = millis();
  }
}
