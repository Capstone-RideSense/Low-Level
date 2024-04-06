#include <Arduino.h>
#include "Sensors/Blindspot.hpp"
#include <Wire.h>
#include <VL53L1X.h>
#include "LEDs/LEDDriver.hpp"

// TwoWire OtherI2C = TwoWire(0);


// The number of sensors in your system.
const uint8_t sensorCount = 2;

// The Arduino pin connected to the XSHUT pin of each sensor.

const uint8_t xshutPins[sensorCount] = { XSHUT_RIGHT, XSHUT_LEFT };
// const uint8_t xshutPins[sensorCount] = { 16 };


VL53L1X sensors[sensorCount];

#define LED_BUILTIN 2

void blindspot_setup()
{
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(50); 
  digitalWrite(LED_BUILTIN, LOW); 
//   pinMode(27, OUTPUT); 
//   digitalWrite(27, HIGH); 
  Serial.begin(115200);

  while (!Serial) {
    digitalWrite(LED_BUILTIN, HIGH); 
  }
  digitalWrite(LED_BUILTIN, LOW); 
  Serial.println("begin wire"); 
  //   OtherI2C.begin(SDA_OTHER, SCL_OTHER, 400000);
  // Wire.begin();
  Serial.println("after wire");
  
  // Wire.setClock(400000); // use 400 kHz I2C

  // Disable/reset all sensors by driving their XSHUT pins low.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
    Serial.println("after pins");
  }

  // Enable, initialize, and start each sensor, one by one.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshutPins[i], INPUT);
    // digitalWrite(xshutPins[i], HIGH);

    delay(10);
    Serial.println("after pin change");
    // sensors[i].setBus(&OtherI2C);
    sensors[i].setBus(&Wire1);
    sensors[i].setTimeout(500);
    if (!sensors[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    Serial.println("before set address");
    sensors[i].setAddress(0x2A + i);
    Serial.println("after set address");

    sensors[i].startContinuous(50);
    Serial.println("after start continuous");
    // digitalWrite(xshutPins[i], LOW);

  }
}

void blindspot_loop()
{
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    uint16_t read = sensors[i].read(); 
    Serial.printf("%d: %d\n", xshutPins[i], read); 
    if(read < BLINDSPOT_MM) {
        Serial.println("detected"); 
        turn_on_blindspot_leds(i); 
    } else {
        turn_off_blindspot_leds(i); 
    }
    
    // Serial.print(sensors[i].read());
    if (sensors[i].timeoutOccurred()) { Serial.println(" TIMEOUT"); }
    // Serial.print('\t');
  }
  // Serial.println();
}
