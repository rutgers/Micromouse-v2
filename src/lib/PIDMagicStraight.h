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

    private:
        double current_angle = 0;
        int current_time = 0;
        int prev_time = 0;
        double error_l = 0.0;
        double error_r = 0.0;
        double prev_error_l = 0.0;
        double prev_error_r = 0.0;
        double total_error_l;
        double total_error_r;

        
        double current_position = 0;
        double error_pos = 0.0;
        double prev_error_pos = 0.0;
        double total_error_pos;

};

extern PIDMagicStraight* pidmagicstraight_instance;

#endif