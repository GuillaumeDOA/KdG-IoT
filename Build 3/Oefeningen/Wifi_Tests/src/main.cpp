#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  int networks = WiFi.scanNetworks();
  Serial.printf("Networks Found: %i\n", networks);
  for (int i = 0; i < networks; i++)
  {
    Serial.printf("%s (%i)\n", WiFi.SSID(i), WiFi.RSSI(i));
  }
  Serial.printf("\n");
}
