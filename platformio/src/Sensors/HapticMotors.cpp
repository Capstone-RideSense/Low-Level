#include <Arduino.h>
#include "Sensors/HapticMotors.hpp"
#include "Constants.hpp"

// write to the haptic motors (connected to Bluetooth)
void haptic_write(String direction) {
  if (direction == LEFT_DIRECTION) {
    Serial.println("LEFT");
    digitalWrite(LED_BUILTIN, LOW); 
    delay(1000); 
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(1000); 
  } else if (direction == RIGHT_DIRECTION) {
    Serial.println("RIGHT");
    digitalWrite(LED_BUILTIN, LOW); 
    delay(1000); 
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(1000); 
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(1000); 
  } else {
    Serial.println("You wrote something other than R or L");
  }
}