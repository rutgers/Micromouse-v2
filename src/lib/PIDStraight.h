#ifndef PIDSTRAIGHT_H
#define PIDSTRAIGHT_H
#include <Encoder.h>
#include <Arduino.h>

class PIDStraight {
    //this class will contain the relevant code for the PIDStraight command. This command will just use two nested PID loops, the inner
    //using the "black magic" straight drive trick using the IMU heading, and the outer scaling it by the encoder values.
    //fuck your black magic. 
    public:
        void InputToMotor(double degree, double distance);

        const double Kp = 0.1;
        const double Ki = 0.1;
        const double Kd = 0.1;

    private:
        double currentDegree = 0;
        double currentError = 10;
        double prevError = 0;
        double currentTime = 0;
        double prevTime = 0;
        double integral = 0;

};

extern PIDStraight* pidstraight_instance;

#endif