#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Bluetooth Initialization
  BLE_Setup();

}

void loop() {
  delay(2000);
}