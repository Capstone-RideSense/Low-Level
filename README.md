# RideSense
RideSense is a haptic navigation system for bikes. As a user approaches a turn, the corresponding handle will vibrate, ramping up in intensity the closer they are. Additioanly safety features such as blind spot detection and blinkers are also implemented. 

## Technical Overview 
The main controller is an ESP32 which connects to an app via Bluetooth. The ESP32, sensors and LED drivers communicate via I2C. 
### Blind Spot Detection 
The VL53L1X Time of Flight sensor was used for blind spot detection. Each bike handlebar has a ToF sensor mounted as to not interfere with the rider. If it detects an object within the distance threshold, it turns the blind spot LEDs on, alerting the user and surrounding drivers. 
### LED Driver 
The LEDs are driven using the PCA9955BTW LED Driver. This handles the control for the blind spot and blinker LEDs 


