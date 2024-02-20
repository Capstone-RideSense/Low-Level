#include <Arduino.h>
#include "Sensors/Blindspot.hpp"
#include "LEDs/BlindspotLED.hpp"
#include "Constants.hpp"
#include <Wire.h>
#include <vl53l1_class.h>
// #include <vl53l4cx_class.h>

#define DEV_I2C Wire

// Components.
// VL53L4CX sensor_vl53l4cx_sat(&DEV_I2C, XSHUT);
VL53L1 sensor_vl53l1_sat(&DEV_I2C, XSHUT);


void blindspot_setup() {
    DEV_I2C.setClock(1000000);  

    // Initialize I2C bus.
    DEV_I2C.begin();

    // Configure VL53L1 satellite component.
    // sensor_vl53l4cx_sat.begin();
    sensor_vl53l1_sat.begin();

    // Switch off VL53L1 satellite component.
    // sensor_vl53l4cx_sat.VL53L4CX_Off();
    sensor_vl53l1_sat.VL53L1_Off();

    //Initialize VL53L1 satellite component.
    // sensor_vl53l4cx_sat.InitSensor(0x12);
    sensor_vl53l1_sat.InitSensor(0x12);

    // Start Measurements
    // with the correct ToF sensor: -- NOTE NOT SURE IF WE NEED THE PRESET MODE???
    // sensor_vl53l1_sat.VL53L1_SetPresetMode(VL53L1_PRESETMODE_MULTIZONES_SCANNING);
    // would need to define the zones - something like this:
    // VL53L1_RoiConfig_t RoiConfig;
    // RoiConfig.UserRois[0].TopLeftX = 9;
    // RoiConfig.UserRois[0].TopLeftY = 13;
    // RoiConfig.UserRois[0].BotRightX = 14;
    // RoiConfig.UserRois[0].BotRightY = 10;
    // Status = VL53L1_SetUserROI(Dev, &RoiConfig);

    // sensor_vl53l4cx_sat.VL53L4CX_StartMeasurement();
    sensor_vl53l1_sat.VL53L1_SetPresetMode(VL53L1_PRESETMODE_RANGING);
    sensor_vl53l1_sat.VL53L1_ClearInterruptAndStartMeasurement();
}

void get_measurement(VL53L1_MultiRangingData_t *pMultiRangingData) {
    char report[64];
    int status = sensor_vl53l1_sat.VL53L1_GetMultiRangingData(pMultiRangingData);
    // int status = sensor_vl53l4cx_sat.VL53L4CX_GetMultiRangingData(pMultiRangingData);
    int no_of_object_found = pMultiRangingData->NumberOfObjectsFound;

    snprintf(report, sizeof(report), "VL53L1 Satellite: Count=%d, #Objs=%1d ", pMultiRangingData->StreamCount, no_of_object_found);
    Serial.print(report);
    int num_in_blindspot = 0;

    for (int i = 0; i < no_of_object_found; i++) {
        // TODO: process measurement
        
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
        turn_on_blindspot_leds();
    } else {
        turn_off_blindspot_leds();
    }

    Serial.println("");
    // if measurement is valid, clear and prep for new measurement
    if (status == 0) {
        status = sensor_vl53l1_sat.VL53L1_ClearInterruptAndStartMeasurement();
        // status = sensor_vl53l4cx_sat.VL53L4CX_ClearInterruptAndStartMeasurement();
    }
}

void blindspot_detect() {
    VL53L1_MultiRangingData_t MultiRangingData;
    VL53L1_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
    // VL53L4CX_MultiRangingData_t MultiRangingData;
    // VL53L4CX_MultiRangingData_t *pMultiRangingData = &MultiRangingData;
    uint8_t NewDataReady = 0;
    int no_of_object_found = 0, j;
    char report[64];
    int status;

    do {
        status = sensor_vl53l1_sat.VL53L1_GetMeasurementDataReady(&NewDataReady);
        // status = sensor_vl53l4cx_sat.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
    } while (!NewDataReady);

    if ((!status) && (NewDataReady != 0)) {
        get_measurement(pMultiRangingData);
    }

}