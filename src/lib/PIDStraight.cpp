#include <Arduino.h>
#include "PIDStraight.h"  //if fails put "src/PIDRotate.h'
#include "IMU.h" //if fails put "src/IMU.h"
#include "Motors.h"
#include "PIDRotate.h"
#include "Encoder.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "tof.h"

void PIDStraight::InputToMotor(double desiredDistance) {
    //initialize current values

    // proportional
    // ===== distance =====
    
    // circumference = pi*d
    // number of circumferences = desiredDistance / circumference
    // 360 ticks/circumference 
    double desiredTicksDistance = ((desiredDistance * 360) / (M_PI * wheelDiameter));
    encoder_instanceA.write(0);
    encoder_instanceB.write(0);
    
    int curA, curB, currEnc;
    double currentErrorA = desiredTicksDistance;
    double currentErrorB = desiredTicksDistance;
    double currentError = (currentErrorA + currentErrorB) / 2;

    // ===== angle =====
    double keepAngle = imu_instance->getCardinal();
    double currentAngle = imu_instance->getHeading();
    double angleDiff = keepAngle - currentAngle;

    // // angle wrap
         if (angleDiff >  180.0) { angleDiff -= 360.0; }
    else if (angleDiff < -180.0) { angleDiff += 360.0; }      
    // // if angleDiff is -, we are too far to the right

    // derivative
    // double oldAngleDiff = angleDiff;
    // double oldTime = micros();
    // double derivativeAngleDiff = (angleDiff - oldAngleDiff) / ((micros() - oldTime) / 1e6);

    
    while(currentError > 5) {

        if(millis() % 20 == 0 && timeofflight_instance->readF() < 30) {
            break;
        }

        double outDistA = Kp * currentErrorA;// + Ki * integral + Kd * deriv;
        double outDistB = Kp * currentErrorB;// + Ki * integral + Kd * deriv;
        
        double outAngle = Kap * angleDiff;

        motors_instance->setRightMotorSpeed(40 + outDistA - outAngle);
        motors_instance->setLeftMotorSpeed(40 + outDistB + outAngle);

        // proportional
        // ------------ get new values ------------
        // ===== distance =====
        curA = encoder_instanceA.read();
        curB = encoder_instanceB.read();
        currEnc = (curA + curB) / 2;
        
        currentErrorA = desiredTicksDistance - curA;
        currentErrorB = desiredTicksDistance - curB;
        currentError = desiredTicksDistance - currEnc;

        //  ===== angle =====
        currentAngle = imu_instance->getHeading();
        angleDiff = keepAngle - currentAngle;
        // angle wrap
             if (angleDiff >  180.0) { angleDiff -= 360.0; }
        else if (angleDiff < -180.0) { angleDiff += 360.0; }   

        // derivative
        // derivativeAngleDiff = (angleDiff - oldAngleDiff) / ((micros() - oldTime) / 1e6);
        // oldAngleDiff = angleDiff;
        // oldTime = micros();

    }

    motors_instance->setMotorsSpeed(0);
    encoder_instanceA.write(0);
    encoder_instanceB.write(0);

    
}

PIDStraight* pidstraight_instance = new PIDStraight();
