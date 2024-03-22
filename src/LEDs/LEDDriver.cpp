#include <Arduino.h>
#include "LEDs/LEDDriver.hpp"
#include "Constants.hpp"
#include <PCA9955B.h>

PCA9955B led_driver_left(LEFT_LED_ADDR);
PCA9955B led_driver_right(RIGHT_LED_ADDR);

PCA9955B led_driver_list[] = {led_driver_left, led_driver_right};

int blindspot_ch[] = {5, 10, 11}; 
// int blinker_ch[] = {0, 1, 2, 3, 4, 12, 13, 14, 15};
int blinker_ch[] = {2}; // for testing
int haptic_ch[] = {6, 7, 8, 9};

int current_interval[] = {0, 0};
bool states[] = {true, true};

int blinker_buttons[] = {BLINKER_BUTTON_LEFT, BLINKER_BUTTON_RIGHT};

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

bool blink_blinker(int direction, bool next_state) {
    Serial.print("current state: ");
    Serial.println(next_state);
    float new_write = 0;
    if (current_interval[direction] > BLINK_DELAY) {
        // edge case - change current state and reset current_interval
        current_interval[direction] = 0;
        next_state = !next_state;
    }

    if (next_state) {
        new_write = 0.5;
    }
    Serial.print("new write: ");
    Serial.println(new_write);
    for (int i = 0; i < sizeof(blinker_ch) / sizeof(int); i++) {
        led_driver_list[direction].pwm(blinker_ch[i], new_write);
    }

    return next_state;
}

void set_blinker (bool state) {
    float w = 0;
    if (state) {
        w = 0.5;
    }
    for (int i = 0; i < sizeof(blinker_ch) / sizeof(int); i++) {
        led_driver_list[0].pwm(blinker_ch[i], w);
    }
}

void read_blinker_button() {
    for (int i = 0; i < sizeof(blinker_buttons) / sizeof(int); i++) {
        if (digitalRead(BLINKER_BUTTON_LEFT) == HIGH) {
            if (current_interval[i] == 0) {
                Serial.println("set");
                //activate the things
                set_blinker(states[i]);
            } else if (current_interval[i] == BLINK_DELAY) {
                Serial.println("change state");
                states[i] = !states[i];
                current_interval[i] = -1;
            }
            Serial.println(current_interval[i]);
            current_interval[i]++;
        } else {
            Serial.println("its off");
            current_interval[i] = 0;
            set_blinker(false);
            states[i] = true;
        }   
    } 
}

void blink_off(int direction) {
    for (int i = 0; i < sizeof(blinker_ch) / sizeof(int); i++) {
        led_driver_list[direction].pwm(blinker_ch[i], 0);
    }
}

bool* read_blinker_button(bool *next_states) {
    // read blinker
    for (int i = 0; i < sizeof(blinker_buttons) / sizeof(int); i++) {
        if (digitalRead(blinker_buttons[i]) == HIGH) {
            Serial.println("Button is pressed");
            next_states[i] = blink_blinker(i, next_states[i]);
        } else {
            blink_off(i);
            next_states[i] = true;
        }
    }

    return next_states;
}

void led_setup() {
    led_driver_left.begin(1.0, PCA9955B::NONE);
    led_driver_right.begin(1.0, PCA9955B::NONE);
}

// write to the haptic motors (connected to Bluetooth)
void haptic_write(char direction, int intensity) { 
    double intensity_d = 0.5 + (double) intensity / 10; 
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