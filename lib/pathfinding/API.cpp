#include "API.h"
#include <cstdlib>
#include <iostream>

#include "..\src\lib\tof.h"
#include "..\src\lib\PIDRotate.h"
#include "..\src\lib\PIDStraight.h"

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


void API::moveForward() {
    pidstraight_instance->InputToMotor(0,blockLength); //go 11 cm. works fine
    return;
}

void API::turnRight() {
    pidrotate_instance->InputToMotor(90); //right 90
    return;
}

void API::turnLeft() {
    pidrotate_instance->InputToMotor(-90); //left 90
    return;
}
