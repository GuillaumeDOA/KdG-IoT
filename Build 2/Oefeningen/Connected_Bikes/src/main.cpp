#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <DFRobot_LIS2DW12.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <time.h>
#include "secrets.h"
#include "config.h"

DFRobot_LIS2DW12_I2C acce(&Wire, 0x19);
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

unsigned long startTime;

void setTimezone(String timezone){
    Serial.printf("Setting Timezone to %s\n", timezone.c_str());
    setenv("TZ", timezone.c_str(), 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
    tzset();
}

String getCurrentDateAndTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return "";
    }
    
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    
    String asString(timeStringBuff);
    asString.replace(" ", "-");

    return asString;
}


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

bool sendToGoogleSheet(String timestamp, double lat, double lng, int putDiepte)
{
    String urlFinal = GOOGLE_APPS_SCRIPT_URL + GOOGLE_SCRIPT_ID + "/exec?time=" + timestamp + "&latitude=" + lat + "&longitude=" + lng + "&putdiepte=" + putDiepte;
    HTTPClient http;
    http.begin(urlFinal.c_str());
    int httpCode = http.GET();

    Serial.printf("HTTP Status Code: %d\n", httpCode);

    http.end();

    if (httpCode >= 200 && httpCode < 300)
        return true;
    return false;
}

void readGPS(double *lat, double *lng)
{
    /*
        Wou absoluut geen while loop gebruiken in de main functie, maar dit is de enige manier
        waarop ik de GPS kon laten werken. Heb meerdere uren problemen hiermee gehad, 
        forums gelezen maar niets wou werken. Vandaar de while.
     */
    bool reading = true;
    while (reading)
        while (gpsSerial.available() > 0)
        {
            if (gps.encode(gpsSerial.read()))
            {
                if (gps.location.isValid())
                {
                    *lat = gps.location.lat();
                    *lng = gps.location.lng();
                    reading = false;
                }
            }
        }
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

    // Setup NTP
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    setTimezone("CET-1CEST,M3.5.0,M10.5.0/3");  

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
    /* 
        Heb de feedback over de If-Else structuren gelezen en heb besloten om ze zo te houden.
        Dit is voor mij duidelijker dan maar steeds dieper te gaan nesten.
        #NeverNester -> https://www.youtube.com/watch?v=CFRhGnuXG-4 (zeer interessant als u nog niet overtuigt bent)
     */


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

    // lees gps waardes uit
    double latitude = 0;
    double longitude = 0;
    readGPS(&latitude, &longitude);

    Serial.printf("Latitude: %f, Longitutde: %f\n", latitude, longitude);

    // als er geen wifi connectie is -> reconnect wifi en doe niets
    if (!checkWifi())
    {
        reconnectWifi();
        return;
    }

    // Get Time
    String timeStamp = getCurrentDateAndTime();

    // Stuur data door naar Google Sheets
    Serial.printf("Sending Data to Google Sheets... %s \n\n", sendToGoogleSheet(timeStamp, latitude, longitude, putDiepte) ? "Succesful" : " Failed");
    startTime = millis();
}
