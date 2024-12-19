#include <Arduino.h>

String PASSWORD = "KdGIoT0!";
String SSID = "IoT";
const char *RPI_ADDRESS = "10.6.121.207";
int MQTT_PORT = 1883;
const char *MQTT_NFC =       "intellihome/nfc/lock";
const char *MQTT_TEMP =      "intellihome/scd4x/temperature";
const char *MQTT_CO2 =       "intellihome/scd4x/co2";
const char *MQTT_HUMIDITY =  "intellihome/scd4x/humidity";