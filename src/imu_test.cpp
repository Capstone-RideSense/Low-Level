// // #include <Arduino.h>
// // #include "Bluetooth/BluetoothConnection.hpp"
// // #include <Wire.h>
// // #include "Sensors/Blindspot.hpp"
// // #include "LEDs/LEDDriver.hpp"
// // #include <bmi323.h>
// // #include <examples/common/common.h>


// // #define DEV_I2C Wire

// // void setup()
// // {
// //     Serial.begin(115200);

    
// //     DEV_I2C.begin();
// //     pinMode(LED_BUILTIN, OUTPUT);

// //     pinMode(XSHUT_LEFT, OUTPUT);
// //     pinMode(BLINKER_BUTTON_LEFT, INPUT);

// //     // pinMode(XSHUT_RIGHT, OUTPUT);
// //     // pinMode(BLINKER_BUTTON_RIGHT, INPUT);

// //     // Bluetooth Initialization
// //     BLE_Setup();

// //     // ToF Initialization
// //     blindspot_setup();

// //     // LEDDriver Initialization
// //     led_setup();
// // }


// // void loop()
// // {
// //     // Check for blinker button
// //     read_blinker_button();
    
// //     // Get blindspot measurements
// //     blindspot_detect();

// //     // Give some delay
// //     delay(100);
// // }

// #include <Arduino.h>
// #include <Wire.h>
// #include <SimpleKalmanFilter.h>

// #define INC_ADDRESS 0x68
// #define ACC_CONF  0x20  //Page 91
// #define GYR_CONF  0x21  //Page 93
// #define CMD       0x7E  //Page 65

// #define GRAVITY_EARTH  (9.80665f)

// u_int16_t x, y, z;
// float accelX_m_s2 , accelY_m_s2, accelZ_m_s2;

// SimpleKalmanFilter imuKalmanFilter(1, 1, .1);
// float baseline = 0.03; // baseline pressure
// float velocity; // velocity of the object
// unsigned long last_time;
// unsigned long prev_time;
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
// static float lsb_to_mps2(int16_t val, int8_t g_range, uint8_t bit_width)
// {
//     double power = 2;

//     float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

//     return (GRAVITY_EARTH * val * g_range) / half_scale;
// }
// //Read all axis
// void readAllAccel() {
//   Wire.beginTransmission(INC_ADDRESS);
//   Wire.write(0x04);
//   Wire.endTransmission();
//   Wire.requestFrom(INC_ADDRESS, 4);
//   u_int8_t data[8];
//   int i =0;
//   while(Wire.available()){
//     data[i] = Wire.read();
//     i++;
//   }
//   Serial.println(data[3], HEX);
//   Serial.println(data[2], HEX);
//   //Offset = 2 because the 2 first bytes are dummy (useless)
//   y = data[2] | (data[3] << 8);  //0x04
//   // int offset = 2;  
//   // x =             (data[offset + 0]   | (short)data[offset + 1] << 8);  //0x03
//   // y =             (data[offset + 2]   | (short)data[offset + 3] << 8);  //0x04
//   // z =             (data[offset + 4]   | (short)data[offset + 5] << 8);  //0x05
//   // accelX_m_s2 = lsb_to_mps2(x, 1, 16);
//   Serial.print(">y_raw:");
//   Serial.println(y);
//   Serial.print('>y_adc:');
//   Serial.println(y/16384.0);
//   // accelY_m_s2 = y/16384.0;
//   accelY_m_s2 = lsb_to_mps2(y, 1, 16);
//   // Serial.println(accelY_m_s2);
//   // accelZ_m_s2 = lsb_to_mps2(z, 1, 16);
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
//   // for (int i = 0; i < 1000; i++){
//   //   delay(10);
//   //   readAllAccel(); 
//   //   baseline += accelY_m_s2;
//   // }
//   // baseline /= 1000; 
  
//   prev_time = millis(); 
// }

// void loop() {

//   if(readRegister16(0x02) == 0x00) {
//     //Read ChipID   
//     readAllAccel();             // read all accelerometer/gyroscope/temperature data     
//     // teleplot.update("x", x);
//     // teleplot.update("y", y);
//     // teleplot.update("z", z);
//     // Serial.print(">x:");
//     // Serial.println(accelX_m_s2);
//     Serial.print(">y:");
//     Serial.println(accelY_m_s2);
//     // Serial.print(">z:");
//     // Serial.println(accelZ_m_s2);
//     float estimated_y_acc = imuKalmanFilter.updateEstimate(accelY_m_s2);
//     unsigned long current_time = millis();
//     Serial.print(">estimated y_acc:");
//     Serial.println(estimated_y_acc);
//     float dv = (estimated_y_acc - baseline) * (current_time - prev_time) / 1000;
//     Serial.print(">dv:"); 
//     Serial.println(dv);
//     velocity += dv; 
//     // velocity += (accelY_m_s2 - baseline) * (current_time - prev_time) / 1000;
//     Serial.print(">velocity:");
//     Serial.println(velocity);
//     baseline = estimated_y_acc;
//     // Serial.print(">baseline:");
//     // Serial.println(baseline);
//     prev_time = current_time; 
//   }
// }
