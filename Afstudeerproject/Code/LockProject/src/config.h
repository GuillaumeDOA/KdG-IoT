// Device Configuration
const char *DEVICE_NAME="Server Room";

// Timers
const unsigned long LOCK_TIMER = 5000;
const unsigned long HALLSENSOR_DELAY = 2000;
const unsigned long NFC_READ_DELAY = 1000;
const unsigned long NFC_WRITE_DELAY = 10000;

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

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
const unsigned long DISPLAY_UPDATE_INTERVAL = 5000;