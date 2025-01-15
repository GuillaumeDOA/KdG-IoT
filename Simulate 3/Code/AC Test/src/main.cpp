#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

#define ACPin A2           // set arduino signal read pin
#define ACTectionRange 20; // set Non-invasive AC Current Sensor tection range (5A,10A,20A)

// VREF: Analog reference
// For Arduino UNO, Leonardo and mega2560, etc. change VREF to 5
// For Arduino Zero, Due, MKR Family, ESP32, etc. 3V3 controllers, change VREF to 3.3
#define VREF 3.3

void initWiFi()
{
  Serial.println("Starting WiFi Connection");
  WiFi.mode(WIFI_STA);
  WiFi.begin("Guillaume", "wachtwoord");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" WiFi connected!");
}

void callback(char *topic, byte *payload, unsigned int length)
{
}

void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ESP-IntelliHome"))
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

float readACCurrentValue()
{
  float ACCurrtntValue = 0;
  float peakVoltage = 0;
  float voltageVirtualValue = 0; // Vrms
  for (int i = 0; i < 5; i++)
  {
    peakVoltage += analogRead(ACPin); // read peak voltage
    delay(1);
  }
  peakVoltage = peakVoltage / 5;
  voltageVirtualValue = peakVoltage * 0.707; // change the peak voltage to the Virtual Value of voltage

  /*The circuit is amplified by 2 times, so it is divided by 2.*/
  voltageVirtualValue = (voltageVirtualValue / 1024 * VREF) / 2;

  ACCurrtntValue = voltageVirtualValue * ACTectionRange;

  return ACCurrtntValue/100;
}

void setup()
{
  Serial.begin(115200);
  pinMode(ACPin, INPUT);
  initWiFi();

  mqttClient.setServer("109.137.19.53", 1883);
  mqttClient.setCallback(callback);
}

void loop()
{
  while (!mqttClient.connected()){
    reconnect();
  }
  mqttClient.loop();

  float ACCurrentValue = readACCurrentValue(); // read AC Current Value

  Serial.print(ACCurrentValue);
  Serial.println(" A");

  
  char msgStr[8];
  dtostrf(ACCurrentValue, 0, 4, msgStr);
  mqttClient.publish("intellihome/current", msgStr);
  

  delay(2000);
}