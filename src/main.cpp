#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include <Wire.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"
#include "Sensors/IMU.hpp"
bool next_states[] = {true, true};
int cur_interval = 0;
bool state = true;

void setup()
{
    Serial.begin(115200);

    
    DEV_I2C.begin();
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(XSHUT_LEFT, OUTPUT);
    pinMode(BLINKER_BUTTON_LEFT, INPUT);

    //interrupt
    pinMode(33, INPUT_PULLUP);
    attachInterrupt(33, measure, FALLING);

    // pinMode(XSHUT_RIGHT, OUTPUT);
    // pinMode(BLINKER_BUTTON_RIGHT, INPUT);

    // Bluetooth Initialization
    BLE_Setup();

    // ToF Initialization
    blindspot_setup();

    // LEDDriver Initialization
    led_setup();

    // // IMU setup
    // imu_setup();
}


void loop()
{
    // Check for blinker
    read_blinker_button();

    // Get blindspot measurements
    blindspot_detect();

    // // read imu
    // read_imu();
}