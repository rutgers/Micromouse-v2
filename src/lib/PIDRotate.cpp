#include <math.h>
#include <Arduino.h>
#include "PIDRotate.h"
#include "Motors.h"
#include "IMU.h"

void PIDRotate::rotate_to_angle(double target_angle, int boost, double threshold, double kI) {

    double current_angle = 0;
    int current_time = 0;
    int prev_time = 0;
    double error = 6.0;
    double prev_error = 0.0;
    double total_error = 0.0;
    exited = false;

    int curA = 0;
    int curB = 0;
    int prevA = 0;
    int prevB = 0;

    int ABdiff = 0;

    int adj_offset = 0;

    int exitCount = 0;

    while (true) {

        prevA = curA;
        prevB = curB;
        curA = ENCA.read();
        curB = ENCA.read();
        ABdiff = (curA-prevA) - (curB-prevB);

        adj_offset = ABdiff * 1;

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

        total_error += error;
        prev_time = current_time;
        current_time = millis();

        //int target_angle = current_angle + angle;

        Serial.print("current angle: ");
        Serial.print(current_angle);
        Serial.println();

        if (abs(error) < 0.5) {
            exitCount++;
        }

        if (exitCount > 10) {
            Serial.println("breaking");
            break;
        }

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

        double cappedIntegral = kI * total_error;

        if (cappedIntegral > 50.0) {
            cappedIntegral = 50.0;
        } else if (cappedIntegral < -50.0) {
            cappedIntegral = -50.0;
        }

        kP *= boost;

        double motor_output = ((kP * error)  /*+ kD * (error-prev_error)/(current_time-prev_time)*/ + cappedIntegral);
        
        if (error > 0.0) {
            motors_instance->setLeftMotorDirection(true);//false
            Serial.println("left false and right true");
            motors_instance->setRightMotorDirection(true);//true
        } else {
            motors_instance->setLeftMotorDirection(false);//true
            Serial.println("left true and right false");
            motors_instance->setRightMotorDirection(false);//false
        }

        if (motor_output < 0.0) {
            motor_output *= -1;
        }

        if (motor_output > 255.0) {
            motor_output = 255.0;
        }

        int motor_output_l = (int)floor(motor_output);
        int motor_output_r = (int)floor(motor_output);
        

        if (abs(curA-prevA) > abs(curB-prevB)) {
            motor_output_l += adj_offset;
            motor_output_r += adj_offset;
        } else {
            motor_output_l -= adj_offset;
            motor_output_r -= adj_offset;
        }

        
        Serial.print("capped integral: ");
        Serial.print(cappedIntegral);
        Serial.println();

        Serial.print("motor output: ");
        Serial.print(motor_output);
        Serial.println();

        //return motor_output;

        motors_instance->setLeftMotorSpeed(motor_output_l);
        motors_instance->setRightMotorSpeed(motor_output_r);
    
    }

    exited = true;
    motors_instance->setMotorsSpeed(0.0);
    return;
}

PIDRotate* pidrotate_instance = new PIDRotate();