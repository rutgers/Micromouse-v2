#ifndef motors_h
#define motors_h

#define BIN1 7
#define BIN2 8
#define PWMB 9
#define STBY 14
#define AIN1 6
#define AIN2 5
#define PWMA 4

void setLeftPWM(int PWM);
void setRightPWM(int PWM);
void motorSetup();

#endif