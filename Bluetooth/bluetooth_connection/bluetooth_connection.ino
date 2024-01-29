#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

BLECharacteristic *directionCharacteristic = NULL;

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DIRECTION_CHARACTERISTIC_UUID "f27b53ad-c63d-49a0-8c0f-9f297e6cc520"

#define LED_BUILTIN 2
#define LEFT_DIRECTION "L"
#define RIGHT_DIRECTION "R"


class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    Serial.println("Connected");
    digitalWrite(LED_BUILTIN, HIGH);
  }

  void onDisconnect(BLEServer *pServer) {
    Serial.println("Disconnected");
    digitalWrite(LED_BUILTIN, LOW);
  }
};

// Handles all characteristic callbacks for ble
class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  // when the client writes something to the server
  void onWrite(BLECharacteristic *pCharacteristic) {
    Serial.println("Something was written");
    String value = pCharacteristic->getValue().c_str();

    if (pCharacteristic == directionCharacteristic)
    {
      value = pCharacteristic->getValue().c_str();
      Serial.println();
      directionCharacteristic->setValue(const_cast<char *>(value.c_str()));
      directionCharacteristic->notify();
      Serial.println("Received a direction");

      if (value == LEFT_DIRECTION) {
        Serial.println("LEFT");
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
        delay(1000);                      // wait for a second
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
        delay(1000);                      // wait for a second
      } else if (value == RIGHT_DIRECTION) {
        Serial.println("RIGHT");
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
        delay(1000);                      // wait for a second
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
        delay(1000);                      // wait for a second
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
        delay(1000);                      // wait for a second
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
        delay(1000);                      // wait for a second
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  BLEDevice::init("BLEExample");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  directionCharacteristic = pService->createCharacteristic(
      DIRECTION_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_INDICATE);

  pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  pCharacteristic->setCallbacks(new CharacteristicCallbacks());

  directionCharacteristic->setCallbacks(new CharacteristicCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}