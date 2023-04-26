#ifndef tofAF_h
#define tofAF_h

#include <Arduino.h>
#include <VL6180X.h>
#include <VL53L1X.h>

struct readings{
    int west;
    int northwest;
    int north;
    int northeast;
    int east;
};
struct connected{
    bool west;
    bool northwest;
    bool north;
    bool northeast;
    bool east;
};


class tofAF{
public:
    tofAF();
    readings getreadings();
    connected isconnected();
    connected con;
    readings read;
private:
    uint8_t xshut[5];
    uint8_t address[5];
    VL53L1X vl53l1x[3];
    VL6180X vl6180x[2];
};

#endif 
