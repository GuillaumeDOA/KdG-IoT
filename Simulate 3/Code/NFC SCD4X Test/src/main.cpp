#include <Arduino.h>
#include <DFRobot_PN532.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>

#define BLOCK_SIZE 16
#define PN532_IRQ 2
#define INTERRUPT 1
#define POLLING 0

// The block to be read
#define READ_BLOCK_NO 2

DFRobot_PN532_IIC nfc(PN532_IRQ, POLLING);
uint8_t dataRead[16] = {0};

SensirionI2CScd4x scd4x;
uint16_t error;
char errorMessage[256];
unsigned long nfcDelay, scd4xDelay;

void printUint16Hex(uint16_t value)
{
  Serial.print(value < 4096 ? "0" : "");
  Serial.print(value < 256 ? "0" : "");
  Serial.print(value < 16 ? "0" : "");
  Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2)
{
  Serial.print("Serial: 0x");
  printUint16Hex(serial0);
  printUint16Hex(serial1);
  printUint16Hex(serial2);
  Serial.println();
}

void initSCD4X()
{
  Serial.print("Inittializing SCD4X\n");
  scd4x.begin(Wire);

  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  uint16_t serial0;
  uint16_t serial1;
  uint16_t serial2;
  error = scd4x.getSerialNumber(serial0, serial1, serial2);
  if (error)
  {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    exit(1);
  }
  else
  {
    printSerialNumber(serial0, serial1, serial2);
    Serial.println("SCD4X Sensor Ready");
  }

  error = scd4x.startPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
}

void initNFC()
{
  Serial.print("Initializiing NFC\n");
  while (!nfc.begin())
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" NFC ready to accept cards");
}

void readNFC()
{
  if (nfc.scan())
  {
    if (nfc.readData(dataRead, READ_BLOCK_NO) != 1)
    {
      Serial.print("Block ");
      Serial.print(READ_BLOCK_NO);
      Serial.println(" read failure!");
    }
    else
    {
      Serial.print("Block ");
      Serial.print(READ_BLOCK_NO);
      Serial.println(" read success!");

      Serial.print("Data read(string):");
      Serial.println((char *)dataRead);
      String readConverted;
      for (int i = 0; i < BLOCK_SIZE; i++)
      {
        readConverted += dataRead[i];
      }

      Serial.println(readConverted);
    }
    nfcDelay = millis();
  }
}

void readSCD4X()
{
  uint16_t co2;
  float temperature;
  float humidity;

  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error)
  {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  else if (co2 == 0)
  {
    Serial.println("Invalid sample detected, skipping.");
  }
  else
  {
    Serial.print("Co2:");
    Serial.print(co2);
    Serial.print(" ppm");
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.print(" °C");
    Serial.print("\t");
    Serial.print("Humidity:");
    Serial.print(humidity);
    Serial.println(" %");
  }
}


void setup()
{
  Wire.begin();
  Serial.begin(115200);
  delay(50);

  Serial.println("Initializing System\n");

  initSCD4X();
  initNFC();

  nfcDelay, scd4xDelay = millis();
}

void loop()
{
  if (millis() - nfcDelay >= 2000)
  {
    readNFC();
  }

  if (millis() - scd4xDelay >= 5000)
  {
    readSCD4X();
    scd4xDelay = millis();
  }
}