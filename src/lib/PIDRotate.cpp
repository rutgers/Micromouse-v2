#include <Arduino.h>
#include "PIDRotate.h"  //if fails put "src/PIDRotate.h'
#include "IMU.h" //if fails put "src/IMU.h"
#include "Motors.h"

// +90 is right, -90 is left
void PIDRotate::InputToMotor(double targetDegreeChange){
    int motorInput = 0;

    double deltaError;
    double deltaTime;
    double deriv;

    currentError = 90; //abritrary
    prevTime = micros();
    
    
    int targetDegree = imu_instance->getHeading() + targetDegreeChange; 
    currentDegree = imu_instance->getHeading();

    currentError = targetDegree - currentDegree;
    //converts possible overflow within the 0 to 360 range to positive difference
    if (currentError > 180.0) {
        currentError -= 360.0;
    } else if (currentError < -180.0) {
        currentError += 360.0;
    }

    double maxError = abs(currentError);


    while(abs(currentError) > 9.8) {
        currentDegree = imu_instance->getHeading(); 

        // derivative
        deltaError = currentError - prevError; //change in error value, de
        prevError = currentError; // the previous, currentDegree = current. 
        deltaTime = currentTime - prevTime; //chang in time, dt
        prevTime = micros();; //previous time
        deriv = deltaError/deltaTime;

        integral += currentError; //integra = summation of every degree over the entire time. awful.

        double out = Kp * currentError + Ki * integral + Kd * deriv; // -27 or -26.99
        Serial.println(Kd * deriv);
        Serial.print(out);
        Serial.print(", ");
        out = map(out, 0, maxError, 38, 68);
        Serial.println(out);

        if((currentError < 0)){
            //right
            //Serial.println("Right I think");
            motors_instance->setRightMotorDirection(true); 
            motors_instance->setLeftMotorDirection(false);
        }

        else if((currentError > 0)){
            //left
            //Serial.println("Left I think");
            motors_instance->setRightMotorDirection(false); 
            motors_instance->setLeftMotorDirection(true);

        }

        motorInput = abs((int) out); //return only positive values
        motors_instance->setLeftMotorSpeed(motorInput);
        motors_instance->setRightMotorSpeed(motorInput);
        


        currentError = targetDegree - currentDegree; //the e(t)
        if (currentError > 180.0) {
            currentError -= 360.0;
        } else if (currentError < -180.0) {
            currentError += 360.0;
        }
        currentError = currentError;
        currentTime = micros(); //current time 
    }

    motors_instance->setMotorsSpeed(0);
}

PIDRotate* pidrotate_instance = new PIDRotate();