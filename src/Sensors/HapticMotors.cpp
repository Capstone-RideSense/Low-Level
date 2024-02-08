#include <Arduino.h>
#include "Sensors/HapticMotors.hpp"
#include "Constants.hpp"

void scratch_blink() {
  digitalWrite(LED_BUILTIN, LOW); 
  delay(300); 
  digitalWrite(LED_BUILTIN, HIGH);   
  delay(300); 
}

void handle_intensity(int intensity) {
  // eventually: set the current going into the haptic motors
  // haptics will be set to 0 automatically
  // when given a new intensity, set it to that

  // for now: blink the led for the level of intensity
  for (int ii = 0; ii < intensity; ii++) {
      scratch_blink();
    }
}

// write to the haptic motors (connected to Bluetooth)
void haptic_write(char direction, int intensity) {  
  int* haptic_addresses;

  if (direction == LEFT_DIRECTION) {
    Serial.print("LEFT ");
    Serial.println(intensity);

    haptic_addresses = {1, 2, 3};
    
    // eventually: will probably also send some address for the haptic motors to
    //             control depending on how we keep track of the motors 
    handle_intensity(intensity);
    
  } else if (direction == RIGHT_DIRECTION) {
    Serial.print("RIGHT ");
    Serial.println(intensity);
    //blink twice
    handle_intensity(intensity);
  } else if (direction == SLIGHT_LEFT_DIRECTION) {
    Serial.print("SLIGHT LEFT ");
    Serial.println(intensity);
    //blink twice
    handle_intensity(intensity);
  } else if (direction == SLIGHT_RIGHT_DIRECTION) {
    Serial.print("SLIGHT RIGHT ");
    Serial.println(intensity);
    //blink twice
    handle_intensity(intensity);
  } else {
    Serial.println("You wrote something other than L, R, X, Y");
  }
}