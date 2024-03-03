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

int timestamp;
unsigned long startTime;
unsigned long timerDelay = 5000;

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

bool writeJson(String status, int capacitive, int resistive)
{
    // Get current timestamp
    timestamp = getTime();

    parentPath = databasePath + "/" + String(timestamp);

    // Prepare json
    json.set(resistivePath, resistive);
    json.set(capacitivePath, capacitive);
    json.set(statusPath, status);
    json.set(timePath, timestamp);

    // Send json and capture error
    return Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str();
}

String getStatus(int resisitve, int capacitive)
{
    // Logic that decides current moisture status
    // "Droog" - "Vochitg" - " Nat"
    return "";
}

void readSensors(int *resistive, int *capacitive)
{
    // Logic that reads moisture sensors
}

void activatePump(String status)
{
    if (status == "Nat")
        return;
    // Activate pump based on if ground is "Droog" or "Vochtig"
}

void setup()
{
    Serial.begin(115200);

    // Setup Sensors
    pinMode(RESISTIVE_SENSOR, INPUT);
    pinMode(CAPACITIVE_SENSOR, INPUT);

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

    Firebase.begin(&config, &auth);
}

void loop()
{
    if (Firebase.ready() && (millis() - startTime > timerDelay))
    {
        startTime = millis();
        
        // Read and store Sensor values
        int resistiveSensorVal, capacitiveSensorVal;
        readSensors(&resistiveSensorVal, &capacitiveSensorVal);
        
        // Get status based on Sensor values and active pump
        String moistureStatus = getStatus(resistiveSensorVal, capacitiveSensorVal);
        activatePump(moistureStatus);

        // Finally send data to Firebase RTDB
        Serial.printf("Set json %s\n", writeJson(moistureStatus, capacitiveSensorVal, resistiveSensorVal) ? "ok" : fbdo.errorReason().c_str());
    }
}