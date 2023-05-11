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

    int adj_offset = 0;

    bool exited = false;

    int curA = 0;
    int curB = 0;
    int prevA = 0;
    int prevB = 0;

    int ABdiff = 0;
        
    ENCA.write(0);
    ENCB.write(0);

    int exitCount = 0;

    while (true) {

        prevA = curA;
        prevB = curB;
        curA = ENCA.read();
        curB = ENCA.read();
        ABdiff = (curA-prevA) - (curB-prevB);

        adj_offset = ABdiff * 1;


        //current_position = imu_instance->getHeading();
        current_position = ENCA.read();//(ENCA.read() + ENCB.read())/2;

        // handle cases of numbers above 360

        // for example, the number -718 will become 718, will become -358
        

        prev_error = error;
        //error = (target_position * 58.2124805) - current_position; //72.74463
        error = (target_position * 72.74463) - current_position;

        if (abs(error) < 10.0) {
            exitCount++;
        }

         if (exitCount > 1000) {
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
        double motor_output_l = (kP * error + kI*total_error)*10;
        double motor_output_r = (kP * error + kI*total_error)*10;
        
        if (error > 0.0) {
            motors_instance->setLeftMotorDirection(true);//false
            Serial.println("left false and right true");
            motors_instance->setRightMotorDirection(false);//true
        } else {
            motors_instance->setLeftMotorDirection(false);//true
            Serial.println("left true and right false");
            motors_instance->setRightMotorDirection(true);//false
            motor_output_l *= -1;
            motor_output_r *= -1;
        }

        if ((curA-prevA) > (curB-prevB)) {
            motor_output_l += adj_offset;
            motor_output_r -= adj_offset;
        } else {
            motor_output_l -= adj_offset;
            motor_output_r += adj_offset;
        }

        if (motor_output_l > 255.0) {
            motor_output_l = 255.0;
        }

        if (motor_output_r > 255.0) {
            motor_output_r = 255.0;
        }

        Serial.print("motor output_l: ");
        Serial.print(motor_output_l);
        Serial.println();

        Serial.print("motor output_r: ");
        Serial.print(motor_output_r);
        Serial.println();

        Serial.print("ENCA: ");
        Serial.print(ENCA.read());
        Serial.println();

        Serial.print("ENCB: ");
        Serial.print(ENCB.read());
        Serial.println();

        motors_instance->setLeftMotorSpeed(motor_output_l);
        motors_instance->setRightMotorSpeed(motor_output_r);
    }

    exited = true;
    motors_instance->setMotorsSpeed(0.0);
    Serial.print("exited: ");
    Serial.println(exited);
    
    
}

PIDStraight* pidstraight_instance = new PIDStraight();