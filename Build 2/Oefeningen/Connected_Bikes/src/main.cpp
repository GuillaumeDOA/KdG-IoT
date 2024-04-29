#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <DFRobot_LIS2DW12.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "secrets.h"
#include "config.h"

DFRobot_LIS2DW12_I2C acce(&Wire, 0x19);
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

unsigned long startTime;

bool detectHole()
{
    // Accelerometer logic here
    return true;
}

int getPutdiepte(){
    // Putdiepte logic hier
    return 20;
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
    if (WiFi.status() == WL_CONNECTED)
        return true;

    return false;
}

void reconnectWifi()
{
    Serial.println("Reconnecting to network");
    WiFi.disconnect();
    WiFi.reconnect();
}

bool sendToGoogleSheet(double lat, double lng, int putDiepte)
{
    String urlFinal = GOOGLE_APPS_SCRIPT_URL + GOOGLE_SCRIPT_ID + "/exec?latitude=" + lat + "&longitude=" + lng + "&putdiepte=" + putDiepte;
    HTTPClient http;
    http.begin(urlFinal.c_str());
    int httpCode = http.GET();

    Serial.printf("HTTP Status Code: %d\n", httpCode);


    http.end();

    if(httpCode >= 200 && httpCode < 300)
        return true;
    return false;
}

bool gpsValid()
{
    if (!(gpsSerial.available() > 0))
        return false;

    if (!gps.encode(gpsSerial.read()))
        return false;

    if (!gps.location.isValid())
        return false;

    return true;
}

void setup()
{
    Serial.begin(9600);

    // Initialise gpsSerial
    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    // Initialise Wifi
    initWifi();

    startTime = millis();
}

void loop()
{
    // als de wachttijd nog niet verstreken is -> doe niets
    if (millis() - startTime < TIMER_DELAY)
        return;

    // als er geen activiteit werd gedetecteerd -> doe niets
    if (!acce.actDetected())
        return;

    // lees accelerometer en als er geen put gedetecteerd werd -> doe niets
    if (!detectHole())
        return;

    // lees gps waardes uit
    if (!gpsValid())
        return;

    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    // als er geen wifi connectie is -> reconnect wifi en doe niets
    if (!checkWifi())
    {
        reconnectWifi();
        return;
    }

    // Stuur data door naar Google Sheets
    Serial.printf("Sending Data to Google Sheets... %s \n", sendToGoogleSheet(latitude, longitude, getPutdiepte()) ? "Succesful" : " Failed");
}
