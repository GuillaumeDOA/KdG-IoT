#include <Arduino.h>

String PASSWORD = "KdGIoT0!";
String SSID = "IoT";
IPAddress RPI_ADDRESS = IPAddress(10,6,121,188);
int MQTT_PORT = 1883;
const char* MQTT_TEMP_TOPIC = "esp/LM35/temperature";
const char* MQTT_LED_TOPIC = "esp/led";
