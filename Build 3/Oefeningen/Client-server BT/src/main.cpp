#include <Arduino.h>

/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
  Ported to Arduino ESP32 by Evandro Copercini


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; // in seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults *foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices->getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
} */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// UUIDs for service and characteristic
#define SERVICE_UUID "0af115cc-3a36-4a2d-ae32-9e57bfeb2a0d"
#define CHARACTERISTIC_UUID "272b9c5c-3264-4622-a5d3-5b068b3bb5ee"

BLECharacteristic *pCharacteristic;
unsigned long startTime;
int characteristicValue = 23; // Starting value for characteristic

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize BLE device and server
  BLEDevice::init("ESP-Guillaume");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE characteristic with read/write properties
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ);

  // Set initial value of the characteristic
  pCharacteristic->setValue(String(characteristicValue).c_str());
  pService->start();

  // Start advertising the BLE service
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // Helps with iPhone connection issues
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Characteristic defined! Now you can read it on your phone!");
  
  startTime = millis();
}

void loop()
{
  // Check if 5 seconds have passed
  if (millis() - startTime >= 5000)
  {
    startTime = millis(); // Reset the timer

    // Update characteristic value
    characteristicValue += 1; // Increment the value
    pCharacteristic->setValue(String(characteristicValue).c_str()); // Set new value as string
    
    Serial.print("Characteristic updated to: ");
    Serial.println(characteristicValue);
  }
}
