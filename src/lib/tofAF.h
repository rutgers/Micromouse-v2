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
    uint8_t xshut[5];
    uint8_t address[5];
    VL6180X vl6180x[2];
};

#endif 