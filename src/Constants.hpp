#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


#define LED_BUILTIN 2
#define DEV_I2C Wire

// shutdown pinds for the time of flight
// TODO: are these different now?
#define XSHUT_LEFT 4
#define XSHUT_RIGHT 5

// blinker button
#define BLINKER_BUTTON_LEFT 13
#define BLINKER_BUTTON_RIGHT 18
#define BLINK_DELAY 400

// threshold for blindspot (in mm)
#define BLINDSPOT_MM 1000

// zone for the sensor when turning (maybe not needed)
#define TURN_TLX 2
#define TURN_TLY 2
#define TURN_BRX 10
#define TURN_BRY 10

// index for each sensor in the sensor array
#define LEFT_IDX 0
#define RIGHT_IDX 1

// addresses for sensor I2Cs
// TODO: fix these?
#define LEFT_BLINDSPOT_ADDR 0x12
#define RIGHT_BLINDSPOT_ADDR 0x52

// TODO: is right actually 70?
#define LEFT_LED_ADDR 0x69
#define RIGHT_LED_ADDR 0x70

#define IMU_ADDR 0x68

// haptic definitions - for stretch goal (not happening)
#define P_HAPTIC 6 // pointer finger
#define M_HAPTIC 7 // middle finger
#define R_HAPTIC 8 // ring finger
#define Y_HAPTIC 9 // pinky finger

// direction related constants
#define LEFT_DIRECTION 'L'
#define RIGHT_DIRECTION 'R'
#define SLIGHT_LEFT_DIRECTION 'X'
#define SLIGHT_RIGHT_DIRECTION 'Y'
#define INTENSITY_MIN 0
#define INTENSITY_MAX 5

// Define UUIDs used
#define SERVICE_UUID "9bc7cd1f-f015-497c-ad8b-67cb800444fe"
#define DIRECTION_CHARACTERISTIC_UUID "67adc54c-16aa-4f26-8b79-29584847d15c"

#endif //CONSTANTS_HPP