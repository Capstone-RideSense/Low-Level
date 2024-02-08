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

bool is_valid_intensity(char intensity_ch) {
  // cast char to int
  int intensity = intensity_ch - '0';

  //check that the intensity is in our options
  if (intensity > INTENSITY_MAX || intensity < INTENSITY_MIN) {
    Serial.print("The given intensity of ");
    Serial.print(intensity);
    Serial.print(" is invalid.");
    return false;
  }
  return true;
}

// when the client writes something to the server
void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  // check that the written info is a direction characteristic
  if (pCharacteristic->getUUID().toString() == DIRECTION_CHARACTERISTIC_UUID)
  {
    std::string instructions = pCharacteristic->getValue();
    Serial.println();
    pCharacteristic->setValue(instructions);
    pCharacteristic->notify();
    Serial.print("The direction written: ");
    Serial.println(instructions.c_str());

    char direction;
    char intensity;

    if (instructions.size() != 2) {
      Serial.println("ERROR: given direction and intensity are in an invalid format");
      return;
    }

    direction = instructions[0];
    intensity = instructions[1];

    //check that intensity is valid
    if (!is_valid_intensity(intensity)) {
      return;
    }
    
    // send haptic motor the direction and intensity. It will determine if the direction is valid
    haptic_write(direction, intensity - '0');
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