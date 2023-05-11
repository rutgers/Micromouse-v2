#ifndef PIDROTATE_H
#define PIDROTATE_H

#include <Arduino.h>

class PIDRotate {
    //this class will contain the relevant code for the PIDRotate command. This command will just use one PID loop based on the IMU heading

    public:
        double kP = 0.25;//0.15;
        //double kI = 0.0;//0.0001;
        double kD = 0.0;

        bool exited = false;


        void rotate_to_angle(double target_angle, int boost, double threshold, double kI);

};

extern PIDRotate* pidrotate_instance;

#endif