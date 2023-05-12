#ifndef PIDROTATE_H
#define PIDROTATE_H

#include <Arduino.h>

class PIDRotate {
    //this class will contain the relevant code for the PIDRotate command. This command will just use one PID loop based on the IMU heading
    public:
        void InputToMotor(double targetdegree);

        //below section is used for tuning, will return to it later. 
        const double Kp = 0.0000005; //proportional
        const double Ki = 0; //integral 
        const double Kd = 2000; //derivative
    
    private: 
        double currentDegree = 0;
        double currentError = 5;
        double prevError = 0;
        double currentTime = 0;
        double prevTime = 0;
        double integral  = 0;
};

extern PIDRotate* pidrotate_instance;

#endif