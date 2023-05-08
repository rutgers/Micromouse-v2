#include <math.h>
#include <Arduino.h>
#include "PIDMagicStraight.h"
#include "Motors.h"
#include "IMU.h"

void PIDMagicStraight::drive_straight_with_magic(double target_position) {
        double current_angle = 0;
        int current_time = 0;
        int prev_time = 0;
        double error_l = 0.0;
        double error_r = 0.0;
        double prev_error_l = 0.0;
        double prev_error_r = 0.0;
        double total_error_l;
        double total_error_r;

        
        double current_position = 0;
        double error_pos = 26.0;
        double prev_error_pos = 0.0;
        double total_error_pos;

        exited = false;
        
        ENCA.write(0);
        ENCB.write(0);

        int count = 0;

        int start_angle = imu_instance->getHeading();

    while (true) {
        current_angle = imu_instance->getHeading();
        current_position = (ENCA.read() + ENCB.read())/2;

        count++;

        // handle cases of numbers above 360

        // for example, the number -718 will become 718, will become -358

        double target_angle_l = -90+start_angle;
        double target_angle_r = 90+start_angle;
        

        prev_error_l = error_l;
        error_l = target_angle_l - current_angle;

        prev_error_r = error_r;
        error_r = target_angle_r - current_angle;


        prev_error_pos = error_pos;
        error_pos = (target_position * 72.74463) - current_position;

        if (abs(error_pos) < 10.0 ) {
            Serial.println("breaking");
            break;
        }

        total_error_pos += abs(error_pos);


        error_l = fmod(error_l, 360.0); // keep between 0 and 360 or 0 and -360
        error_r = fmod(error_r, 360.0); // keep between 0 and 360 or 0 and -360

        if (error_l > 180.0) {
            error_l = -360.0 + error_l;
        } else if (error_l < -180.0) {
            error_l = 360.0 + error_l;
        }

        if (error_r > 180.0) {
            error_r = -360.0 + error_r;
        } else if (error_r < -180.0) {
            error_r = 360.0 + error_r;
        }

        total_error_l += abs(error_l);
        total_error_r += abs(error_r);
        prev_time = current_time;
        current_time = millis();

        //int target_angle = current_angle + angle;

        //Serial.print("current angle: ");
        //Serial.print(current_angle);
        //Serial.println();

        /*Serial.print("target angle: ");
        Serial.print(target_angle);
        Serial.println();*/


        /*if (error > 180.0) {
            error -= 180.0;
        }

        if (error < -180.0) {
            error += 180.0;
        }*/

        /*Serial.print("error_l: ");
        Serial.print(error_l);
        Serial.print("error_r: ");
        Serial.print(error_r);
        Serial.println();*/

        double motor_output_l = (kP * error_l);//  + kD * (error_l-prev_error_l)/(current_time-prev_time) + kI * total_error_l);
        double motor_output_r = (kP * error_r);//  + kD * (error_r-prev_error_r)/(current_time-prev_time) + kI * total_error_r);

        double motor_output_pos = (kP * error_pos)*25;

        //figure out avg of l and r. Figure out distance from average of l and r; scale this distance by the ratio between the motor outputs, add and subtract to motor_output_pos respectively
        

        motor_output_l *= 40;
        motor_output_r *= 40;


        double motor_output_rotate_avg = (abs(motor_output_l)+abs(motor_output_r))/2;

        double l_diff = motor_output_rotate_avg - abs(motor_output_l);
        double r_diff = motor_output_rotate_avg - abs(motor_output_r);

        Serial.print("\nmotor_output_pos: ");
        Serial.print(motor_output_pos);
        Serial.print("\nmotor_output_l: ");
        Serial.print(motor_output_l);
        Serial.print("\nmotor_output_r: ");
        Serial.print(motor_output_r);
        Serial.print("\nmotor_output_rotate_avg: ");
        Serial.print(motor_output_rotate_avg);
        Serial.print("\nl_diff: ");
        Serial.print(l_diff);
        Serial.print("\nr_diff: ");
        Serial.print(r_diff);
        Serial.print("\nerror_pos: ");
        Serial.print(error_pos);
        Serial.print("\nENCA: ");
        Serial.print(ENCA.read());
        Serial.print("\nENCB: ");
        Serial.print(ENCB.read());
        Serial.print("\ncount: ");
        Serial.print(count);
        Serial.println();

        /*if (error_l > 0.0) { // l or r?
            motors_instance->setLeftMotorDirection(false);//false
        } else {
            motors_instance->setLeftMotorDirection(true);//true
            motor_output_l *= -1;
        }

        if (error_r > 0.0) { // l or r?
            motors_instance->setRightMotorDirection(true);//true
        } else {
            motors_instance->setRightMotorDirection(false);//false
            motor_output_r *= -1;
        }*/

        if (error_pos > 0.0) {
            motors_instance->setLeftMotorDirection(true);//false
            Serial.println("left false and right true");
            motors_instance->setRightMotorDirection(true);//true
        } else {
            motors_instance->setLeftMotorDirection(false);//true
            Serial.println("left true and right false");
            motors_instance->setRightMotorDirection(false);//false
            motor_output_pos *= -1;
        }


        if (motor_output_l > 255.0) {
            motor_output_l = 255.0;
        }
        if (motor_output_r > 255.0) {
            motor_output_r = 255.0;
        }

        /*Serial.print("motor output_l: ");
        Serial.print(motor_output_l);
        Serial.print("motor output_r: ");
        Serial.print(motor_output_r);
        Serial.println();*/

        motors_instance->setLeftMotorSpeed((int)floor(motor_output_pos-l_diff));
        motors_instance->setRightMotorSpeed((int)floor(motor_output_pos-r_diff));

    }

    exited = true;
    motors_instance->setMotorsSpeed(0.0);
    Serial.print("exited: ");
    Serial.println(exited);
    
}

PIDMagicStraight* pidmagicstraight_instance = new PIDMagicStraight();