#include <Arduino.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/LEDDriver.hpp"

// #define DEV_I2C Wire

// Components.
int interruptCount=0;
VL53L1 left_vl53l1(&DEV_I2C, XSHUT_LEFT);
// VL53L1 right_vl53l1(&DEV_I2C, XSHUT_RIGHT);

// VL53L4CX left_vl53l1(&DEV_I2C, XSHUT_LEFT);
// VL53L4CX right_vl53l1(&DEV_I2C, XSHUT_RIGHT);


// VL53L1 vl53l1_list[] = {left_vl53l1, right_vl53l1};
VL53L1 vl53l1_list[] = {left_vl53l1};


int num_sensors = sizeof(vl53l1_list)/sizeof(vl53l1_list[0]);

void measure() {
    interruptCount=1;
}

void blindspot_setup() {
    // Configure VL53L1 satellite component.
    left_vl53l1.begin();
    
    // right_vl53l1.begin();

    // Switch off VL53L1 satellite component.
    left_vl53l1.VL53L1_Off();
    // right_vl53l1.VL53L1_Off();

    //Initialize VL53L1 satellite component.
    left_vl53l1.InitSensor(LEFT_BLINDSPOT_ADDR);
    // right_vl53l1.InitSensor(RIGHT_BLINDSPOT_ADDR);

    left_vl53l1.VL53L1_SetPresetMode(VL53L1_PRESETMODE_RANGING);
    left_vl53l1.VL53L1_ClearInterruptAndStartMeasurement();
    // right_vl53l1.VL53L1_SetPresetMode(VL53L1_PRESETMODE_RANGING);
    // right_vl53l1.VL53L1_ClearInterruptAndStartMeasurement();

}

void get_measurement(VL53L1_MultiRangingData_t *pMultiRangingData, VL53L1& vl53l1_sensor, int direction) {
    char report[64];
    int status = vl53l1_sensor.VL53L1_GetMultiRangingData(pMultiRangingData);
    Serial.print("GetMultiRangingData status: ");
    Serial.println(status);
    if (status < 0) {
        // Serial.println(status);
        Serial.println("NEGATIVE ERROR");
    }

    int no_of_object_found = pMultiRangingData->NumberOfObjectsFound;

    snprintf(report, sizeof(report), "VL53L1 Satellite: Count=%d, #Objs=%1d ", pMultiRangingData->StreamCount, no_of_object_found);
    Serial.print(report);
    int num_in_blindspot = 0;
    for (int i = 0; i < no_of_object_found; i++) {        
        // determine if it is in the blindspot
        if (pMultiRangingData->RangeData[i].RangeMilliMeter < BLINDSPOT_MM) {
            num_in_blindspot++;
        }

        if (i != 0) {
            Serial.print("\r\n                               ");
        }
        Serial.print("status=");
        Serial.print(pMultiRangingData->RangeData[i].RangeStatus);
        Serial.print(", D=");
        Serial.print(pMultiRangingData->RangeData[i].RangeMilliMeter);
        Serial.print("mm");
        Serial.print(", Signal=");
        Serial.print((float)pMultiRangingData->RangeData[i].SignalRateRtnMegaCps / 65536.0);
        Serial.print(" Mcps, Ambient=");
        Serial.print((float)pMultiRangingData->RangeData[i].AmbientRateRtnMegaCps / 65536.0);
        Serial.print(" Mcps");
    }

    if (num_in_blindspot > 0) {
        turn_on_blindspot_leds(direction);
    } else {
        turn_off_blindspot_leds(direction);
    }

    Serial.println("");
    // if measurement is valid, clear and prep for new measurement
    if (status == 0) {
        status = vl53l1_sensor.VL53L1_ClearInterruptAndStartMeasurement();
        Serial.print("ClearInterruptAndStartMeasurement status: ");
        Serial.println(status);
    }
}

void blindspot_detect() {
    for (int vl53l1_idx = 0; vl53l1_idx < num_sensors; vl53l1_idx++) { 
        VL53L1_MultiRangingData_t MultiRangingData;
        VL53L1_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
        uint8_t NewDataReady = 0;
        int no_of_object_found = 0, j;
        char report[64];
        if (interruptCount) {
            int status;
            interruptCount = 0;
        
            do {
                status = vl53l1_list[vl53l1_idx].VL53L1_GetMeasurementDataReady(&NewDataReady);
                Serial.print("GetMeasurementDataReady status: ");
                Serial.println(status);
            } while (!NewDataReady);
            if ((!status) && (NewDataReady != 0)) {
                get_measurement(pMultiRangingData, vl53l1_list[vl53l1_idx], vl53l1_idx);
            }
        }
    }
}
