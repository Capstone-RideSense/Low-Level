#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Constants.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define DIRECTION_CHARACTERISTIC_UUID "f27b53ad-c63d-49a0-8c0f-9f297e6cc520"

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer); 
  void onDisconnect(BLEServer *pServer);
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  // when the client writes something to the server
  void onWrite(BLECharacteristic *pCharacteristic);
};