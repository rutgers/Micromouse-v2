/*#include "trueNorth.h"
#include "motors.h"

double trueNorth;

void setTrueNorth() {
    trueNorth = imu_instance->getHeading();
    return;
}

void makeTrueNorth() {
    int difference = trueNorth - imu_instance->getHeading();
    if (difference > 180.0) {
        difference -= 360.0;
    } else if (difference < -180.0) {
        difference += 360.0;
    }


    while(abs(difference) >= 0.5) {    
        if(difference >= 0.5) {
            motors_instance->setLeftMotorDirection(true);
            motors_instance->setLeftMotorSpeed(60);
            delay(10);
            motors_instance->setLeftMotorSpeed(0);
        } else if(difference <= -0.5) {
            motors_instance->setRightMotorDirection(false);
            motors_instance->setRightMotorSpeed(60);
            delay(10);
            motors_instance->setRightMotorSpeed(0);
        }
        delay(40);
    }


    return;
}
*/