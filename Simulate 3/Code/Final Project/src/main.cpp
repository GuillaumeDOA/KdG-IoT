#include <Arduino.h>
#include <DFRobot_PN532.h>
#include <SensirionI2CScd4x.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <secrets.h>
extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}

// NFC Config Variables
#define BLOCK_SIZE 16
#define PN532_IRQ 2
#define POLLING 0

// Set Currentsensor Variables
#define ACTectionRange 20
#define VREF 3.3

// Sensor pinout
#define PIR D12
#define CURRENTSENSOR A3

// The block to be read
#define READ_BLOCK_NO 2

// PIR State
int state = LOW;
int val = 0; 

// WiFi and MQTT Client objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// NFC Module object
DFRobot_PN532_IIC nfc(PN532_IRQ, POLLING);
uint8_t dataRead[16] = {0};

// SCD4X Sensor object and variables
SensirionI2CScd4x scd4x;
uint16_t error;
char errorMessage[256];

// Timer variables
unsigned long nfcDelay, sensorDelay, doorTimer;
bool openDoor = false;

// Servo setup
Servo door;
Servo window;

void printUint16Hex(uint16_t value)
{
  Serial.print(value < 4096 ? "0" : "");
  Serial.print(value < 256 ? "0" : "");
  Serial.print(value < 16 ? "0" : "");
  Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2)
{
  Serial.print("Serial: 0x");
  printUint16Hex(serial0);
  printUint16Hex(serial1);
  printUint16Hex(serial2);
  Serial.println();
}

// Initialise SCD4X sensor
void initSCD4X()
{
  Serial.print("Inittializing SCD4X\n");
  scd4x.begin(Wire);

  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  uint16_t serial0;
  uint16_t serial1;
  uint16_t serial2;
  error = scd4x.getSerialNumber(serial0, serial1, serial2);
  if (error)
  {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    exit(1);
  }
  else
  {
    printSerialNumber(serial0, serial1, serial2);
    Serial.println("SCD4X Sensor Ready");
  }

  error = scd4x.startPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
}

// Initialise NFC module
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

// Function to read NFC module
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
      // Send data to MQTT Broker
      mqttClient.publish(MQTT_NFC, (char *)dataRead);
    }
    nfcDelay = millis();
  }
}

// Function to read SCD4X sensor
void readSCD4X()
{
  uint16_t co2;
  float temperature;
  float humidity;

  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error)
  {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  else if (co2 == 0)
  {
    Serial.println("Invalid sample detected, skipping.");
  }
  else
  {
    Serial.print("Co2:");
    Serial.print(co2);
    Serial.print(" ppm");
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.print(" °C");
    Serial.print("\t");
    Serial.print("Humidity:");
    Serial.print(humidity);
    Serial.println(" %");

    char msgStr[50];

    // Send Data to MQTT Broker
    itoa(co2, msgStr, 10);
    mqttClient.publish(MQTT_CO2, msgStr);

    itoa(temperature, msgStr, 10);
    mqttClient.publish(MQTT_TEMP, msgStr);

    itoa(humidity, msgStr, 10);
    mqttClient.publish(MQTT_HUMIDITY, msgStr);
  }
  door.write(0);
}

// Function to read AC current sensor
void readACCurrent()
{
  float ACCurrtntValue = 0;
  float peakVoltage = 0;
  float voltageVirtualValue = 0; // Vrms
  for (int i = 0; i < 5; i++)
  {
    peakVoltage += analogRead(CURRENTSENSOR); // read peak voltage
    delay(1);
  }
  peakVoltage = peakVoltage / 5;
  voltageVirtualValue = peakVoltage * 0.707; // change the peak voltage to the Virtual Value of voltage

  /*The circuit is amplified by 2 times, so it is divided by 2.*/
  voltageVirtualValue = (voltageVirtualValue / 1024 * VREF) / 2;

  ACCurrtntValue = voltageVirtualValue * ACTectionRange;
  Serial.print("Currentsensor Value: ");
  Serial.println(ACCurrtntValue);

  char msgStr[50];
  itoa(ACCurrtntValue, msgStr, 10);
  mqttClient.publish(MQTT_CURRENT, msgStr);
}

// Function to read PIR sensor
void readPIRSensor()
{
 val = digitalRead(PIR);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      // Turn Leds on
      digitalWrite(D6, HIGH);
      mqttClient.publish(MQTT_MOTION, "Motion Detected");
      state = HIGH;
    }
  } 
  else {
      if (state == HIGH){
        Serial.println("Motion stopped!");
        // Turn Leds off
        digitalWrite(D6, LOW);
        mqttClient.publish(MQTT_MOTION, "No Motion Detected");
        state = LOW;
      
    }
  }
}

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

  String topicStr = (String)topic;

  char message[length + 1];
  for (int i = 0; i < length; i++)
  {
    message[i] = (char)payload[i];
  }
  message[length] = '\0'; // Null-terminate the char array

  String receivedMsg = String(message);
  Serial.println("Received message: " + receivedMsg);

  if (topicStr.compareTo((String)MQTT_DOORRESPONSE) == 0)
  {
    door.write(180);
    doorTimer = millis();
  }
  else if (topicStr.compareTo((String)MQTT_WINDOWRESPOSNE) == 0)
  {
    if (receivedMsg.compareTo("true") == 0)
    {
      // Set window servo open
      window.write(0);
    }
    else
    {
      // Set window servo closed
      window.write(180);
    }
  }
  else
  {
    Serial.println("Unknown Topic");
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
    if (mqttClient.connect("ESP-IntelliHome"))
    {
      Serial.println("connected");
      // mqttClient.subscribe("test/led");
      mqttClient.subscribe(MQTT_DOORRESPONSE);
      mqttClient.subscribe(MQTT_WINDOWRESPOSNE);
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
  Wire.begin();
  Serial.begin(115200);
  delay(50);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(CURRENTSENSOR, INPUT);
  pinMode(D6, OUTPUT);

  Serial.println("Initializing System\n");

  /* initSCD4X();
  initNFC();
  initWiFi(); */

  mqttClient.setServer(RPI_ADDRESS, 1883);
  mqttClient.setCallback(callback);

  door.attach(D10);
  window.attach(D11);

  nfcDelay, sensorDelay, doorTimer = millis();
}

void loop()
{
  while (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();

  if (millis() - nfcDelay >= 2000)
  {
   // readNFC();
  }

  if (millis() - sensorDelay >= 50)
  {
    // readSCD4X();
    readACCurrent();
    sensorDelay = millis();
  }

  if (doorTimer + 5000 <= millis())
  {
    // Close door after 5 sec
    door.write(0);
  }
  
  readPIRSensor();
}