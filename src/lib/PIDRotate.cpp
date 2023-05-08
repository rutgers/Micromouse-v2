#include <math.h>
#include <Arduino.h>
#include "PIDRotate.h"
#include "Motors.h"
#include "IMU.h"

void PIDRotate::rotate_to_angle(double target_angle) {

    double current_angle = 0;
    int current_time = 0;
    int prev_time = 0;
    double error = 6.0;
    double prev_error = 0.0;
    double total_error = 0.0;
    exited = false;

    while (abs(error) > 2.0) {
        current_angle = imu_instance->getHeading();

        // handle cases of numbers above 360

        // for example, the number -718 will become 718, will become -358
        

        prev_error = error;
        error = target_angle - current_angle;

        error = fmod(error, 360.0); // keep between 0 and 360 or 0 and -360

        if (error > 180.0) {
            error = -360.0 + error;
        } else if (error < -180.0) {
            error = 360.0 + error;
        }

        total_error += abs(error);
        prev_time = current_time;
        current_time = millis();

        //int target_angle = current_angle + angle;

        Serial.print("current angle: ");
        Serial.print(current_angle);
        Serial.println();

        /*Serial.print("target angle: ");
        Serial.print(target_angle);
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

        double motor_output = (kP * error  + kD * (error-prev_error)/(current_time-prev_time) + kI * total_error)*25;
        
        if (error > 0.0) {
            motors_instance->setLeftMotorDirection(true);//false
            Serial.println("left false and right true");
            motors_instance->setRightMotorDirection(false);//true
        } else {
            motors_instance->setLeftMotorDirection(false);//true
            Serial.println("left true and right false");
            motors_instance->setRightMotorDirection(true);//false
            motor_output *= -1;
        }

        if (motor_output > 255.0) {
            motor_output = 255.0;
        }

        Serial.print("motor output: ");
        Serial.print(motor_output);
        Serial.println();

        //return motor_output;

        motors_instance->setMotorsSpeed((int)floor(motor_output));
    
    }

    exited = true;
    motors_instance->setMotorsSpeed(0.0);
    return;
}

PIDRotate* pidrotate_instance = new PIDRotate();