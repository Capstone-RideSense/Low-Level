#include <Arduino.h>
#include "Sensors/Blindspot.hpp"
#include <Wire.h>
#include <VL53L1X.h>
#include "LEDs/LEDDriver.hpp"


void blindspot_setup() {
    pinMode(BLINDSPOT_LEFT_PIN, INPUT);
    pinMode(BLINDSPOT_RIGHT_PIN, INPUT);
}

void detect(int dir) {
    if(digitalRead(dir ? BLINDSPOT_RIGHT_PIN : BLINDSPOT_LEFT_PIN) == HIGH) {
        turn_on_blindspot_leds(dir);
    } else {
        turn_off_blindspot_leds(dir);
    }
}

void blindspot_detect() {
    detect(0); // left
    detect(1); // right
}