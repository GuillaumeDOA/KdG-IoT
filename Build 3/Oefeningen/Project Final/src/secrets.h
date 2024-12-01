#include <Arduino.h>

String SSID = "Home de Oliveira - Haesaert";
String PASSWORD = "0475212120";
IPAddress RPI_ADDRESS = IPAddress(192,168,1,26);
int MQTT_PORT = 1883;
const char* MQTT_TEMP_TOPIC = "esp/LM35/temperature";
const char* MQTT_LED_TOPIC = "esp/led";
