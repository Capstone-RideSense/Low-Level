#include "Bluetooth/BluetoothConnection.hpp"
#include "Sensors/HapticMotors.hpp"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// when a client connects to bluetooth
void ServerCallbacks::onConnect(BLEServer *pServer) {
  Serial.println("Connected");
  digitalWrite(LED_BUILTIN, HIGH);
}

// when a client disconnects from bluetooth
void ServerCallbacks::onDisconnect(BLEServer *pServer) {
  Serial.println("Disconnected");
  digitalWrite(LED_BUILTIN, LOW);
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