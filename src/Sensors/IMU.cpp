#include <Sensors/IMU.hpp>
#include <Arduino.h>
#include <Wire.h>
#include <SimpleKalmanFilter.h>
#include <Constants.hpp>

// #define IMU_ADDR 0x70
#define ACC_CONF  0x20  //Page 91
#define GYR_CONF  0x21  //Page 93
#define CMD       0x7E  //Page 65
#define GRAVITY_EARTH  (9.80665f)

short x, y, z;

long prev_vel = 0;
long prev_ms = 0;
long prev_y = 0;

SimpleKalmanFilter imuKalmanFilter(1, 1, .1);
float baseline = 0.03; 
float velocity;
unsigned long prev_time;
//Write data in 16 bits
void writeRegister16(int16_t reg, int16_t value) {
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(reg);
  //Low 
  Wire.write((int16_t)value & 0xff);
  //High
  Wire.write((int16_t)value >> 8);
  Wire.endTransmission();
}

//Read data in 16 bits
int16_t readRegister16(int8_t reg) {
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  int n = Wire.requestFrom(IMU_ADDR, 4);  
  int16_t data[4];
  int i =0;
  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }  
  return (data[3]   | data[2] << 8);
}
static float lsb_to_mps2(int16_t val, int8_t g_range, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (GRAVITY_EARTH * val * g_range) / half_scale;
}

//Read all axis
void readAllAccel() {
  Wire.beginTransmission(IMU_ADDR);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(IMU_ADDR, 16);
  short data[16];
  int i =0;
  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }

  //Offset = 2 because the 2 first bytes are dummy (useless)
  int offset = 2;  
  x =             lsb_to_mps2(data[offset + 0]   | (short)data[offset + 1] << 8, 1, 16);  //0x03
  y =             lsb_to_mps2(data[offset + 2]   | (short)data[offset + 3] << 8, 1, 16);  //0x04
  z =             lsb_to_mps2(data[offset + 4]   | (short)data[offset + 5] << 8, 1, 16);  //0x05
}

void softReset(){  
  writeRegister16(CMD, 0xDEAF);
  delay(50);    
}

void imu_setup() {  
    //   Serial.begin(115200); 
    //   //Accelerometer
    //   Wire.begin();  
    Wire.setClock(400000);      // I2C Fast Mode (400kHz)  
    softReset();  
    /*
    * Acc_Conf P.91
    * mode:        0x7000  -> High
    * average:     0x0000  -> No
    * filtering:   0x0080  -> ODR/4
    * range:       0x0000  -> 2G
    * ODR:         0x000B  -> 800Hz
    * Total:       0x708B
    */
    writeRegister16(ACC_CONF,0x708C);//Setting accelerometer  
    /*
    * Gyr_Conf P.93
    * mode:        0x7000  -> High
    * average:     0x0000  -> No
    * filtering:   0x0080  -> ODR/4
    * range:       0x0000  -> 125kdps
    * ODR:         0x000B  -> 800Hz
    * Total:       0x708B
    */
    writeRegister16(GYR_CONF,0x708B);//Setting gyroscope    
}

void read_imu() {
    long current_ms = millis();
    long ms_dif = current_ms - prev_ms;
    prev_ms = current_ms;
    Serial.println("in read_imu");
    Serial.println(ms_dif);
    if(readRegister16(0x02) == 0x00) {
        //Read ChipID   
        readAllAccel();             // read all accelerometer/gyroscope/temperature data   

        // current_velocity = previous velocity + a*t
        long y_dif = y - prev_y;
        long vel = prev_vel + (y_dif * (ms_dif / 1000));  
        float estimated_y_acc = imuKalmanFilter.updateEstimate(y);
        unsigned long current_time = millis();
        float dv = (estimated_y_acc - baseline) * (current_time - prev_time) / 1000;
        Serial.print(">x:");
        Serial.print(x);
        Serial.print(">y:");
        Serial.print(y);
        Serial.print(">z:");
        Serial.print(z);
        Serial.print(">vel:");
        Serial.println(vel);

        prev_vel = vel;
        velocity += dv; 
        baseline = estimated_y_acc;
        prev_time = current_time; 
    }
}
