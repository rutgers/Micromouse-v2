#include <math.h>
#include <Arduino.h>
#include "PIDStraight.h"
#include "Motors.h"
#include "IMU.h"

void PIDStraight::drive_to_position(double target_position) {
    double current_position = 0;
    int current_time = 0;
    int prev_time = 0;
    double error = 0.0;
    double prev_error = 0.0;
    double total_error;

    bool exited = false;
        
    ENCA.write(0);
    ENCB.write(0);

    while (true) {
        //current_position = imu_instance->getHeading();
        current_position = (ENCA.read() + ENCB.read())/2;

        // handle cases of numbers above 360

        // for example, the number -718 will become 718, will become -358
        

        prev_error = error;
        error = (target_position * 58.2124805) - current_position; //72.74463

         if (abs(error) < 10.0 ) {
            Serial.println("breaking");
            break;
        }

        total_error += abs(error);
        prev_time = current_time;
        current_time = millis();

        //int target_position = current_position + position;

        Serial.print("current position: ");
        Serial.print(current_position);
        Serial.println();

        /*Serial.print("target position: ");
        Serial.print(target_position);
        Serial.println();*/


        /*if (error > 180.0) {
            error -= 180.0;
        }

        if (error < -180.0) {
            error += 180.0;
        }*/

        Serial.print("error: ");
        Serial.print(error);
        Serial.println();

        //double motor_output = (kP * error  + kD * (error-prev_error)/(current_time-prev_time) + kI * total_error)*25;
        double motor_output = (kP * error)*25;
        
        if (error > 0.0) {
            motors_instance->setLeftMotorDirection(true);//false
            Serial.println("left false and right true");
            motors_instance->setRightMotorDirection(true);//true
        } else {
            motors_instance->setLeftMotorDirection(false);//true
            Serial.println("left true and right false");
            motors_instance->setRightMotorDirection(false);//false
            motor_output *= -1;
        }

        if (motor_output > 255.0) {
            motor_output = 255.0;
        }

        Serial.print("motor output: ");
        Serial.print(motor_output);
        Serial.println();

        motors_instance->setMotorsSpeed(motor_output);
    }

    exited = true;
    motors_instance->setMotorsSpeed(0.0);
    Serial.print("exited: ");
    Serial.println(exited);
    
    
}

PIDStraight* pidstraight_instance = new PIDStraight();