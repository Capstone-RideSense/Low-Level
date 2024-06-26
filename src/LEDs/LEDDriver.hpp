#ifndef LEDDRIVER_HPP
#define LEDDRIVER_HPP

#include <Arduino.h>
#include "Constants.hpp"

void turn_on_blindspot_leds(int direction);

void turn_off_blindspot_leds(int direction);

void set_blinker(int direction, bool state);

void read_blinker_button();

void led_setup();

void haptic_write(char dir, int intensity);

#endif // LEDDRIVER_HPP