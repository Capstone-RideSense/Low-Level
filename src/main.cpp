#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include <Wire.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"
#include "Sensors/IMU.hpp"

#define DEBUG_MODE 1
#define BLINDSPOT_EN 0
#define IMU_EN 0 
#define LED_EN 1
#define BLE_EN 1

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
    pinMode(XSHUT_RIGHT, OUTPUT);
    pinMode(BLINKER_BUTTON_RIGHT, INPUT);

    // Bluetooth Initialization
    Serial.println("BLE setup");
    BLE_Setup();

    // ToF Initialization
    #if BLINDSPOT_EN
    Serial.println("Blindspot setup");
    // blindspot_setup();
    #endif

    // LEDDriver Initialization
    Serial.println("led setup");
    led_setup();
    #if IMU_EN
    // IMU setup
    imu_setup();
    #endif
}


void loop()
{
    // Check for blinker
    Serial.println("read blinker button");
    read_blinker_button();

    // Get blindspot measurements
    #if BLINDSPOT_EN
    Serial.println("blindspot detect");
    blindspot_detect();
    #endif

    #if IMU_EN
    // read imu
    read_imu();
    #endif
}