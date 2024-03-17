#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>
#include <ESPAsyncWebServer.h>
#include "secrets.h"
#include "config.h"

#define RESISTIVE_SENSOR A0
#define CAPACITIVE_SENSOR A1
#define TEMPARETURE_SENSOR A2
#define PUMP_RELAY D13

int resistiveSensorVal, capacitiveSensorVal;

// Setup ntp time server
const char *ntpServer = "pool.ntp.org";

// Create Async Webserver object on port 80
AsyncWebServer server(80);

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
String capacitiveStatusPath = "/capacitiveStatus";
String resistiveStatusPath = "/resistiveStatus";
String temperaturePath = "/temperature";

int timestamp;
unsigned long startTime;

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

// Setup Access Point
void setupWifiAccessPoint()
{
    // Set Wifi mode to both standard connection and AP
    WiFi.mode(WIFI_AP_STA);

    // Make sure the password is at least 8 characters long
    Serial.println("Setting up soft access point...");
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    // Display IP address to use for webserver
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

// Return Moisturelevel in percentage
String moistureLevel()
{
    int val = map(resistiveSensorVal, RESISTIEF_DROOG_INTERVAL_MIN, RESISTIEF_NAT_INTERVAL_MAX, 0, 100);
    return String(val);
}

// Replaces placeholders with buttons or value section in your web page
String replacePlaceholdersInHtml(const String &var)
{
    if (var == "BUTTONPLACEHOLDER")
    {
        String buttons = "<input type=\"button\" value=\"Plant bewateren\" onclick=\"clickButton(this)\" >";
        return buttons;
    }
    else if (var == "BODEMVOCHTIGHEID")
    {
        return moistureLevel();
    }
    return String();
}

// Check Current wiFi status
bool checkWifi()
{
    Serial.println("Checking WiFi Status");
    unsigned long wifiReconnectTime = millis();
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

        Serial.println("Trying to reconnect to WiFi");
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
bool writeJson(String resStatus, String capStatus, int capacitive, int resistive, float temperature)
{
    // Get current timestamp
    timestamp = getTime();

    parentPath = databasePath + "/" + String(timestamp);

    // Prepare json
    json.set(resistivePath, resistive);
    json.set(capacitivePath, capacitive);
    json.set(capacitiveStatusPath, capStatus);
    json.set(resistiveStatusPath, resStatus);
    json.set(timePath, timestamp);
    json.set(temperaturePath, temperature);

    // Send json and capture error
    return Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json);
}

// Get status based on sensor values
void getStatus(int resisitve, int capacitive, String *resStatus, String *capStatus)
{
    // Logic that decides current moisture status
    // "Droog" - "Vochitg" - " Nat"

    if (resisitve >= RESISTIEF_DROOG_INTERVAL_MIN && resisitve <= RESISTIEF_DROOG_INTERVAL_MAX)
        *resStatus = "Droog";
    else if (resisitve >= RESISTIEF_VOCHTIG_INTERVAL_MIN && resisitve <= RESISTIEF_VOCHTIG_INTERVAL_MAX)
        *resStatus = "Vochtig";
    else if (resisitve >= RESISTIEF_NAT_INTERVAL_MIN && resisitve <= RESISTIEF_NAT_INTERVAL_MAX)
        *resStatus = "Nat";

    if (capacitive <= CAPACITIEF_DROOG_INTERVAL_MIN && capacitive >= CAPACITIEF_DROOG_INTERVAL_MAX)
        *capStatus = "Droog";
    else if (capacitive <= CAPACITIEF_VOCHTIG_INTERVAL_MIN && capacitive >= CAPACITIEF_VOCHTIG_INTERVAL_MAX)
        *capStatus = "Vochtig";
    else if (capacitive <= CAPACITIEF_NAT_INTERVAL_MIN && capacitive >= CAPACITIEF_NAT_INTERVAL_MAX)
        *capStatus = "Nat";
}

// Read moisture sensors values
void readSensors(int *resistive, int *capacitive)
{
    // Logic that reads moisture sensors
    *resistive = analogRead(RESISTIVE_SENSOR);
    *capacitive = analogRead(CAPACITIVE_SENSOR);
}

// Activate water pump for a duration based in soilstatus
void activatePump(String resStatus, String capStatus)
{
    if (resStatus == "Nat" || capStatus == "Nat")
        return;

    // Activate pump based on if ground is "Droog" or "Vochtig"
    unsigned long pumpTime;
    if (resStatus == "Droog" || capStatus == "Droog")
        pumpTime = 2000;
    else
        pumpTime = 1000;

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
    // get the ADC value from the temperature sensor
    int adcVal = analogRead(TEMPARETURE_SENSOR);
    // convert the ADC value to voltage in millivolt
    float milliVolt = adcVal * (5000.0 / 4095.0);
    // convert the voltage to the temperature in Celsius
    float tempC = milliVolt / 10;
    return tempC;
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

    // Setup Wifi access point
    setupWifiAccessPoint();

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", INDEX_HTML, replacePlaceholdersInHtml); });

    // Send a GET request to <ESP_IP>/update?output=<pinNumber>
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    if (request->hasParam(BUTTON_PARAM)) {
      Serial.println("Button Pressed");
      activatePump("Vochtig", "Vochtig");
    } else {
      Serial.println("No pinNumber parameter found.");
    }
    
    request->send(200, "text/plain", "OK"); });

    server.begin();

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
    if (Firebase.ready() && (millis() - startTime > TIMER_DELAY))
    {
        startTime = millis();

        // Read temperature tensor
        int temperatureValue = readTemperature();

        // Read and store Sensor values
        readSensors(&resistiveSensorVal, &capacitiveSensorVal);

        // Get status based on Sensor values and active pump
        String resistiveStatus, capacitiveStatus;
        getStatus(resistiveSensorVal, capacitiveSensorVal, &resistiveStatus, &capacitiveStatus);

        // Acticate pump if temperature is not below minimum
        if (temperatureValue > MINIMUM_TEMPERATUUR)
            activatePump(resistiveStatus, capacitiveStatus);

        // Finally send data to Firebase RTDB
        if (checkWifi())
            Serial.printf("Set json %s\n", writeJson(resistiveStatus, capacitiveStatus, capacitiveSensorVal, resistiveSensorVal, temperatureValue) ? "ok" : fbdo.errorReason().c_str());
        else
            Serial.println("Failed to send json.\nNot connected to WiFi");
    }
}