#ifndef LEDDRIVER_HPP
#define LEDDRIVER_HPP

#include <Arduino.h>
#include "Constants.hpp"

void turn_on_blindspot_leds(int direction);

void turn_off_blindspot_leds(int direction);

void read_blinker_button();

void blink_blinker(int direction);

void led_setup();

void haptic_write(char dir, int intensity);

#endif // LEDDRIVER_HPP