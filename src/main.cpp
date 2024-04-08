#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include <Wire.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"
#include "Sensors/IMU.hpp"
#include "Utils/scanI2C.hpp"

#define DEBUG_MODE 1
#define BLINDSPOT_EN 0
#define IMU_EN 0
#define LED_EN 1
#define BLE_EN 0
#define BLINKER_EN 1
#define SCAN_I2C 0

#define SDA_OTHER 13
#define SCL_OTHER 14


void setup()
{
    Serial.begin(115200);
    Serial.println("I2C begin");

    DEV_I2C.begin();
    Wire1.begin(SDA_OTHER, SCL_OTHER, 400000);

    Serial.println("pin setup");
    pinMode(LED_BUILTIN, OUTPUT);

    #if SCAN_I2C

    #else
    #if LEFT
    pinMode(BLINKER_BUTTON_LEFT, INPUT);
    #endif

    #if RIGHT
    pinMode(BLINKER_BUTTON_RIGHT, INPUT);
    #endif

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

    // #if DEBUG_MODE

    // haptic_write('R', 5);

    #if BLINKER_EN
    // Check for blinker
    // Serial.println("blinker button");
    read_blinker_button();
    #endif

    // Get blindspot measurements
    #if BLINDSPOT_EN
    Serial.println("blindspot detect");
    blindspot_loop();
    #endif

    #if IMU_EN
    // read imu
    read_imu();
    #endif
    #endif
}

// /*
// This example shows how to set up and read multiple VL53L1X sensors connected to
// the same I2C bus. Each sensor needs to have its XSHUT pin connected to a
// different Arduino pin, and you should change sensorCount and the xshutPins array
// below to match your setup.

// For more information, see ST's application note AN4846 ("Using multiple VL53L0X
// in a single design"). The principles described there apply to the VL53L1X as
// well.
// */

// #include <Arduino.h>
// #include <Wire.h>
// #include <VL53L1X.h>

// TwoWire OtherI2C = TwoWire(0);
// #define SDA_OTHER 13
// #define SCL_OTHER 14

// // The number of sensors in your system.
// const uint8_t sensorCount = 2;

// // The Arduino pin connected to the XSHUT pin of each sensor.
// const uint8_t xshutPins[sensorCount] = { 16, 27 };
// // const uint8_t xshutPins[sensorCount] = { 16 };


// VL53L1X sensors[sensorCount];

// #define LED_BUILTIN 2

// void setup()
// {
//   pinMode(LED_BUILTIN, OUTPUT); 
//   digitalWrite(LED_BUILTIN, HIGH); 
//   delay(50); 
//   digitalWrite(LED_BUILTIN, LOW); 
//   Serial.begin(115200);

//   while (!Serial) {
//     digitalWrite(LED_BUILTIN, HIGH); 
//   }
//   digitalWrite(LED_BUILTIN, LOW); 
//   Serial.println("begin wire"); 
//   OtherI2C.begin(SDA_OTHER, SCL_OTHER, 400000);
//   // Wire.begin();
//   Serial.println("after wire");
  
//   // Wire.setClock(400000); // use 400 kHz I2C

//   // Disable/reset all sensors by driving their XSHUT pins low.
//   for (uint8_t i = 0; i < sensorCount; i++)
//   {
//     pinMode(xshutPins[i], OUTPUT);
//     digitalWrite(xshutPins[i], LOW);
//     Serial.println("after pins");
//   }

//   // Enable, initialize, and start each sensor, one by one.
//   for (uint8_t i = 0; i < sensorCount; i++)
//   {
//     // Stop driving this sensor's XSHUT low. This should allow the carrier
//     // board to pull it high. (We do NOT want to drive XSHUT high since it is
//     // not level shifted.) Then wait a bit for the sensor to start up.
//     pinMode(xshutPins[i], INPUT);
//     // digitalWrite(xshutPins[i], HIGH);

//     delay(10);
//     Serial.println("after pin change");
//     sensors[i].setBus(&OtherI2C);
//     sensors[i].setTimeout(500);
//     if (!sensors[i].init())
//     {
//       Serial.print("Failed to detect and initialize sensor ");
//       Serial.println(i);
//       while (1);
//     }

//     // Each sensor must have its address changed to a unique value other than
//     // the default of 0x29 (except for the last one, which could be left at
//     // the default). To make it simple, we'll just count up from 0x2A.
//     Serial.println("before set address");
//     sensors[i].setAddress(0x2A + i);
//     Serial.println("after set address");

//     sensors[i].startContinuous(50);
//     Serial.println("after start continuous");
//     // digitalWrite(xshutPins[i], LOW);

//   }
// }

// void loop()
// {
//   for (uint8_t i = 0; i < sensorCount; i++)
//   {
//     Serial.printf("%d: %d\n", xshutPins[i], sensors[i].read()); 
//     // Serial.print(sensors[i].read());
//     if (sensors[i].timeoutOccurred()) { Serial.println(" TIMEOUT"); }
//     // Serial.print('\t');
//   }
//   // Serial.println();
// }
