#include "API.h"
#include <cstdlib>
#include <iostream>

double blockLength = 12.3;

bool API::wallFront() {
    int distance = timeofflight_instance->readF();
    if(distance < 150) {
        return true;
    } else {
        return false;
    }
}

bool API::wallRight() {
    int distance = timeofflight_instance->readR();
    if(distance < 150) {
        return true;
    } else {
        return false;
    }
}

bool API::wallLeft() {
    int distance = timeofflight_instance->readL();
    if(distance < 150) {
        return true;
    } else {
        return false;
    }
}


//  pidstraight_instance->InputToMotor(0,blocklength); //go 11 cm. works fine
//   pidrotate_instance->InputToMotor(-90); //left 90


void API::moveForward() {
    pidstraight_instance->InputToMotor(5,blockLength); //go 11 cm. works fine
    return;
}

void API::turnRight() {
    pidrotate_instance->InputToMotor(imu_instance->getHeading()+90); //right 90
    return;
}

void API::turnLeft() {
    pidrotate_instance->InputToMotor(imu_instance->getHeading()-90); //left 90
    return;
}
