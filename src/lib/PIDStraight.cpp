#include <Arduino.h>
#include "PIDStraight.h"  //if fails put "src/PIDRotate.h'
#include "IMU.h" //if fails put "src/IMU.h"
#include "Motors.h"
#include "PIDRotate.h"
#include "Encoder.h"
#define _USE_MATH_DEFINES
#include <math.h>

void PIDStraight::InputToMotor(double degree, double distance){
    encoder_instanceA.write(0);
    encoder_instanceB.write(0);
    while(currentError > 1){
    int motorInput = 0;

    double deltaError = 0;
    double deltaTime = 0;
    double deriv = 0;
    //360 ticks per 1 rotation. 
    // circumference of the wheel D = 4 cm piD;
    double distTraveled = 0;  //sub for current degree. 
    prevError = currentError;
    distTraveled = encoder_instanceA.read();//)/360) * (M_PI); //in centimeters
    Serial.print("distTraveled: ");
    Serial.print(distTraveled);


    currentError = ((distance * 360) / (M_PI * 5)) - distTraveled;
    deltaError = currentError - prevError;

    prevTime = currentTime;
    currentTime = micros();
    deltaTime = currentTime -prevTime;
    
    deriv = deltaError/deltaTime;

    integral += currentError;

    double out = Kp * currentError + Ki * integral + Kd * deriv;
    out *= 100;

    if(out < 0){
        //backwards
        Serial.println("Backwards I think");
        motors_instance->setRightMotorDirection(false); 
        motors_instance->setLeftMotorDirection(false);
    }

    else if(out > 0){
        //forwards
        Serial.println("Forwards I think");
        motors_instance->setRightMotorDirection(true); 
        motors_instance->setLeftMotorDirection(true);

    }

    Serial.println(out);
    Serial.println(currentError);
    Serial.println();


    if(((int)currentTime)%5 == 0){ //every 5 micros, recorrect the angle. 
    PIDRotate(degree); //correct the degree. 
    }
    motorInput = abs((int)out);
    motors_instance->setLeftMotorSpeed(motorInput);
    motors_instance->setRightMotorSpeed(motorInput);
    }

}
PIDStraight* pidstraight_instance = new PIDStraight();
