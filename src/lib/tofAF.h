#ifndef tofAF_h
#define tofAF_h

#include <Arduino.h>
#include <VL6180X.h>
#include <VL53L1X.h>

class tofAF{
public:
    tofAF();
    int getreadings(int d);

private:
    void SetupVL6180xsensors();
    void SetupVL531lxsenors();
    void resetpins();
    uint8_t vl6180xpins[2] = {21,22}; //xshut pins for vl6180x sensors
    uint8_t vl53l1xpins[3] = {15,20,23}; //xshut pins fr vl53l1x sensors
    uint8_t VL6180xaddress[2] = {0x20,0x21}; //address for 6180
    uint8_t VL53l1xxaddress[3] = {0x22,0x23,0x24}; //address fr 53l1x sensors
    VL6180X vl6180x[2];
    VL53L1X VL53L1Xsensors[3];
};

#endif 