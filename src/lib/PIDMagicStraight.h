#ifndef PIDMAGICSTRAIGHT_H
#define PIDMAGICSTRAIGHT_H

#include <Arduino.h>

class PIDMagicStraight {
    //this class will contain the relevant code for the PIDRotate command. This command will just use one PID loop based on the IMU heading

    public:
        double kP = 0.05;
        double kI = 0.0;//0.00001;
        double kD = 0.0;


        void drive_straight_with_magic(double target_position);

        bool exited = false;

};

extern PIDMagicStraight* pidmagicstraight_instance;

#endif