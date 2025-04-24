// Device Configuration
const char *DEVICE_NAME="Server Room";

// MQTT Configuration
const char *MQTT_ADDRESS = "172.201.156.113";
const int MQTT_PORT = 1883;

/* MQTT UI Password: StrongUIP@ssword
   DB Password: StrongDBP@ssword */
   
// MQTT Topics
const char *MQTT_READ = "nfc/scan";

// NFC Configuration
#define BLOCK_SIZE 16
#define PN532_IRQ 2
#define POLLING 0
#define READ_BLOCK_NO 2
const unsigned long NFC_DELAY = 1000;