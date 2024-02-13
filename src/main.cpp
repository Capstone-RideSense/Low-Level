#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include "Sensors/Blindspot.hpp"

// void setup() {
//   Serial.begin(115200);
//   pinMode(LED_BUILTIN, OUTPUT);

//   // Bluetooth Initialization
//   BLE_Setup();

// }

// void loop() {
//   delay(2000);
// }

/* Includes ------------------------------------------------------------------*/
// #include <Arduino.h>
// #include <Wire.h>
// #include <vl53l4cx_class.h>
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <assert.h>
// #include <stdlib.h>

/* Setup ---------------------------------------------------------------------*/

void setup()
{
  // Led.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize serial for output.
  Serial.begin(115200);
  Serial.println("Starting...");

  // Initialize time of flight sensor
  blindspot_setup();
}

void loop()
{
  blindspot_detect();
}
