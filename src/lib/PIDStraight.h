#ifndef PIDSTRAIGHT_H
#define PIDSTRAIGHT_H
#include <Encoder.h>
#include <Arduino.h>
#include "PIDRotate.h"

class PIDStraight {
    //this class will contain the relevant code for the PIDStraight command. This command will just use two nested PID loops, the inner
    //using the "black magic" straight drive trick using the IMU heading, and the outer scaling it by the encoder values.
    //fuck your black magic. 
    public:
        void InputToMotor(double distance);

        const double Kp = 0.0005;
        const double Ki = 0;
        const double Kd = 10;


        const double wheelDiameter = 3.9;
    private:
        double currentDegree = 0;
        double currentError = 30;
        double prevError = 0;
        double currentTime = 0;
        double prevTime = 0;
        double integral = 0;

};

extern PIDStraight* pidstraight_instance;

#endif