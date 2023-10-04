#include "API.h"
#include <cstdlib>
#include <iostream>


bool API::wallFront() {
    int distance = timeofflight_instance->readF();
    if(distance < 100) {
        return true;
    } else {
        return false;
    }
}

bool API::wallRight() {
    int distance = timeofflight_instance->readR();
    if(distance < 100) {
        return true;
    } else {
        return false;
    }
}

bool API::wallLeft() {
    int distance = timeofflight_instance->readL();
    if(distance < 100) {
        return true;
    } else {
        return false;
    }
}


//  pidstraight_instance->InputToMotor(0,blocklength); //go 11 cm. works fine
//   pidrotate_instance->InputToMotor(-90); //left 90
double blockLength = 16.8;

void API::moveForward() {
    pidstraight_instance->InputToMotor(blockLength); //go 11 cm. works fine
    delay(300);
    return;
}


void API::turnRight() {
    double fieldAngle = imu_instance->getCardinal();


    pidrotate_instance->InputToMotor(fieldAngle+90); //right 90
    delay(300);
    return;
}

void API::turnLeft() {
    double fieldAngle = imu_instance->getCardinal();
    
    pidrotate_instance->InputToMotor(fieldAngle-90); //left 90
    delay(300);
    return;
}

void API::turnAround() {
    double fieldAngle = imu_instance->getCardinal();
    pidrotate_instance->InputToMotor(fieldAngle+180); //180
    delay(300);
    return;
}
