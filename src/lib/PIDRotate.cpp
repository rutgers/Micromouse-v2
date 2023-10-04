#include <Arduino.h>
#include "PIDRotate.h"  //if fails put "src/PIDRotate.h'
#include "IMU.h" 
#include "Motors.h"

//use absolute angle based on field 
void PIDRotate::InputToMotor(double targetDegree){
    
    double startTime = millis() / 1e3;

    // proportional
    double currentDegree = imu_instance->getHeading();
    double currentError = targetDegree - currentDegree;
    
    // angle wrap
    //converts possible overflow to between  -180 to 180
         if (currentError >  180.0) { currentError -= 360.0; }
    else if (currentError < -180.0) { currentError += 360.0; }
    //if currentError is +, we want to turn right

    
    // derivative
    // double oldError = currentError;
    // double oldTime = millis();
    // double derivativeError = 0;


    while(abs(currentError) > 5) {

        if((millis() / 1e3) - startTime > 2) {
            // exit loop after 2 seconds
            break;
        }

        double out = Kp * currentError;// + Ki * integral + Kd * deriv; // -27 or -26.99
       
        motors_instance->setLeftMotorSpeed(out);
        motors_instance->setRightMotorSpeed(-out);
        


        // get new values
        currentDegree = imu_instance->getHeading(); 
        currentError = targetDegree - currentDegree;
        // angle wrap
             if (currentError >  180.0) { currentError -= 360.0; }
        else if (currentError < -180.0) { currentError += 360.0; }

        // derivative
        // derivativeError = (currentError - oldError) / ((millis() - oldTime) / 1e3);
        // oldError = currentError;
        // oldTime = millis();


    }
    
    motors_instance->setMotorsSpeed(0);
}

PIDRotate* pidrotate_instance = new PIDRotate();