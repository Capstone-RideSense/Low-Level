#include <Arduino.h>
#include "LEDs/LEDDriver.hpp"
#include "Constants.hpp"
#include <PCA9955B.h>

PCA9955B led_driver_left(LEFT_LED_ADDR);
PCA9955B led_driver_right(RIGHT_LED_ADDR);

PCA9955B led_driver_list[] = {led_driver_left, led_driver_right};

// int blindspot_ch[] = {5, 10, 11}; 
int blindspot_ch[] = {2}; // for testing 
int blinker_ch[] = {0, 1, 2, 3, 4, 12, 13, 14, 15};
int haptic_ch[] = {6, 7, 8, 9};

int blinker_buttons[] = {BLINKER_LEFT, BLINKER_RIGHT};


void turn_on_blindspot_leds(int direction) {
    for (int i = 0; i < sizeof(blindspot_ch) / sizeof(int); i++) {
        led_driver_list[direction].pwm(blindspot_ch[i], .5); 
    }
}

void turn_off_blindspot_leds(int direction) {
    for (int i = 0; i < sizeof(blindspot_ch) / sizeof(int); i++) {
        led_driver_list[direction].pwm(blindspot_ch[i], 0); 
    }
}

void blink_blinker(int direction) {
    for (int i = 0; i < sizeof(blinker_ch) / sizeof(int); i++) {
        led_driver_list[direction].pwm(blinker_ch[i], 0.5);
    }
    delay(200);
    for (int i = 0; i < sizeof(blinker_ch) / sizeof(int); i++) {
        led_driver_list[direction].pwm(blinker_ch[i], 0);
    }
    delay(200);
}

void read_blinker_button() {
    // read blinker
    for (int i = 0; i < sizeof(blinker_buttons) / sizeof(int); i++) {
        if (digitalRead(blinker_buttons[i]) == HIGH) {
            blink_blinker(i);
        }
    }
}

void led_setup() {
//   Wire.begin();

  led_driver_left.begin(1.0, PCA9955B::NONE);
  led_driver_right.begin(1.0, PCA9955B::NONE);

  // pinMode(BLINKER_BUTTON, INPUT);
}

// write to the haptic motors (connected to Bluetooth)
void haptic_write(char direction, int intensity) { 
    double intensity_d = 0.4 + (double) intensity / 10; 
    if (intensity == 0) {
        intensity_d = 0;
    }
    
    if (direction == LEFT_DIRECTION) {
        Serial.print("LEFT ");
        Serial.println(intensity);

        for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
            led_driver_list[LEFT_IDX].pwm(haptic_ch[i], intensity_d);
        }
    } else if (direction == RIGHT_DIRECTION) {
        for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
            led_driver_list[RIGHT_IDX].pwm(haptic_ch[i], intensity_d);
        }
    } else {
        Serial.println("You wrote something other than L, R");
    }
}