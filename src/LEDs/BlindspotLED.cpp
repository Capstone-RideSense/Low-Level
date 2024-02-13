#include <Arduino.h>
#include "LEDs/BlindspotLED.hpp"
#include "Constants.hpp"

void turn_on_blindspot_leds() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void turn_off_blindspot_leds() {
  digitalWrite(LED_BUILTIN, LOW);
}