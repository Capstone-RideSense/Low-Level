#include <BluetoothConnection.h>
#include <HapticMotors.h>
// #include <HapticMotors.h>
// #include <UltrasonicSensors.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

BLECharacteristic *directionCharacteristic = NULL;

void ServerCallbacks::onConnect(BLEServer *pServer) {
  Serial.println("Connected");
  digitalWrite(LED_BUILTIN, HIGH);
}

void ServerCallbacks::onDisconnect(BLEServer *pServer) {
  Serial.println("Disconnected");
  digitalWrite(LED_BUILTIN, LOW);
}

void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  Serial.println("Something was written");
  String direction = pCharacteristic->getValue().c_str();

  if (pCharacteristic == directionCharacteristic)
  {
    direction = pCharacteristic->getValue().c_str();
    Serial.println();
    directionCharacteristic->setValue(const_cast<char *>(direction.c_str()));
    directionCharacteristic->notify();
    Serial.print("The direction written: ");
    Serial.println(direction);

    haptic_write(direction);

    // if (direction == LEFT_DIRECTION) {
      
    //   Serial.println("LEFT");
    //   digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
    //   delay(1000);                      // wait for a second
    //   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
    //   delay(1000);                      // wait for a second
    // } else if (direction == RIGHT_DIRECTION) {
    //   Serial.println("RIGHT");
    //   digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
    //   delay(1000);                      // wait for a second
    //   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
    //   delay(1000);                      // wait for a second
    //   digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
    //   delay(1000);                      // wait for a second
    //   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
    //   delay(1000);                      // wait for a second
    // }
  }
}


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