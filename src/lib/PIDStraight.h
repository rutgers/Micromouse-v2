#ifndef PIDSTRAIGHT_H
#define PIDSTRAIGHT_H

#include <Arduino.h>

class PIDStraight {
    //this class will contain the relevant code for the PIDStraight command. This command will just use two nested PID loops, the inner
    //using the "black magic" straight drive trick using the IMU heading, and the outer scaling it by the encoder values.

    public:
        double kP = 0.1;
        double kI = 0.0;//0.00001;
        double kD = 0.0;


        int drive_to_position(double target_position);

    private:
        double current_position = 0;
        int current_time = 0;
        int prev_time = 0;
        double error = 0.0;
        double prev_error = 0.0;
        double total_error;
};

extern PIDStraight* pidstraight_instance;

#endif