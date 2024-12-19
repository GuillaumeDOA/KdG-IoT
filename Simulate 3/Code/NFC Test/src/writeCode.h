#include <Arduino.h>
#include <DFRobot_PN532.h>

#define  BLOCK_SIZE         16
#define  PN532_IRQ          2
#define  INTERRUPT          1
#define  POLLING            0
// The block to be written
#define  WRITE_BLOCK_NO      2

DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);

uint8_t dataWrite[BLOCK_SIZE] = {"Secure Password"};

void setup() {
  Serial.begin(115200);
  Serial.print("Initializing");
  while (!nfc.begin()) {
    Serial.print(".");
    delay (1000);
  }
  Serial.println();
  Serial.println("Waiting for a card......");
}
void loop() {
  // For S50 card/tag, block 1-2, 4-6, 8-10, 12-14... 56-58, 60-62 are for user data
  // You can read/write these blocks freely.
  // Use "MifareClassic_ReadAllMemory.ino" to check all the blocks
  if (nfc.scan()) {
    if (nfc.writeData(WRITE_BLOCK_NO, dataWrite) != 1) {
      Serial.print("Block ");
      Serial.print(WRITE_BLOCK_NO);
      Serial.println(" write failure!");
    }
    else {
      Serial.print("Block ");
      Serial.print(WRITE_BLOCK_NO);
      Serial.println(" write success!");
      Serial.print("Data written(string):");
      Serial.println((char *)dataWrite);
      Serial.print("Data written(HEX):");
      for (int i = 0; i < BLOCK_SIZE; i++) {
        Serial.print(dataWrite[i], HEX);
        Serial.print(" ");
      }
    }
  }
  delay(500);
}