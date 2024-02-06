#include <Arduino.h>
#include "Sensors/HapticMotors.hpp"
#include "Constants.hpp"

void scratch_blink() {
  digitalWrite(LED_BUILTIN, LOW); 
  delay(1000); 
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(1000); 
}

// write to the haptic motors (connected to Bluetooth)
void haptic_write(std::string dir, int intensity) {  
  if (dir == LEFT_DIRECTION) {
    Serial.print("LEFT ");
    Serial.println(intensity);
    // blink once
    scratch_blink();
  } else if (dir == RIGHT_DIRECTION) {
    Serial.print("RIGHT ");
    Serial.println(intensity);
    //blink twice
    scratch_blink();
    scratch_blink();
  } else if (dir == SLIGHT_LEFT_DIRECTION) {
    Serial.print("SLIGHT LEFT ");
    Serial.println(intensity);
    //blink twice
    scratch_blink();
  } else if (dir == SLIGHT_RIGHT_DIRECTION) {
    Serial.print("SLIGHT RIGHT ");
    Serial.println(intensity);
    //blink twice
    scratch_blink();
    scratch_blink();
  } else {
    Serial.println("You wrote something other than R or L");
  }
}