#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include "Sensors/Blindspot.hpp"

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    // Bluetooth Initialization
    BLE_Setup();

    // ToF Initialization
    blindspot_setup();
}

void loop()
{
    // Get measurements
    blindspot_detect();

    // Give some delay
    delay(100);
}
