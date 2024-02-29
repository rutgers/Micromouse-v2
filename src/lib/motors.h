#ifndef motors_h
#define motors_h


#include<Encoder.h>

#define BIN1 7
#define BIN2 8
#define PWMB 9
#define STBY 14
#define AIN1 6
#define AIN2 5
#define PWMA 4

extern Encoder encLeft;
extern Encoder encRight;


void setLeftPWM(int PWM);
void setRightPWM(int PWM);
void motorSetup();

#endif