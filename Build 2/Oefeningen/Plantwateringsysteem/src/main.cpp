#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>
#include "secrets.h"
#include "config.h"

#define RESISTIVE_SENSOR A0
#define CAPACITIVE_SENSOR A1
#define TEMPARETURE_SENSOR A2
#define PUMP_RELAY D13

// Setup ntp time server
const char *ntpServer = "pool.ntp.org";

// Firebase config variables
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
FirebaseJson json;

// Firebase paths
String parentPath;
String databasePath = "/readings";
String timePath = "/timestamp";
String resistivePath = "/resistive";
String capacitivePath = "/capacitive";
String statusPath = "/status";
String temperaturePath = "/temperature";

int timestamp;
unsigned long startTime;
unsigned long timerDelay = 10000;

// Setup WiFi
void setupWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(" connected!");
}

// Check Current wiFi status
bool checkWifi()
{
    Serial.println("Checking WiFi Status") unsigned long wifiReconnectTime = millis();
    unsigned long tryInterval = millis();
    while (millis() - wifiReconnectTime < 10000)
    {
        if (millis() - tryInterval < 1000)
            continue;

        tryInterval = millis();

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("WiFi Connected");
            return true;
        }

        Serial.prinln("Trying to reconnect to WiFi");
        WiFi.disconnect();
        WiFi.reconnect();
    }
    return false;
}

// Gets current time in epoch format
unsigned long getTime()
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        // Serial.println("Failed to obtain time");
        return (0);
    }
    time(&now);
    return now;
}

// Write json data to Firebase RTDB
bool writeJson(String status, int capacitive, int resistive, float temperature)
{
    // Get current timestamp
    timestamp = getTime();

    parentPath = databasePath + "/" + String(timestamp);

    // Prepare json
    json.set(resistivePath, resistive);
    json.set(capacitivePath, capacitive);
    json.set(statusPath, status);
    json.set(timePath, timestamp);
    json.set(temperaturePath, temperature);

    // Send json and capture error
    return Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json);
}

// Get status based on sensor values
String getStatus(int resisitve, int capacitive)
{
    // Logic that decides current moisture status
    // "Droog" - "Vochitg" - " Nat"

    String calibratedResistive;
    String calibratedCapacitive;

    if (resisitve >= RESISTIEF_DROOG_INTERVAL_MIN && resisitve <= RESISTIEF_DROOG_INTERVAL_MAX)
        calibratedResistive = "Droog";
    else if (resisitve >= RESISTIEF_VOCHTIG_INTERVAL_MIN && resisitve <= RESISTIEF_VOCHTIG_INTERVAL_MAX)
        calibratedResistive = "Vochtig";
    else if (resisitve >= RESISTIEF_NAT_INTERVAL_MIN && resisitve <= RESISTIEF_NAT_INTERVAL_MAX)
        calibratedResistive = "Nat";

    if (capacitive >= CAPACITIEF_DROOG_INTERVAL_MIN && capacitive <= CAPACITIEF_DROOG_INTERVAL_MAX)
        calibratedCapacitive = "Droog";
    else if (capacitive >= CAPACITIEF_VOCHTIG_INTERVAL_MIN && capacitive <= CAPACITIEF_VOCHTIG_INTERVAL_MAX)
        calibratedCapacitive = "Vochtig";
    else if (capacitive >= CAPACITIEF_NAT_INTERVAL_MIN && capacitive <= CAPACITIEF_NAT_INTERVAL_MAX)
        calibratedCapacitive = "Nat";

    if (calibratedCapacitive == "Droog" || calibratedResistive == "Droog")
        return "Droog";
    if (calibratedCapacitive == "Vochtig" || calibratedResistive == "Vochtig")
        return "Vochtig";
    if (calibratedCapacitive == "Nat" || calibratedResistive == "Nat")
        return "Nat";

    return "Vochtig";
}

// Read moisture sensors values
void readSensors(int *resistive, int *capacitive)
{
    // Logic that reads moisture sensors
    *resistive = analogRead(RESISTIVE_SENSOR);
    *capacitive = analogRead(CAPACITIVE_SENSOR);
}

// Activate water pump for a duration based in soilstatus
void activatePump(String status)
{
    if (status == "Nat")
        return;

    // Activate pump based on if ground is "Droog" or "Vochtig"
    unsigned long pumpTime;
    if (status == "Droog")
        pumpTime = 4000;
    else
        pumpTime = 2000;

    unsigned long pumpStartTime = millis();
    Serial.print("Pumping water");
    do
    {
        digitalWrite(PUMP_RELAY, HIGH);
    } while (millis() - pumpStartTime < pumpTime);
    digitalWrite(PUMP_RELAY, LOW);
    Serial.println("\nWater done pumping");
}

// Read and return temperature sensor
float readTemperature()
{
    // Read the analog value from LM35
    int sensorValue = analogRead(TEMPARETURE_SENSOR);

    // Convert analog value to voltage (0-3.3V)
    float voltage = sensorValue * (3.3 / 4095.0);

    // Convert voltage to temperature in Celsius
    return voltage * 100.0;
}

void setup()
{
    Serial.begin(115200);

    // Setup Sensors and Relay
    pinMode(RESISTIVE_SENSOR, INPUT);
    pinMode(CAPACITIVE_SENSOR, INPUT);
    pinMode(PUMP_RELAY, OUTPUT);
    pinMode(TEMPARETURE_SENSOR, INPUT);

    setupWifi();

    // Setup time server
    configTime(0, 0, ntpServer);

    // Setup Firebase RTDB
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    config.token_status_callback = tokenStatusCallback;
    config.max_token_generation_retry = 5;

    // Start firebase connection
    Firebase.begin(&config, &auth);
}

void loop()
{
    if (Firebase.ready() && (millis() - startTime > timerDelay))
    {
        startTime = millis();

        // Read temperature tensor
        int temperatureValue = readTemperature();

        // Read and store Sensor values
        int resistiveSensorVal, capacitiveSensorVal;
        readSensors(&resistiveSensorVal, &capacitiveSensorVal);

        // Get status based on Sensor values and active pump
        String moistureStatus = getStatus(resistiveSensorVal, capacitiveSensorVal);

        // Acticate pump if temperature is not below minimum
        if (temperatureValue > MINIMUM_TEMPERATUUR)
            activatePump("Vochtig");

        // Finally send data to Firebase RTDB
        if (checkWifi())
            Serial.printf("Set json %s\n", writeJson(moistureStatus, capacitiveSensorVal, resistiveSensorVal, temperatureValue) ? "ok" : fbdo.errorReason().c_str());
        else
            Serial.println("Failed to send json.\nNot connected to WiFi") :
    }
}