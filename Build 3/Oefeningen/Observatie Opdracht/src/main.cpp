/*
I have used code from RandomNerdTutorials.com and ChatGPT for this project
for the html portion of the project
- OpenAI (2024) - ChatGPT - https://chat.openai.com
- https://RandomNerdTutorials.com/esp32-web-bluetooth (08/10/2024)
*/

#include <Arduino.h>
#include <ESP32Servo.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "9f122ef6-f322-40f2-9bf7-d418a563f998"
#define DOOR_CHARACTERISTIC_UUID "780f36a0-dbe1-4acc-ade1-f0edd96ecd0c"

BLEServer *pServer = NULL;
BLECharacteristic *pDoorCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
unsigned long startTime;
Servo door;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pDoorCharacteristic)
  {
    Serial.println("LED Characteristic");
    std::string doorvalue = pDoorCharacteristic->getValue();
    String value = String(doorvalue.c_str());
    if (value.length() > 0)
    {
      Serial.print("Characteristic event, written: ");
      Serial.println(static_cast<int>(value[0])); // Print the integer value

      int receivedValue = static_cast<int>(value[0]);
      if (receivedValue == 1)
      {
        door.write(0);
      }
      else
      {
        door.write(65);
      }
    }
  }
};

void setup()
{
  Serial.begin(115200);
  door.attach(D10);

  // Create the BLE Device
  BLEDevice::init("ESP-Guillaume");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create the OPEN button Characteristic
  pDoorCharacteristic = pService->createCharacteristic(
      DOOR_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_INDICATE);

  // Register the callback for the OPEN button characteristic
  pDoorCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pDoorCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
  startTime = millis();
}

bool doorState = true;

void loop()
{
  if (deviceConnected)
  {
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    Serial.println("Device disconnected.");
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("Start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
    Serial.println("Device Connected");
  }
  if (millis() - startTime >= 5000)
  {
    if (doorState)
    {
      door.write(0);
      doorState = false;
      pDoorCharacteristic->setValue(String(1).c_str());
      pDoorCharacteristic->notify();
    }
    else
    {
      door.write(65);
      doorState = true;
      pDoorCharacteristic->setValue(String(2).c_str());
      pDoorCharacteristic->notify();
    }
    startTime = millis();
  }
}
