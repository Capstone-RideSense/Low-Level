#include <Arduino.h>
#include "Bluetooth/BluetoothConnection.hpp"
#include <Wire.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"
// #include <bmi323.h>
// #include <examples/common/common.h>


#define DEV_I2C Wire

void setup()
{
    Serial.begin(115200);

    
    DEV_I2C.begin();
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(XSHUT_LEFT, OUTPUT);
    pinMode(BLINKER_BUTTON_LEFT, INPUT);

    // pinMode(XSHUT_RIGHT, OUTPUT);
    // pinMode(BLINKER_BUTTON_RIGHT, INPUT);

    // Bluetooth Initialization
    BLE_Setup();

    // ToF Initialization
    blindspot_setup();

    // LEDDriver Initialization
    led_setup();
}


void loop()
{
    // Check for blinker button
    read_blinker_button();
    
    // Get blindspot measurements
    blindspot_detect();

    // Give some delay
    delay(100);
}

// #include <Arduino.h>
// #include <Wire.h>

// #define INC_ADDRESS 0x68
// #define ACC_CONF  0x20  //Page 91
// #define GYR_CONF  0x21  //Page 93
// #define CMD       0x7E  //Page 65

// short x, y, z;

// //Write data in 16 bits
// void writeRegister16(int16_t reg, int16_t value) {
//   Wire.beginTransmission(INC_ADDRESS);
//   Wire.write(reg);
//   //Low 
//   Wire.write((int16_t)value & 0xff);
//   //High
//   Wire.write((int16_t)value >> 8);
//   Wire.endTransmission();
// }

// //Read data in 16 bits
// int16_t readRegister16(int8_t reg) {
//   Wire.beginTransmission(INC_ADDRESS);
//   Wire.write(reg);
//   Wire.endTransmission(false);
//   int n = Wire.requestFrom(INC_ADDRESS, 4);  
//   int16_t data[4];
//   int i =0;
//   while(Wire.available()){
//     data[i] = Wire.read();
//     i++;
//   }  
//   return (data[3]   | data[2] << 8);
// }

// //Read all axis
// void readAllAccel() {
//   Wire.beginTransmission(INC_ADDRESS);
//   Wire.write(0x03);
//   Wire.endTransmission();
//   Wire.requestFrom(INC_ADDRESS, 6);
//   short data[8];
//   int i =0;
//   while(Wire.available()){
//     data[i] = Wire.read();
//     i++;
//   }

//   //Offset = 2 because the 2 first bytes are dummy (useless)
//   int offset = 2;  
//   x =             (data[offset + 0]   | (short)data[offset + 1] << 8);  //0x03
//   y =             (data[offset + 2]   | (short)data[offset + 3] << 8);  //0x04
//   z =             (data[offset + 4]   | (short)data[offset + 5] << 8);  //0x05
// }

// void softReset(){  
//   writeRegister16(CMD, 0xDEAF);
//   delay(50);    
// }

// void setup(void) {  
//   Serial.begin(115200); 
//   //Accelerometer
//   Wire.begin();  
//   Wire.setClock(400000);      // I2C Fast Mode (400kHz)  
//   softReset();  
//   /*
//    * Acc_Conf P.91
//    * mode:        0x7000  -> High
//    * average:     0x0000  -> No
//    * filtering:   0x0080  -> ODR/4
//    * range:       0x0000  -> 2G
//    * ODR:         0x000B  -> 800Hz
//    * Total:       0x708B
//    */
//   writeRegister16(ACC_CONF,0x708C);//Setting accelerometer  
//   /*
//    * Gyr_Conf P.93
//    * mode:        0x7000  -> High
//    * average:     0x0000  -> No
//    * filtering:   0x0080  -> ODR/4
//    * range:       0x0000  -> 125kdps
//    * ODR:         0x000B  -> 800Hz
//    * Total:       0x708B
//    */
//   writeRegister16(GYR_CONF,0x708B);//Setting gyroscope    
// }

// void loop() {

//   if(readRegister16(0x02) == 0x00) {
//     //Read ChipID   
//     readAllAccel();             // read all accelerometer/gyroscope/temperature data     
//     Serial.print(" \tx:");
//     Serial.print(x);
//     Serial.print(" \ty:");
//     Serial.print(y);
//     Serial.print(" \tz:");
//     Serial.println(z);

//   }
// }
