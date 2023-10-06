#include <Arduino.h>
#include "PIDRotate.h"  //if fails put "src/PIDRotate.h'
#include "IMU.h" //if fails put "src/IMU.h"
#include "Motors.h"

//use absolute angle based on field 
void PIDRotate::InputToMotor(double targetDegree){
     
    double currentDegree = imu_instance->getHeading();

    double currentError = targetDegree - currentDegree;
    
    // angle wrap
    //converts possible overflow to between  -180 to 180
         if (currentError >  180.0) { currentError -= 360.0; }
    else if (currentError < -180.0) { currentError += 360.0; }
    //if currentError is +, we want to turn right

    while(abs(currentError) > 5) {

        double out = Kp * currentError;// + Ki * integral + Kd * deriv; // -27 or -26.99
       
        motors_instance->setLeftMotorSpeed(out);
        motors_instance->setRightMotorSpeed(-out);
        


        // currentError = targetDegree - currentDegree; //the e(t)
        // if (currentError > 180.0) {
        //     currentError -= 360.0;
        // } else if (currentError < -180.0) {
        //     currentError += 360.0;
        // }
        // currentError = currentError;
        // currentTime = micros(); //current time 

        // set old values

        // get new values
        currentDegree = imu_instance->getHeading(); 
        currentError = targetDegree - currentDegree;
        // angle wrap
             if (currentError >  180.0) { currentError -= 360.0; }
        else if (currentError < -180.0) { currentError += 360.0; }


    }
    
    motors_instance->setMotorsSpeed(0);
}

PIDRotate* pidrotate_instance = new PIDRotate();