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

bool detectHole(int *putDiepte)
{
    // Accelerometer logic here
    int x = abs(acce.readAccX());
    int y = abs(acce.readAccY());
    int z = abs(acce.readAccZ());

    printf("Measuring hole: x: %d mg, y: %d mg, z: %d mg\n", x, y, z);
    if (x >= ACCELEROMETER_THRESHOLD || y >= ACCELEROMETER_THRESHOLD || z >= ACCELEROMETER_THRESHOLD)
    {
        *putDiepte = map(max(max(x, y), z), 5000, 14000, 1, 20);
        return true;
    }
    
    return false;
}

void initWifi()
{
    Serial.print("Connecting to WiFi: ");
    Serial.print(WIFI_SSID);
    Serial.flush();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");
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

    if (httpCode >= 200 && httpCode < 300)
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

void initAccelerometer()
{
    while (!acce.begin())
    {
        Serial.println("Communication with accelerometer failed... ");
        delay(1000);
    }
    Serial.println("Accelerometer connected");
    // Software reset
    acce.softReset();
    acce.setRange(DFRobot_LIS2DW12::e16_g);
    acce.setFilterPath(DFRobot_LIS2DW12::eLPF);
    acce.setFilterBandwidth(DFRobot_LIS2DW12::eRateDiv_4);
    acce.setWakeUpDur(/*dur = */ 3);
    acce.setWakeUpThreshold(/*threshold = */ 0.3);
    acce.setPowerMode(DFRobot_LIS2DW12::eContLowPwrLowNoise1_12bit);
    acce.setActMode(DFRobot_LIS2DW12::eDetectAct);
    acce.setInt1Event(DFRobot_LIS2DW12::eWakeUp);
    acce.setDataRate(DFRobot_LIS2DW12::eRate_200hz);
    delay(100);
}

void setup()
{
    delay(500);
    Serial.begin(115200);

    // Initialise Accelerometer
    initAccelerometer();

    // Initialise gpsSerial
    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

    // Initialise Wifi
    initWifi();

    startTime = millis();
}

void loop()
{
    int putDiepte;

    // als de wachttijd nog niet verstreken is -> doe niets
    if (millis() - startTime < TIMER_DELAY)
        return;

    // als er geen activiteit werd gedetecteerd -> doe niets
    if (!acce.actDetected())
        return;

    // lees accelerometer en als er geen put gedetecteerd werd -> doe niets
    if (!detectHole(&putDiepte))
        return;

    Serial.println("Put diep genoeg");

    /* // lees gps waardes uit
    if (!gpsValid())
        return;
     */
    Serial.println("GPS locatie valid");

    double latitude = 30;  // gps.location.lat();
    double longitude = 50; // gps.location.lng();

    // als er geen wifi connectie is -> reconnect wifi en doe niets
    if (!checkWifi())
    {
        reconnectWifi();
        return;
    }

    // Stuur data door naar Google Sheets
    Serial.printf("Sending Data to Google Sheets... %s \n\n", sendToGoogleSheet(latitude, longitude, putDiepte) ? "Succesful" : " Failed");
    startTime = millis();
}
