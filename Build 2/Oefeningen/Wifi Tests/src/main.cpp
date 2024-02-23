#include <Arduino.h>
#include <Wifi.h>

const char* SSID = "Jouw papa";
const char* PASSWORD = "esp32build";

unsigned long startTime;

void setup() {
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);
  
  while( WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to network");
  }

  Serial.println("Connected to network!");

  startTime = millis();
}

void loop() {
  if ((WiFi.status() != WL_CONNECTED) && (millis() - startTime >= 1000)){
    Serial.println("Reconnecting to network");
    WiFi.disconnect();
    WiFi.reconnect();
    startTime = millis();
  }
}
