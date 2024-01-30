#include "Bluetooth/BluetoothConnection.hpp"
#include "Sensors/HapticMotors.hpp"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// when a client connects to bluetooth
void ServerCallbacks::onConnect(BLEServer *pServer) {
  Serial.println("Connected");
  digitalWrite(LED_BUILTIN, HIGH);
  pServer->startAdvertising();
}

// when a client disconnects from bluetooth
void ServerCallbacks::onDisconnect(BLEServer *pServer) {
  Serial.println("Disconnected");
  digitalWrite(LED_BUILTIN, LOW);
  pServer->startAdvertising();
}

// when the client writes something to the server
void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  Serial.println("Something was written");
  String direction = pCharacteristic->getValue().c_str();

  // check that the written info is a direction characteristic
  if (pCharacteristic->getUUID().toString() == DIRECTION_CHARACTERISTIC_UUID)
  {
    direction = pCharacteristic->getValue().c_str();
    Serial.println();
    pCharacteristic->setValue(const_cast<char *>(direction.c_str()));
    pCharacteristic->notify();
    Serial.print("The direction written: ");
    Serial.println(direction);

    haptic_write(direction);
  }
}

void BLE_Setup() {
  BLEDevice::init("BLEExample");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *directionCharacteristic = pService->createCharacteristic(
      DIRECTION_CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_INDICATE);

  directionCharacteristic->setCallbacks(new CharacteristicCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}