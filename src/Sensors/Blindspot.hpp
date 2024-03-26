#ifndef BLINDSPOT_HPP
#define BLINDSPOT_HPP

#include <Arduino.h>
#include "Constants.hpp"
#include <Wire.h>
#include <vl53l1_class.h>
#include <vl53l4cx_class.h>

void blindspot_setup();

void blindspot_detect();

void measure();

#endif // BLINDSPOT_HPP