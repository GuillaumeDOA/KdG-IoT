#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DFRobot_PN532.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <config.h>
#include <secret.h>

// Pin Definitions
#define LOCK_PIN 12
#define HALL_SENSOR A5

// OLED Display Object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi and MQTT Client objects
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// NFC object
DFRobot_PN532_IIC nfc(PN532_IRQ, POLLING);
uint8_t dataRead[BLOCK_SIZE] = {0};
uint8_t dataWrite[BLOCK_SIZE] = {""};

// Global Variables
unsigned long nfcReadTimer, hallTimer, lockStartTime, nfcWriteTimer, displayTimer;
bool lockActive = false;
bool isWriteMode = false;
bool displayChanged = false;
String doorState = "Closed", previousDoorState = "Closed";

// Initialise the OLED display
void initDisplay()
{
  Serial.println(F("Initializing OLED display..."));

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1315 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.println(F(DEVICE_NAME));
  display.setCursor(0, 16);
  display.println(F("Booting up System"));
  display.display();
}

// Function to write to the OLED display
void writeToDisplay(const char *message)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println((String(DEVICE_NAME) + " - " + doorState).c_str());
  display.setCursor(0, 16);
  display.println(message);
  display.display();
}

// Function to append to the OLED display
void appendToDisplay(const char *message, int cursorX = 0, int cursorY = 16)
{
  // Set the cursor to the specified position
  display.setCursor(cursorX, cursorY);

  // Write the new message
  display.println(message);

  // Update the display with the new content
  display.display();
}

// Function to update disply to default state
void defaultDisplay()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println((String(DEVICE_NAME) + " - " + doorState).c_str());
  display.setCursor(0, 16);
  display.println(F("Ready To Accept Cards"));
  display.display();
}

// Initialise WiFi
void initWiFi()
{
  Serial.println("Starting WiFi Connection");
  writeToDisplay("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" WiFi connected!");
  appendToDisplay("WiFi connected!", 0, 32);
}

// MQTT Callback funtion
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mssage arrived on topic: ");
  Serial.println(topic);

  // Parse the Json pyaload
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  Serial.print("Message: ");
  Serial.println(message);

  // Parse the JSON payload
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, message);

  // Check for parsing errors
  if (error)
  {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract "doorname" from the JSON
  const char *doorname = doc["DoorName"];

  // Check if the door name matches the device name
  if (strcmp(doorname, DEVICE_NAME) != 0)
  {
    Serial.println("DoorName mismatch. Ignoring message.");
    return;
  }

  // Check if the message is on the MQTT_RESULT topic
  if (strcmp(topic, MQTT_RESULT) == 0)
  {
    // Extract "unlock" from the JSON
    bool unlock = doc["Unlock"];

    // Open the lock if the door name matches and unlock is true
    if (unlock)
    {
      // Extract "User" from the JSON
      const char *user = doc["User"];

      Serial.println("Valid card detected. Opening lock...");
      appendToDisplay("Acces granted!", 0, 32);
      appendToDisplay(("Welcome " + String(user)).c_str(), 0, 48);
      
      digitalWrite(LOCK_PIN, HIGH); // Open the lock
      lockActive = true;
      lockStartTime = millis(); // Start the lock timer
      displayTimer = millis();  // Start the display timer
      displayChanged = true; // Set display changed flag
    }
    else
    {
      Serial.println("Access denied.");
      appendToDisplay("Access denied!", 0, 32);

      displayTimer = millis(); // Start the display timer
      displayChanged = true; // Set display changed flag
    }
  }
  // Check if the message is on the MQTT_WRITE topic
  else if (strcmp(topic, MQTT_WRITE) == 0)
  {
    // Extract TagSecret from the JSON
    const char *secret = doc["TagSecret"];

    // Set dataWrite to the new Secret
    memset(dataWrite, 0, BLOCK_SIZE);                   // Clear the dataWrite buffer
    strncpy((char *)dataWrite, secret, BLOCK_SIZE - 1); // Copy the new secret to dataWrite
    dataWrite[BLOCK_SIZE - 1] = '\0';                   // Null-terminate the string

    // Set NFC Wrtite mode to True and start Timer
    isWriteMode = true;
    nfcWriteTimer = millis();

    Serial.println("NFC Write mode activated. Please scan the card to write the new secret.");
    writeToDisplay("Write mode activated!Please scan the card.");
  }
  // Unhandled topic
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
      mqttClient.subscribe(MQTT_WRITE);
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
    if (nfc.readData(dataRead, BLOCK_NO) != 1)
    {
      Serial.println("Read failure!");
    }
    else
    {
      Serial.print("NFC string: ");
      Serial.println((char *)dataRead);

      writeToDisplay("Card Detected!\nChecking access...");

      // Sending Json over MQTT
      String payload = "{\"tag_secret\":\"" + String((char *)dataRead) + "\",\"door_name\":\"" + DEVICE_NAME + "\"}";
      mqttClient.publish(MQTT_READ, payload.c_str());
    }
    nfcReadTimer = millis(); // Reset the timer to avoid reading the same card multiple times
  }
}

// Function to write NFC card
void writeNFC()
{
  if (nfc.scan())
  {
    if (nfc.writeData(BLOCK_NO, dataWrite) != 1)
    {
      Serial.println("Write failure!");
    }
    else
    {
      Serial.print("Data written (string): ");
      Serial.println((char *)dataWrite);
      appendToDisplay("Card detected!\nSecret written...", 0, 48);
      isWriteMode = false; // Reset write mode
      displayTimer = millis(); // Start the display timer
      displayChanged = true; // Set display changed flag
    }
  }
}

// Function to read Hall Sensor
void readHallSensor()
{
  // Read the hall sensor value
  int hallValue = analogRead(HALL_SENSOR);
  Serial.print("Hall Sensor Value: ");
  Serial.println(hallValue);

  // Determine the door state based on the Hall sensor value
  if (hallValue > 2800) // Adjust threshold based on your sensor's behavior
  {
    doorState = "Closed";
  }
  else
  {
    doorState = "Open";
  }

  // Sending Json over MQTT if door state has changed
  if (!doorState.equals(previousDoorState))
  {
    Serial.print("Door state changed: ");
    Serial.println(doorState);
    previousDoorState = doorState;

    // Sending Json over MQTT
    String payload = "{\"doorstate\":\"" + doorState + "\",\"doorname\":\"" + DEVICE_NAME + "\"}";
    // mqttClient.publish(MQTT_HALLSENSOR, payload.c_str());
  }
}

void setup()
{
  // Start Serial Communication
  Serial.begin(115200);

  // Initialise the OLED display
  initDisplay();

  // Initialise Wifi and MQTT
  initWiFi();
  initNFC();

  // Set MQTT server and callback
  mqttClient.setServer(MQTT_ADDRESS, MQTT_PORT);
  mqttClient.setCallback(callback);

  // Set Pin Modes
  pinMode(LOCK_PIN, OUTPUT);
  pinMode(HALL_SENSOR, INPUT);

  // Lock the door by default
  digitalWrite(LOCK_PIN, LOW);

  // Start the NFC timer
  nfcReadTimer = millis();
  hallTimer = millis();

  // Clear the display
  defaultDisplay();
}

void loop()
{
  // Start MQTT Connection
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();

  // Read NFC
  if (!isWriteMode && millis() - nfcReadTimer >= NFC_READ_DELAY)
  {
    readNFC();
  }

  // Read Hall Sensor
  if (millis() - hallTimer >= HALLSENSOR_DELAY)
  {
    readHallSensor();
    hallTimer = millis(); // Reset the timer
  }

  // Check if the lock should be deactivated
  if (lockActive && millis() - lockStartTime >= LOCK_TIMER)
  {
    Serial.println("Locking the door...");
    digitalWrite(LOCK_PIN, LOW); // Lock the door
    lockActive = false;
  }

  // Check if NFC write mode is active
  if (isWriteMode)
  {
    writeNFC();
    nfcReadTimer = millis(); // Reset the NFC timer to avoid reading the same card multiple times
  }

  // Check if NFC write mode timer has expired
  if (isWriteMode && millis() - nfcWriteTimer >= NFC_WRITE_DELAY)
  {
    Serial.println("NFC Write mode timed out.");
    isWriteMode = false; // Reset write mode
    defaultDisplay(); // Reset the display to default state
  }

  if (displayChanged && millis() - displayTimer >= DISPLAY_UPDATE_INTERVAL)
  {
    displayChanged = false; // Reset the flag
    defaultDisplay();       // Reset the display to default state
  }
}
