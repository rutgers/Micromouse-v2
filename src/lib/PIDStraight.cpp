#include "PIDStraight.h"
#include<cstdlib>
PIDstraight::PIDstraight(){
    // imu = new IMU();
    motors_instance->setTick(0);
    // tof_instance = new tof();
    // pidrotate_instance = new PIDRotate();
    

    

}

PIDstraight::~PIDstraight(){
    // delete imu;
    // delete tof_instance;
    // delete pidrotate_instance;
   
}
/**
 * Allows the bot to go straight weeeee
 * 4.82370523622 inches = 360 is circumference
*/

void PIDstraight::drive_to_position(double distance){
    distance *= 82;
    motors_instance->enableMotors();
    motors_instance->setMotorsDirection(true);
    motors_instance->setTick(0);
    delay(100);
    motors_instance->setMotorsSpeed(50); 
            
        while(((double)(((abs(motors_instance->getlefttick())) + (abs(motors_instance->getrighttick())))/2) < distance)){
            Proportion = ( abs(motors_instance->getlefttick())) - (abs(motors_instance->getrighttick()));
            Integral += Proportion;
            Derivative = Proportion - lasterror;
            int total = (Kp * Proportion) + Ki * Integral + Kd * Derivative;
            leftpower -= total;
            rightpower += total;
            
            leftpower = constrain(leftpower,0,100);
            rightpower = constrain(rightpower,0,100);
            motors_instance->setLeftMotorSpeed(leftpower);
            motors_instance->setRightMotorSpeed(rightpower);
            lasterror = Proportion; 
            // delay(20);

        }
    
        motors_instance->setMotorsSpeed(0);
        delay(10);
        if(tof_instance->readF() < 175){
            gointowall();
        }

}
void PIDstraight::gointowall(){
    motors_instance->setMotorsDirection(true);
    while(tof_instance->readF() > 30 && tof_instance->readF() < 175){
          motors_instance->setMotorsSpeed(40);
          delay(40);
          motors_instance->setMotorsSpeed(0);
          delay(90);

    }
    motors_instance->setMotorsDirection(false);
    while(tof_instance->readF() < 25){
        motors_instance->setMotorsSpeed(40);
          delay(40);
          motors_instance->setMotorsSpeed(0);
          delay(90);
    }
   
}
double PIDstraight::map(double error){
    if (error > 180.0) {
        return -360.0 + error;
    } else if (error < -180.0) {
        return 360.0 + error;
    }
    return error;
}