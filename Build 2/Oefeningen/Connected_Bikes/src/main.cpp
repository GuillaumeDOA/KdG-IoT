#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <DFRobot_LIS2DW12.h>
#include "secrets.h"

DFRobot_LIS2DW12_I2C acce(&Wire, 0x19);

unsigned long startTime;

bool detectHole()
{
    // Accelerometer logic here
    return true;
}

void initWifi()
{
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);
    Serial.flush();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
}

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

bool sendToSheet()
{
    return true;
}

void setup()
{
    Serial.begin(9600);

    initWifi();

    startTime = millis();
}

void loop()
{
    if (!acce.actDetected() && !(millis() - startTime >= 3000))
        return;

    if (!detectHole())
        return;
    
    if (checkWifi())
        Serial.printf("Sending Data to Google Sheets... %s \n", sendToSheet() ? "Succesful" : " Failed");
}
