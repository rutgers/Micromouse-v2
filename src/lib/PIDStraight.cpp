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

    // ===== distance =====
    
    // circumference = pi*d
    // number of circumferences = desiredDistance / circumference
    // 360 ticks/circumference 
    double desiredTicksDistance = ((desiredDistance * 360) / (M_PI * wheelDiameter));
    encoder_instanceA.write(0);
    encoder_instanceB.write(0);
    
    // ===== angle =====
    double keepAngle = imu_instance->getCardinal();
    double currentAngle = imu_instance->getHeading();
    double angleDiff = keepAngle - currentAngle;

    // // angle wrap
         if (angleDiff >  180.0) { angleDiff -= 360.0; }
    else if (angleDiff < -180.0) { angleDiff += 360.0; }      
    // // if angleDiff is -, we are too far to the right

    // ===== wall distance =====
    // double leftDist = timeofflight_instance->readL();
    // double rightDist = timeofflight_instance->readR();

    /*
    // valid wall checking ranges from 20 to 60
    if(leftDist > 60) leftDist = 38;
    if(rightDist > 60) rightDist = 38;


    
    // ===== front wall distance ====
    // distance to front wall should always be greater than 20 (wall), with 255 (no wall)
    double frontDist = timeofflight_instance->readF();

    */

    //initialize old values
    // int prevEnc = currEnc;
    // double oldError = currentError;


    int curA, curB, currEnc;
    double currentErrorA = desiredTicksDistance;
    double currentErrorB = desiredTicksDistance;
    double currentError = (currentErrorA + currentErrorB) / 2;
    
    while(currentError > 10) {
    // while(currentError > 10) {

        Serial.println(angleDiff);

        double outDistA = Kp * currentErrorA;// + Ki * integral + Kd * deriv;
        double outDistB = Kp * currentErrorB;// + Ki * integral + Kd * deriv;
        
        double outAngle = Kap * angleDiff;

        // double closeLeft = 0.7*(38-leftDist);
        // double closeRight = 0.7*(38-rightDist);
        
        motors_instance->setRightMotorSpeed(40 + outDistA - outAngle);
        motors_instance->setLeftMotorSpeed(40 + outDistB + outAngle);

        // motors_instance->setRightMotorSpeed(outDistA);        
        // motors_instance->setLeftMotorSpeed(outDistB);

        // set old values

        // ------------ get new values ------------
        // ===== distance =====
        curA = encoder_instanceA.read();
        curB = encoder_instanceB.read();
        currEnc = (curA + curB) / 2;

        currentError = desiredTicksDistance - currEnc;
        // currentErrorA = desiredTicksDistance - curA;
        // currentErrorB = desiredTicksDistance - curB;

        //  ===== angle =====
        currentAngle = imu_instance->getHeading();
        angleDiff = keepAngle - currentAngle;
        // angle wrap
             if (angleDiff >  180.0) { angleDiff -= 360.0; }
        else if (angleDiff < -180.0) { angleDiff += 360.0; }   

        // ===== wall distance =====
        //  leftDist = timeofflight_instance->readL();
        //  rightDist = timeofflight_instance->readR();

        // valid wall checking ranges from 20 to 60
        // if(leftDist > 60) leftDist = 38;
        // if(rightDist > 60) rightDist = 38;
        
        // ===== front wall distance ====
        // distance to front wall should either be 255 (no wall) or 20 (wall)
        //  frontDist = timeofflight_instance->readF();

    }

    motors_instance->setMotorsSpeed(0);
    encoder_instanceA.write(0);
    encoder_instanceB.write(0);

    
}

PIDStraight* pidstraight_instance = new PIDStraight();
