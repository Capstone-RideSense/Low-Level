// #include <Arduino.h>
// #include "Bluetooth/BluetoothConnection.hpp"
// #include <Wire.h>
// #include "Sensors/Blindspot.hpp"
// #include <PCA9955B.h>


// #define DEV_I2C Wire

// void setup()
// {
//     Serial.begin(115200);
//     DEV_I2C.begin();
//     pinMode(LED_BUILTIN, OUTPUT);
//     pinMode(XSHUT_LEFT, OUTPUT);
//     pinMode(XSHUT_RIGHT, OUTPUT);

//     // Bluetooth Initialization
//     BLE_Setup();

//     // ToF Initialization
//     blindspot_setup();
// }


// void loop()
// {
//     // need a way to be notified when we are turning
//     //  IMU will handle the detection - read them first
//     // char turn_direction = imu_read_turn(); //return 0 on no turn, L on left, R on right
//     // if (turn_direction) {
//     //     set_roi_turn(turn_direction);
//     // }     
    
//     // Get blindspot measurements
//     blindspot_detect();

//     // Give some delay
//     delay(100);
// }
/** PCA9955B LED driver operation sample
 *  
 *  This sample code is showing PCA9955B LED driver operation with PWM value setting change.
 *  The ledd.pwm() takes channel number and PWM ratio. 
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCA9955B:
 *    https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/led-drivers/16-channel-fm-plus-ic-bus-57-ma-20-v-constant-current-led-driver:PCA9955BTW
 */

#include <PCA9955B.h>

PCA9955B ledd;

void setup() {
  Serial.begin(115200);
  Serial.println("HELLO 1");

  Wire.begin();
  Serial.println("HELLO 2");
  ledd.begin(1.0, PCA9955B::NONE);

  Serial.println("\n***** Hello, PCA9955B! *****");
}

void loop() {
  for (int ch = 0; ch < ledd.n_channel; ch++) {
    Serial.println(ch);
    for (int i = 0; i < 256; i++) {
      ledd.pwm(ch, i / 255.0);
      delay(1);
    }
    for (int i = 255; 0 <= i; i--) {
      ledd.pwm(ch, i / 255.0);
      delay(1);
    }
  }
}