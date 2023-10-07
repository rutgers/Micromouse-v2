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
double IMU::getHeadingFast() {
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    if(orientationData.orientation.x == 0) {
        return getHeading();

    }

    return orientationData.orientation.x;
}


double IMU::getHeading() {
    sensors_event_t orientationData0;
    sensors_event_t orientationData1;
    sensors_event_t orientationData2;
    sensors_event_t orientationData3;
    sensors_event_t orientationData4;
    sensors_event_t orientationData5;
    sensors_event_t orientationData6;
    sensors_event_t orientationData7;
    sensors_event_t orientationData8;
    sensors_event_t orientationData9;
    

    bno.getEvent(&orientationData0, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData1, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData2, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData3, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData4, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData5, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData6, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData7, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData8, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&orientationData9, Adafruit_BNO055::VECTOR_EULER);
    

    std::vector<double> angleReadings = {
        orientationData0.orientation.x,
        orientationData1.orientation.x,
        orientationData2.orientation.x,
        orientationData3.orientation.x,
        orientationData4.orientation.x,
        orientationData5.orientation.x,
        orientationData6.orientation.x,
        orientationData7.orientation.x,
        orientationData8.orientation.x,
        orientationData9.orientation.x
        };
    
    return findMostProminentAngle(angleReadings);
}


double IMU::findMostProminentAngle(const std::vector<double>& angleReadings) {
    std::map<double, int> angleCount;
    
    // Count occurrences of each angle
    for (const double& angle : angleReadings) {
        angleCount[angle]++;
    }
    
    // Find the angle with the highest count
    double mostProminentAngle = angleReadings[0];  // Default to the first reading
    int maxCount = angleCount[mostProminentAngle];
    
    for (const auto& pair : angleCount) {
        if (pair.second > maxCount) {
            mostProminentAngle = pair.first;
            maxCount = pair.second;
        }
    }
    
    return mostProminentAngle;
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


    return 404;
}


IMU* imu_instance = new IMU();