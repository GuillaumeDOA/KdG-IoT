#include "WiFi.h"
#include "HTTPClient.h"
#include "time.h"

String WIFI_SSID = "IoT";
String WIFI_PASSWORD = "KdGIoT43!";

// NTP = Network Time Protocol (
const char* NTP_SERVER = "pool.ntp.org";
const long  GMT_OFFSET_SEC = 0; //19800;
const int   DAYLIGHT_OFFSET_SEC = 0;

// Google Apps Script URL
const String GOOGLE_APPS_SCRIPT_URL = "https://script.google.com/macros/s/";
String GOOGLE_SCRIPT_ID = "";

// Keep track of number of requests
int requestCounter = 0;

void initWifi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  Serial.flush();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void setTimezone(String timezone){
  Serial.printf("Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void setup() {
  Serial.begin(115200);

  // connect to WiFi
  initWifi();

  // Init NTP and set timezone to Berlin
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  setTimezone("CET-1CEST,M3.5.0,M10.5.0/3");  
}

String getCurrentDateAndTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }
  
  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%d/%m/%Y %H:%M", &timeinfo);
  
  String asString(timeStringBuff);
  asString.replace(" ", "-");

  return asString;
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Get current date and time
    String currentDateAndTime = getCurrentDateAndTime();
    Serial.print("Current date and time: ");
    Serial.println(currentDateAndTime);

    // Create URL with parameters to call Google Apps Script
    String status ="";
    if(requestCounter%3 == 0)
      status = "DIEP";
    else
      status = "ONDIEP";

    String urlFinal = GOOGLE_APPS_SCRIPT_URL + GOOGLE_SCRIPT_ID + "/exec?longitude=4.4161392444414345&latitude=51.24622103637487&putdiepte=" + status+"&putvalue="+requestCounter;
    Serial.print("POST data to spreadsheet: ");
    Serial.println(urlFinal);
    
    // Send HTTP request
    HTTPClient http;
    http.begin(urlFinal.c_str());
    //http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    
    // Get response from HTTP request
    String payload;
    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: " + payload);
    }
    http.end();
  }

  // Increase request counter
  requestCounter++;

  // Wait 5 seconds between calls
  delay(5000);
}