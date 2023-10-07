
#ifndef PIDSTRAIGHT_H
#define PIDSTRAIGHT_H
#include "IMU.h"
#include "motors.h"
#include "tof.h"
#include "PidRotate.h"
class PIDstraight{
    public:
        void drive_to_position(double distance);
        void gointowall();
        PIDstraight();
        ~PIDstraight();
    private:
    tof* tof_instance;
    IMU* imu;
        double map(double error);

        int Proportion;
        int Integral = 0;
        int Derivative;
        int lasterror = 0;
        int leftpower = 50;
        int rightpower = 50;

         const double Kp = 6;
        const double Ki = 0.00007;
        const int Kd = 20;
};
// PIDstraight* pidstraight_instance;
#endif