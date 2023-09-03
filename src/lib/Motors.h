#ifndef MOTORS_H
#define MOTORS_H
#include <Encoder.h>
#include <Arduino.h>

#define pin_PWMA 4 // right
#define pin_PWMB 9 // left
#define pin_AIN1 5 
#define pin_AIN2 6 
#define pin_BIN1 7
#define pin_BIN2 8
#define pin_STBY 14

class Motors {

    public:

        void enableMotors();
        void disableMotors();
        void setLeftMotorSpeed(int speed);
        void setRightMotorSpeed(int speed);
        void setMotorsSpeed(int speed);
        void setLeftMotorDirection(bool forward);
        void setRightMotorDirection(bool forward);
};

extern Motors* motors_instance;
extern Encoder encoder_instanceA;
extern Encoder encoder_instanceB;

#endif