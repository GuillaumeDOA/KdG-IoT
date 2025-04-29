// Device Configuration
const char *DEVICE_NAME="Server Room";

// Timers
const unsigned long LOCK_TIMER = 5000;
const unsigned long HALLSENSOR_DELAY = 1000;
const unsigned long NFC_READ_DELAY = 1000;
const unsigned long NFC_WRITE_DELAY = 5000;

// MQTT Configuration
const char *MQTT_ADDRESS = "172.201.156.113";
const int MQTT_PORT = 1883;

/* MQTT UI Password: StrongUIP@ssword
   DB Password: StrongDBP@ssword */
   
// MQTT Topics
const char *MQTT_READ = "nfc/scan";
const char *MQTT_WRITE = "nfc/write";
const char *MQTT_RESULT = "nfc/result";
const char *MQTT_HALLSENSOR = "hallsensor/read";

// NFC Configuration
#define BLOCK_SIZE 16
#define PN532_IRQ 2
#define POLLING 0
#define BLOCK_NO 2