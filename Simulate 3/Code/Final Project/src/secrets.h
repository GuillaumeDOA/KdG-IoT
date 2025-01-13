#include <Arduino.h>

String PASSWORD = "KdGIoT0!";
String SSID = "IoT";
const char *RPI_ADDRESS = "109.137.19.53";
int MQTT_PORT = 1883;
const char *MQTT_NFC =            "intellihome/nfc/lock";
const char *MQTT_TEMP =           "intellihome/scd4x/temperature";
const char *MQTT_CO2 =            "intellihome/scd4x/co2";
const char *MQTT_HUMIDITY =       "intellihome/scd4x/humidity";
const char *MQTT_MOTION =         "intellihome/motion/send";
const char *MQTT_CURRENT =        "intellihome/current";
const char *MQTT_MOTIONRESPONSE = "intellihome/motion/response";
const char *MQTT_WINDOWRESPOSNE = "intellihome/window/response";
const char *MQTT_DOORRESPONSE =   "intellihome/door/response";