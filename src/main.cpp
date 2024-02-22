#include "lib/imu.h"
#include "lib/distance_sensor.h"
#include "lib/motors.h"

#include "Arduino.h"

void setup(void)
{
  Serial.begin(9600);
  imuSetup();
  motorSetup();
}

void loop(void) {
  delay(500);
  Serial.println();
  setRightPWM(75);
  setLeftPWM(75);

  delay(3000);
  setRightPWM(0);
  setLeftPWM(0);

  // Serial.println(getHeading());
}