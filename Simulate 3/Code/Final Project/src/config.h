// Sensor Read Times
const unsigned long DOORDELAY = 7000;
const unsigned long SENSORDELAY = 5000;
const unsigned long NFCDELAY = 2000;

// MQTT Topics
const char *MQTT_NFC =            "intellihome/nfc/lock";
const char *MQTT_TEMP =           "intellihome/scd4x/temperature";
const char *MQTT_CO2 =            "intellihome/scd4x/co2";
const char *MQTT_HUMIDITY =       "intellihome/scd4x/humidity";
const char *MQTT_MOTION =         "intellihome/motion/send";
const char *MQTT_CURRENT =        "intellihome/current";
const char *MQTT_WINDOWRESPOSNE = "intellihome/window/response";
const char *MQTT_DOORRESPONSE =   "intellihome/door/response";