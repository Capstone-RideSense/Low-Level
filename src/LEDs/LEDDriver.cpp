#include <Arduino.h>
#include "LEDs/LEDDriver.hpp"
#include "Constants.hpp"
#include <PCA9955B.h>

#if LEFT 
PCA9955B led_driver_left(LEFT_LED_ADDR);
#endif 
#if RIGHT 
PCA9955B led_driver_right(RIGHT_LED_ADDR);
#endif 

PCA9955B led_driver_list[] = {
    #if LEFT
    led_driver_left,
    #endif 
    #if RIGHT
    led_driver_right
    #endif
};

// TODO: change these back to the correct channels
int blindspot_ch_left[] = {1, 2, 15}; 
int blindspot_ch_right[] = {5, 10, 11};
int blinker_ch_left[] = {0, 3, 4, 5, 10, 11, 12, 13, 14};
int blinker_ch_right[] = {0, 1, 2, 3, 4, 12, 13, 14, 15};
int haptic_ch[] = {6, 7, 8, 9};

long start_ms[] = {0, 0};
bool states[] = {true, true};

// TODO: redefine with correct values
int blinker_buttons[] = {
    #if LEFT
    BLINKER_BUTTON_LEFT, 
    #endif
    #if RIGHT
    BLINKER_BUTTON_RIGHT
    #endif
};
// int blinker_buttons[] = {BLINKER_BUTTON_LEFT};
int left_i = 0;
int right_i = 1;


void turn_on_blindspot_leds(int direction) {
    for (int i = 0; i < sizeof(blindspot_ch_left) / sizeof(int); i++) {
        if (direction == LEFT_IDX) {
            led_driver_list[direction].pwm(blindspot_ch_left[i], .5); 
        } else {
            led_driver_list[direction].pwm(blindspot_ch_right[i], .5); 
        }
    }
}

void turn_off_blindspot_leds(int direction) {
    for (int i = 0; i < sizeof(blindspot_ch_right) / sizeof(int); i++) {
        if (direction == LEFT_IDX) {
            led_driver_list[direction].pwm(blindspot_ch_left[i], 0); 
        } else {
            led_driver_list[direction].pwm(blindspot_ch_right[i], 0); 
        }
    }
}

void set_blinker (int direction, bool state) {
    float w = 0;
    if (state) {
        w = 0.5;
    }
    for (int i = 0; i < sizeof(blinker_ch_left) / sizeof(int); i++) {
        if (direction == LEFT_IDX) {
            led_driver_list[direction].pwm(blinker_ch_left[i], w);
        } else {
            led_driver_list[direction].pwm(blinker_ch_right[i], w);
        }
    }
}

void read_blinker_button() {
    // Serial.printf("L:%d R:%d\n", digitalRead(blinker_buttons[0]), digitalRead(blinker_buttons[1]));
    for (int i = 0; i < sizeof(blinker_buttons) / sizeof(int); i++) {
        long current_ms = millis();
        long ms_dif = current_ms - start_ms[i];
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
    #if LEFT
    led_driver_left.begin(1.0, PCA9955B::NONE, 1);
    #endif
    // TODO: enable right
    #if RIGHT
    led_driver_right.begin(1.0, PCA9955B::NONE, 1);
    #endif
}

// write to the haptic motors (connected to Bluetooth)
void haptic_write(char direction, int intensity) { 
    double intensity_d = 0.4 + (double) intensity / 10; 
    if (intensity == 0) {
        intensity_d = 0;
    }
    if (direction == LEFT_DIRECTION) {
        Serial.printf("LEFT - Written:%d Haptic:%f\n", intensity, intensity_d);

        for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
            led_driver_list[LEFT_IDX].pwm(haptic_ch[i], intensity_d);
        }
    } else if (direction == RIGHT_DIRECTION) {
        Serial.printf("RIGHT - Written:%d Haptic:%f\n", intensity, intensity_d);
        // TODO: enable right (uncomment this loop)
        for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
            led_driver_list[RIGHT_IDX].pwm(haptic_ch[i], intensity_d);
        }
    } else if (direction == COMPLETE_DIR) {
        Serial.println("Navigation Complete");
        for (int num_blinks = 0; num_blinks < 3; num_blinks++) {
            for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
                led_driver_list[LEFT_IDX].pwm(haptic_ch[i], 0);
                led_driver_list[RIGHT_IDX].pwm(haptic_ch[i], 0);
                Serial.println("haptics off");
            }
            delay(200);
            for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
                led_driver_list[LEFT_IDX].pwm(haptic_ch[i], intensity_d);
                led_driver_list[RIGHT_IDX].pwm(haptic_ch[i], intensity_d);
                Serial.println("haptics on");
            }
            delay(200);
            for (int i = 0; i < sizeof(haptic_ch) / sizeof(int); i++) {
                led_driver_list[LEFT_IDX].pwm(haptic_ch[i], 0);
                led_driver_list[RIGHT_IDX].pwm(haptic_ch[i], 0);
                Serial.println("haptics off");
            }
            delay(200);
        }
    } else {
        Serial.println("You wrote something other than L, R");
    }
}