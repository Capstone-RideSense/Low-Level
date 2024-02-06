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

int findFirstOccurrence(int* arr, size_t arr_size, int target) {

  for (int ii = 0; ii < arr_size; ii++) {
    if (arr[ii] == target) {
      return ii;
    }
  }
  return -1;
}

bool is_valid_intensity(std::string intensity_str) {
  int intensity;
  // check that it is a valid int
  try {
    intensity = stoi(intensity_str);
  } catch (std::invalid_argument& e) {
    Serial.print("Not a valid int");
    return false;
  }
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
  std::string direction = pCharacteristic->getValue();

  // check that the written info is a direction characteristic
  if (pCharacteristic->getUUID().toString() == DIRECTION_CHARACTERISTIC_UUID)
  {
    direction = pCharacteristic->getValue();
    Serial.println();
    pCharacteristic->setValue(const_cast<char *>(direction.c_str()));
    pCharacteristic->notify();
    Serial.print("The direction written: ");
    Serial.println(direction.c_str());

    std::string dir;
    std::string intensity_str;
    int intensity = -1;

    std::string intensity_options = "";
    for (int ii = INTENSITY_MIN; ii <= INTENSITY_MAX; ii++) {
      intensity_options += std::to_string(ii);
    }

    size_t digitPos = direction.find_first_of(intensity_options);
    if (digitPos == std::string::npos) {
      Serial.println("Given direction has an invalid intensity");
      return;
    }
    dir = direction.substr(0, digitPos);
    intensity_str = direction.substr(digitPos);

    // make sure given information is valid
    if (!is_valid_intensity(intensity_str)) {
      return;
    }
    
    // send haptic motor the direction and intensity. It will determine if the direction is valid
    haptic_write(dir, stoi(intensity_str));
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