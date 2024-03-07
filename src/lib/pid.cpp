#include "pid.h"



void turnTo(char direction) {
    double targetDirection;

    switch(direction) {
        case 'N':
            targetDirection = 0;
            break;
        case 'S':
            targetDirection = 180;
            break;
        case 'E':
            targetDirection = 90;
            break;
        case 'W':
            targetDirection = 270;
            break;
    }

    double error;

    error = targetDirection - angle();
    


    if(error < 0) error += 360;
    if (error > 180) error -= 360;

    // positive error means we are left of the desired angle
    // negative error means we are right of the desired angle

    Serial.println(error);
}