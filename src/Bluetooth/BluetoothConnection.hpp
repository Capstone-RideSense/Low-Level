#ifndef BLUETOOTHCONNECTION_HPP
#define BLUETOOTHCONNECTION_HPP

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
// #include <regex>
#include "Constants.hpp"

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer); 
  void onDisconnect(BLEServer *pServer);
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic);
};

void BLE_Setup();

#endif //BLUETOOTHCONNECTION_HPP