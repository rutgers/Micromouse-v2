#include <Arduino.h>
#include "IMU.h"


/**
 *  Constructs IMU class
 */
IMU::IMU() {
    
    //Serial.begin(115200);

    // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
    //                                   id, address
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
    // while (!Serial) delay(10);  // wait for serial port to open!
    
    if (!bno.begin())
    {
        Serial.print("No BNO055 detected");
        while (1);
    }


    //delay(1000);
}


/**
 *  Returns the heading of the IMU
 *  returns a value between 0 and 360
 */
double IMU::getHeading() {
    sensors_event_t orientationData , linearAccelData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

    return orientationData.orientation.x;
}

// rounds the imu value to nearest 0, 90, 180, 270
double IMU::getCardinal() {

    double currAngle = imu_instance->getHeading();
    currAngle /= 90;
    int cardinalDir = round(currAngle);
 

    switch(cardinalDir) {
        //north = 0
        case 0:
            return 0;
        case 4:
            return 0;    
        //east
        case 1:
            return 90;
        case 2:
            return 180;
        case 3:
            return 270;
    }



}


IMU* imu_instance = new IMU();