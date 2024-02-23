#include <Arduino.h>
#include "LEDs/BlindspotLED.hpp"
#include "Constants.hpp"

void turn_on_blindspot_leds(int direction) {
    // the standin until we get the led drivers
    digitalWrite(LED_BUILTIN, HIGH);
}

void turn_off_blindspot_leds(int direction) {
    // the standin until we get the led drivers
    digitalWrite(LED_BUILTIN, LOW);
}