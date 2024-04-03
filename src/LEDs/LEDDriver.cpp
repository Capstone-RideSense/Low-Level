#include <Arduino.h>
#include "LEDs/LEDDriver.hpp"
#include "Constants.hpp"
#include <PCA9955B.h>

PCA9955B led_driver_left(LEFT_LED_ADDR);
// PCA9955B led_driver_right(RIGHT_LED_ADDR);

PCA9955B led_driver_list[] = {led_driver_left};

// TODO: change these back to the correct channels
int blindspot_ch[] = {5, 10, 11}; 
// int blinker_ch[] = {0, 1, 2, 3, 4, 12, 13, 14, 15};
int blinker_ch[] = {2}; // TODO: switch back
int haptic_ch[] = {6, 7, 8, 9};

long start_ms[] = {0, 0};
bool states[] = {true, true};

// TODO: redefine with correct values
// int blinker_buttons[] = {BLINKER_BUTTON_LEFT, BLINKER_BUTTON_RIGHT};
int blinker_buttons[] = {BLINKER_BUTTON_LEFT};


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

void set_blinker (int direction, bool state) {
    float w = 0;
    if (state) {
        w = 0.5;
    }
    for (int i = 0; i < sizeof(blinker_ch) / sizeof(int); i++) {
        led_driver_list[direction].pwm(blinker_ch[i], w);
    }
}

void read_blinker_button() {
    for (int i = 0; i < sizeof(blinker_buttons) / sizeof(int); i++) {
        long current_ms = millis();
        long ms_dif = current_ms - start_ms[i];
        Serial.println(digitalRead(blinker_buttons[i]));
        if (digitalRead(blinker_buttons[i]) == HIGH) {
            if (ms_dif > BLINK_DELAY) {
                states[i] = !states[i];
                start_ms[i] = millis();
            } else {
                set_blinker(i, states[i]);
            }
        } else {
            start_ms[i] = millis();
            set_blinker(i, false);
            states[i] = true;
        }   
    } 
}

void led_setup() {
    led_driver_left.begin(1.0, PCA9955B::NONE, 1);
    // TODO: enable right
    // led_driver_right.begin(1.0, PCA9955B::NONE);
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
        Serial.println("RIGHT");
        Serial.println(intensity);
        Serial.println(blindspot_ch[3]);
        // TODO: enable right (uncomment this loop)
        // for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
        //     led_driver_list[RIGHT_IDX].pwm(haptic_ch[i], intensity_d);
        // }
    } else {
        Serial.println("You wrote something other than L, R");
    }
}