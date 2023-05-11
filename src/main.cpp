#include <Arduino.h>
#include <SPI.h>
#include "lib\Motors.h"
#include "lib\PIDRotate.h"
#include "lib\PIDStraight.h"
#include "lib\PIDMagicStraight.h"
#include "lib\tof.h"
#include "lib\IMU.h"

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //timeofflight = new tof();
    //timeofflight->checkAddresses();  
    // Serial.println(timeofflight->readDistance().left);
    //Serial.println("hello world");
    //delay(5000);
    //Serial.print("left dist: ");
    motors_instance->enableMotors();
}

    void loop() {
    // put your main code here, to run repeatedly:
    //Serial.println("looping");
    /*Serial.print("encoder A positon: ");
    Serial.println(ENCA.read());

    Serial.print("encoder B positon: ");
    Serial.println(ENCB.read());*/
    //motors_instance->setMotorsSpeed((int)floor(pidrotate_instance->rotate_to_angle(180)));
    //motors_instance->setMotorsSpeed((int)floor(pidstraight_instance->drive_to_position(10)));
    //pidmagicstraight_instance->drive_straight_with_magic(30);

    //if (!pidrotate_instance->exited) {
    //  pidrotate_instance->rotate_to_angle(90);
    //}

    /*if (!pidmagicstraight_instance->exited) {
    Serial.println("exited = false");
    pidmagicstraight_instance->drive_straight_with_magic(10);
    }*/

    // if (tof_instance->readF() > 100) {
    //     pidmagicstraight_instance->drive_straight_with_magic(10);
    // } else if (tof_instance->readL() > 100) {
    //     pidrotate_instance->rotate_to_angle(-90);
    // } else if (tof_instance->readR() > 100) {
    //     pidrotate_instance->rotate_to_angle(90);
    // }


    while (true) {

        /*Serial.print("left dist: ");
        Serial.println(tof_instance->getLeftDistance());
        Serial.println(tof_instance->readL());
        Serial.print("front dist: ");
        Serial.println(tof_instance->getFrontDistance());
        Serial.println(tof_instance->readF());
        Serial.print("right dist: ");
        Serial.println(tof_instance->getRightDistance());
        Serial.println(tof_instance->readR());*/


       // pidrotate_instance->rotate_to_angle(90, 10, 5.0);

        if (tof_instance->readF() > 100) {
            pidstraight_instance->drive_to_position(8);
        } else if (tof_instance->readL() > 100) {
            pidrotate_instance->rotate_to_angle(imu_instance->getHeading()-90, 1.25, 5.0, 0.0001);
        } else if (tof_instance->readR() > 100) {
            pidrotate_instance->rotate_to_angle(imu_instance->getHeading()+90, 1.25, 5.0, 0.0001);
        }

        delay(500);
        pidrotate_instance->rotate_to_angle(round(imu_instance->getHeading()/90.0)*90.0, 1.25, 1.0, 0.01);
        delay(500);

    }




    /*pidmagicstraight_instance->drive_straight_with_magic(10);
    delay(500);
    pidrotate_instance->rotate_to_angle(-45);
    delay(500);
    pidrotate_instance->rotate_to_angle(45);
    delay(500);
    pidrotate_instance->rotate_to_angle(-45);
    delay(500);
    pidrotate_instance->rotate_to_angle(45);
    delay(500);
    pidrotate_instance->rotate_to_angle(0);
    delay(500);
    pidmagicstraight_instance->drive_straight_with_magic(-10);
    delay(5000);*/







}