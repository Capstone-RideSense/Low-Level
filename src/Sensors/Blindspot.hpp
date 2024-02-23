#ifndef BLINDSPOT_HPP
#define BLINDSPOT_HPP

#include <Arduino.h>
#include "Constants.hpp"
#include <Wire.h>
#include <vl53l1_class.h>

void blindspot_setup();

void blindspot_detect();

void set_roi_turn(int direction);


#endif // BLINDSPOT_HPP