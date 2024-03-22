#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include <Wire.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"
// #include <bmi323.h>
// #include <examples/common/common.h>
bool next_states[] = {true, true};
int cur_interval = 0;
bool state = true;

#define DEV_I2C Wire

void setup()
{
    Serial.begin(115200);

    
    DEV_I2C.begin();
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(XSHUT_LEFT, OUTPUT);
    pinMode(BLINKER_BUTTON_LEFT, INPUT);

    // pinMode(XSHUT_RIGHT, OUTPUT);
    // pinMode(BLINKER_BUTTON_RIGHT, INPUT);

    // Bluetooth Initialization
    BLE_Setup();

    // ToF Initialization
    blindspot_setup();

    // LEDDriver Initialization
    led_setup();
}


void loop()
{
    // Check for blinker
    read_blinker_button();

    // Get blindspot measurements
    blindspot_detect();

    // Give some delay
    delay(200);
}