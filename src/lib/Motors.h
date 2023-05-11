#ifndef MOTORS_H
#define MOTORS_H
#include <Encoder.h>
#include <Arduino.h>

#define pin_PWMA 4
#define pin_PWMB 9
#define pin_AIN1 7 //swapped ain1 = 6 and bin1 = 7 was
#define pin_AIN2 8 //swapped ain2 = 5 with bin2 = 8
#define pin_BIN1 6
#define pin_BIN2 5
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
        void setMotorsDirection(bool forward);

};

extern Motors* motors_instance;
extern Encoder encoder_instanceA;
extern Encoder encoder_instanceB;

#endif