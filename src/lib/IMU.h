#ifndef IMU_h
#define IMU_h

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>


class IMU {

    // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
    //                                   id, address
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

    public:
        IMU();
        double getHeading();
        double getCardinal();
};

extern IMU* imu_instance;

#endif