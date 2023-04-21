#ifndef PIDROTATE_H
#define PIDROTATE_H

#include <Arduino.h>

class PIDRotate {
    //this class will contain the relevant code for the PIDRotate command. This command will just use one PID loop based on the IMU heading

    public:
        double kP = 0.05;
        double kI = 0.0;//0.00001;
        double kD = 0.0;


        int rotate_to_angle(double target_angle);

    private:
        double current_angle = 0;
        int current_time = 0;
        int prev_time = 0;
        double error = 0.0;
        double prev_error = 0.0;
        double total_error;

};

extern PIDRotate* pidrotate_instance;

#endif