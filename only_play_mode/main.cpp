#include<Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#define b1_PIN 13
#define b2_PIN 12
#define b3_PIN 25
#define b4_PIN 33

#define temperatureCelsius
#define bleServerName "ESP32_BLE"
unsigned long lastTime=0;

    bool deviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// Temperature Characteristic and Descriptor
#ifdef temperatureCelsius
// BLECharacteristic bmeTemperatureCelsiusCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
// BLEDescriptor bmeTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));
#else
BLECharacteristic bmeTemperatureFahrenheitCharacteristics("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeTemperatureFahrenheitDescriptor(BLEUUID((uint16_t)0x2902));
#endif

// Humidity Characteristic and Descriptor
BLECharacteristic bmeHumidityCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeHumidityDescriptor(BLEUUID((uint16_t)0x2903));

// Setup callbacks onConnect and onDisconnect
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



void setup()
{
  // Start serial communication
  Serial.begin(115200);
  pinMode(b1_PIN,INPUT_PULLUP);
  pinMode(b2_PIN, INPUT_PULLUP);
  pinMode(b3_PIN, INPUT_PULLUP);
  pinMode(b4_PIN, INPUT_PULLUP);

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *bmeService = pServer->createService(SERVICE_UUID);

// Create BLE Characteristics and Create a BLE Descriptor

  // Humidity
  bmeService->addCharacteristic(&bmeHumidityCharacteristics);
  bmeHumidityDescriptor.setValue("BME humidity");
  bmeHumidityCharacteristics.addDescriptor(new BLE2902());

  // Start the service
  bmeService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop()
{
  int a=digitalRead(b1_PIN);
  int b=digitalRead(b2_PIN);
  int c=digitalRead(b3_PIN);
  int d=digitalRead(b4_PIN);
  if (deviceConnected)
  {
    if (a == 0)
    {
      bmeHumidityCharacteristics.setValue("1");
      bmeHumidityCharacteristics.notify();
      delay(200);
    }
    if (b == 0)
    {
      bmeHumidityCharacteristics.setValue("2");
      bmeHumidityCharacteristics.notify();
      delay(200);
    }
    if (c == 0)
    {
      bmeHumidityCharacteristics.setValue("3");
      bmeHumidityCharacteristics.notify();
      delay(200);
    }
    if (d == 0)
    {
      bmeHumidityCharacteristics.setValue("4");
      bmeHumidityCharacteristics.notify();
      delay(200);
    }

  
    // if ((millis() - lastTime) > 1000)
    // {
    //   // Set humidity Characteristic value and notify connected client
    //   bmeHumidityCharacteristics.setValue("1");
    //   bmeHumidityCharacteristics.notify();
   
    //   Serial.println("B1 : "+(String)digitalRead(b1_PIN));
    //   Serial.println("B2 : " + (String)digitalRead(b2_PIN));
    //   Serial.println("B3 : " + (String)digitalRead(b3_PIN));
    //   Serial.println("B4 : " + (String)digitalRead(b4_PIN));
    //   lastTime = millis();
    // }
  }
}