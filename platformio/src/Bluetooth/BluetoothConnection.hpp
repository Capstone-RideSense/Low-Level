#ifndef BLUETOOTHCONNECTION_HPP
#define BLUETOOTHCONNECTION_HPP

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "Constants.hpp"

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer); 
  void onDisconnect(BLEServer *pServer);
};

class CharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic);
};

#endif //BLUETOOTHCONNECTION_HPP