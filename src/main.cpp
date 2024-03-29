#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include <Wire.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"
#include "Sensors/IMU.hpp"

#define DEBUG_MODE 1

void setup()
{
    Serial.begin(115200);
    Serial.println("I2C begin");
    DEV_I2C.begin();
    Serial.println("pin setup");
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(XSHUT_LEFT, OUTPUT);
    pinMode(BLINKER_BUTTON_LEFT, INPUT);

    // TODO: enable right
    // pinMode(XSHUT_RIGHT, OUTPUT);
    // pinMode(BLINKER_BUTTON_RIGHT, INPUT);

    // Bluetooth Initialization
    Serial.println("BLE setup");
    BLE_Setup();

    // ToF Initialization
    Serial.println("Blindspot setup");
    blindspot_setup();

    // LEDDriver Initialization
    Serial.println("led setup");
    led_setup();

    // // IMU setup
    // imu_setup();
}


void loop()
{
    // Check for blinker
    Serial.println("read blinker button");
    read_blinker_button();

    // Get blindspot measurements
    Serial.println("blindspot detect");
    blindspot_detect();

    // // read imu
    // read_imu();
}