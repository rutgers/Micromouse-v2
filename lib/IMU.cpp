#include "Arduino.h"
#include "IMU.h"

IMU::IMU() {
    
    Serial.begin(115200);

    // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
    //                                   id, address
    Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
    while (!Serial) delay(10);  // wait for serial port to open!
    
    if (!bno.begin())
    {
        Serial.print("No BNO055 detected");
        while (1);
    }


    delay(1000);
}

double IMU::getHeading() {
    sensors_event_t orientationData , linearAccelData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

    return orientationData.orientation.x;
}