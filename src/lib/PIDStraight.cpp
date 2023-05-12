#include <Arduino.h>
#include "PIDStraight.h"  //if fails put "src/PIDRotate.h'
#include "IMU.h" //if fails put "src/IMU.h"
#include "Motors.h"
#include "PIDRotate.h"
#include "Encoder.h"
#define _USE_MATH_DEFINES
#include <math.h>

void PIDStraight::InputToMotor(double distance){ //imu heading. 
    encoder_instanceA.write(0);
    encoder_instanceB.write(0);
    // int keepAngle = imu_instance->getHeading();

    int curA = 0;
    int curB = 0;
    int prevA = 0;
    int prevB = 0;
    double distTraveled = 0;
    int motorInput;
    prevTime = micros();
    currentError = ((distance * 360) / (M_PI * wheelDiameter)) - distTraveled;
    
    double maxError = ((distance * 360) / (M_PI * 4));

    
    while(currentError > 1){
        // Serial.print("currenterror");
        // Serial.println (currentError);

        double deltaError = 0;
        double deltaTime = 0;
        double deriv = 0;
        //360 ticks per 1 rotation. 
        // circumference of the wheel D = 4 cm piD;
        
        distTraveled = encoder_instanceB.read();//)/360) * (M_PI); //in centimeters
        // Serial.print("distTraveled: ");
        // Serial.print(distTraveled);

        deltaError = currentError - prevError;
        deltaTime = currentTime - prevTime;
        prevError = currentError;
        prevTime = micros();        
        deriv = deltaError/deltaTime;

        integral += currentError;

        prevA = curA;    //exit counter
        prevB = curB;
        curA = encoder_instanceA.read();
        curB = encoder_instanceB.read();

        int offset = 0;

        if ((curA - prevA) > (curB - prevB)) {
            offset = 10;
        } else {
            offset = -10;
        } //end of exit counter

        //if ()

        double out = Kp * currentError + Ki * integral + Kd * deriv;
        // out *= 100;
        //map out to a value from 60 to 255 for motor thresholds
        out = map(out, 0, maxError, 60, 255);
        // Serial.println(out);

        if(out < 0){
            //backwards
            // Serial.println("Backwards ????I think");
            motors_instance->setRightMotorDirection(true); 
            motors_instance->setLeftMotorDirection(true);
        }

        else if(out > 0){
            //forwards
            // Serial.println("Forwards? I think");
            motors_instance->setRightMotorDirection(false); 
            motors_instance->setLeftMotorDirection(false);

        }

        //Serial.println(out);
        // Serial.println(currentError);
        // Serial.println();
        // Serial.print(encoder_instanceA.read()); //right
        //     Serial.println("\n");
        // Serial.println(encoder_instanceB.read()); //left
        //     Serial.println("\n");

        if(((int)currentTime)%5 == 0){ //every 5 micros, recorrect the angle. 
        // PIDRotate(degree); //correct the degree. 
        }
        motorInput = abs((int)out);
        motors_instance->setLeftMotorSpeed((motorInput-10+offset));
        motors_instance->setRightMotorSpeed((motorInput-10+offset));

        currentError = ((distance * 360) / (M_PI * 4)) - distTraveled;
        currentTime = micros();
    }

    motors_instance->setMotorsSpeed(0);
    encoder_instanceA.write(0);
    encoder_instanceB.write(0);
    
}
PIDStraight* pidstraight_instance = new PIDStraight();
