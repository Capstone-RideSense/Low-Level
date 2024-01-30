#include <Arduino.h>
#include "Sensors/HapticMotors.hpp"
#include "Constants.hpp"

// write to the haptic motors (connected to Bluetooth)
void haptic_write(String direction) {
  if (direction == LEFT_DIRECTION) {
    Serial.println("LEFT");
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second
  } else if (direction == RIGHT_DIRECTION) {
    Serial.println("RIGHT");
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off by making the voltage LOW
    delay(1000); 
  }
}