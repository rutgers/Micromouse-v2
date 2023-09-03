#ifndef PIDROTATE_H
#define PIDROTATE_H

#include <Arduino.h>

class PIDRotate {
    //this class will contain the relevant code for the PIDRotate command. This command will just use one PID loop based on the IMU heading
    public:
        void InputToMotor(double targetdegree);

        //below section is used for tuning, will return to it later. 
        const double Kp = 1.4; //proportional
        const double Ki = 0; //integral 
        const double Kd = 0; //derivative
    
    private: 
};

extern PIDRotate* pidrotate_instance;

#endif