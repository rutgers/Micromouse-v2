#include "lib\imu.h"

void setup(void)
{
  Serial.begin(9600);
  imuSetup();
}

void loop(void) {
  delay(500);
  Serial.println(getHeading());
}