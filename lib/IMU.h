#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#ifndef IMU_h
#define IMU_h

class IMU {

    double xPos = 0, yPos = 0, headingVel = 0;
    uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; //how often to read data from the board
    uint16_t PRINT_DELAY_MS = 10; // how often to print the data
    uint16_t printCount = 0; //counter to avoid printing every 10MS sample

    //velocity = accel*dt (dt in seconds)
    //position = 0.5*accel*dt^2
    double ACCEL_VEL_TRANSITION =  (double)(BNO055_SAMPLERATE_DELAY_MS) / 1000.0;
    double ACCEL_POS_TRANSITION = 0.5 * ACCEL_VEL_TRANSITION * ACCEL_VEL_TRANSITION;
    double DEG_2_RAD = 0.01745329251; //trig functions require radians, BNO055 outputs degrees

    // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
    //                                   id, address
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

    public:
        IMU();
        double getHeading();
};

#endif