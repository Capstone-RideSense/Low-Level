#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define LED_BUILTIN 2

// for blind spot detection
#define XSHUT 4
#define BLINDSPOT_MM 1000

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