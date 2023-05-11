#include <Arduino.h>
#include "PIDRotate.h"  //if fails put "src/PIDRotate.h'
#include "IMU.h" //if fails put "src/IMU.h"
#include "Motors.h"


void PIDRotate::InputToMotor(double targetdegree){
    int motorInput = 0;
    double deltaError = 0;
    double deltaTime = 0;
    double deriv = 0;

//    double currentDegree; moved to private in header
  //  double prevDegree; moved to private in header
while(abs(currentError) > 2){
    prevError = currentError; // the previous, currentDegree = current. 
    currentDegree = imu_instance->getHeading(); 
    currentError = targetdegree - currentDegree;//the e(t)
    deltaError = currentError - prevError; //change in error value, de
    
    //change in time. 
    prevTime = currentTime; //previous time
    currentTime = micros(); //current time 
    deltaTime = currentTime - prevTime; //chang in time, dt

    deriv = deltaError/deltaTime;

    integral += currentError; //integra = summation of every degree over the entire time. awful.

    double out = Kp * currentError + Ki * integral + Kd * deriv; // -27 or -26.99
    out *= 16; //or 25 for scaling, trial and error.

    //between x and 255
    

    if(out >= 255){
        out = 255;
    }

    if(out <= -255){
        out = -255;
    }

    if((out < 0)){
        //right
        //Serial.println("Right I think");
        motors_instance->setRightMotorDirection(false); 
        motors_instance->setLeftMotorDirection(true);
    }

    else if((out > 0)){
        //left
        //Serial.println("Left I think");
        motors_instance->setRightMotorDirection(true); 
        motors_instance->setLeftMotorDirection(false);

    }


    //Serial.print("HEWWO "); testing if current degree prints
    //Serial.println(currentDegree);


    motorInput = abs((int) out); //return only positive values

    //Serial.println(out);
    Serial.print("motor input:  ");
    Serial.print(motorInput);
    Serial.print("\tout:  ");
    Serial.println(out);


    motors_instance->setLeftMotorSpeed(motorInput);
    motors_instance->setRightMotorSpeed(motorInput);
    
    Serial.print(imu_instance->getHeading());
}
}
PIDRotate* pidrotate_instance = new PIDRotate();
