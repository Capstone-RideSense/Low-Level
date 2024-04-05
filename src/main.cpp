#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include <Wire.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"
#include "Sensors/IMU.hpp"
#include "Utils/scanI2C.hpp"

#define DEBUG_MODE 1
#define BLINDSPOT_EN 1
#define IMU_EN 0
#define LED_EN 0
#define BLE_EN 1
#define SCAN_I2C 0

void setup()
{
    Serial.begin(115200);
    Serial.println("I2C begin");

    DEV_I2C.begin();
    Serial.println("pin setup");
    pinMode(LED_BUILTIN, OUTPUT);

    #if SCAN_I2C

    #else
    // pinMode(XSHUT_LEFT, OUTPUT);
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
    blindspot_setup();
    #endif

    // LEDDriver Initialization
    Serial.println("led setup");
    led_setup();
    #if IMU_EN
    // IMU setup
    imu_setup();
    #endif
    #endif
}


void loop()
{
    #if SCAN_I2C
    scani2cbus();
    #else 
    // Check for blinker
    Serial.println("blinker button");
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
    #endif
}