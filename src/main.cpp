#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include "Sensors/Blindspot.hpp"

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(XSHUT_LEFT, OUTPUT);
    pinMode(XSHUT_RIGHT, OUTPUT);

    // Bluetooth Initialization
    BLE_Setup();

    // ToF Initialization
    blindspot_setup();
}

void loop()
{
    // need a way to be notified when we are turning
    //  IMU will handle the detection - read them first
    // char turn_direction = imu_read_turn(); //return 0 on no turn, L on left, R on right
    // if (turn_direction) {
    //     set_roi_turn(turn_direction);
    // }     
    
    // Get blindspot measurements
    blindspot_detect();

    // Give some delay
    delay(100);
}