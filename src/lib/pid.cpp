#include "pid.h"

double p = 1.1;
double d = 0;
double i = 4;
void turnTo(char direction) {
    double targetDirection = 0;

    switch(direction) {
        case 'N':
            targetDirection = 0;
            break;
        case 'S':
            targetDirection = 180;
            break;
        case 'E':
            targetDirection = 90;
            break;
        case 'W':
            targetDirection = 270;
            break;
    }

    double error;

    error = targetDirection - angle();
    
    if(error < 0) error += 360;
    if (error > 180) error -= 360;

    // positive error means we are left of the desired angle
    // negative error means we are right of the desired angle

    double oldTime = 0;
    double newTime = micros();
    double oldError = error;
    double newError = error;

    double dt;
    double proportional;
    double derivative = 0;
    double integral = 0;
    double pid = 0;

    double sampleTime = micros();
    double sampleAngle = angle();
    while(abs(newError) > 1.5) {
        if(micros() > sampleTime + 1e6){
            if(angle() == sampleAngle) {
                return;
            }
            sampleTime = micros();
            sampleAngle = angle();
        }

        // Serial.println(error);
        // Serial.printf("angle %lf\n", angle());
        dt = (newTime - oldTime) / 1e6;
        Serial.println(dt);
        proportional = newError;
        
        derivative = (newError - oldError)/dt;
        
        // Serial.printf("newerror = %lf    olderr: %lf  der: %lf   \n",newError,oldError,derivative);
        // delay(100);
        if (abs(newError) < 20) {
            integral += 0.5*(newError + oldError)*dt;
            // integral += 1;
            Serial.print("integral: ");
            Serial.println(integral);

        }
            
        pid = p*proportional + d*derivative + i*integral;
        if(pid > 150){
        pid = 150;
        }
        else if(pid < -150){
            pid = -150;
        }
        Serial.printf("P: %lf   d: %lf   i: %lf   pid: %lf\n",proportional,derivative,integral,-pid);
        setLeftPWM(-pid);
        setRightPWM(-pid);
        // Serial.println(-pid);
        
        delayMicroseconds(1);
        
        oldError = newError;
        newError = targetDirection - angle();
    
        if(newError < 0) newError += 360;
        if (newError > 180) newError -= 360;

        oldTime = newTime;
        newTime = micros();
    }   

    setLeftPWM(0);
    setRightPWM(0);

}