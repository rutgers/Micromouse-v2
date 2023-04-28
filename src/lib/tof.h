#ifndef tof_h
#define tof_h

#include <Arduino.h>
#include <VL6180X.h>
#include <VL53L1X.h>

struct readings{
    int left;
    int frontleft;
    int front;
    int frontright;
    int right;
};
struct connected{
    bool left;
    bool frontleft;
    bool front;
    bool frontright;
    bool right;
};



class tof{
    public:
        tof();
        void isConnected();

        int getLeftDistance();
        int getFrontDistance();
        int getRightDistance();
        int getFrontLeftDistance();
        int getFrontRightDistance();
    private:
        uint8_t xshut[5];
        uint8_t address[5];
        VL53L1X vl53l1x[3];
        VL6180X vl6180x[2];
        struct connected isworking;
};

extern tof* tof_instance;

#endif 
