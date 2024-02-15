#include "lib/imu.h"
#include "lib/distance_sensor.h"

#include "Arduino.h"

void setup(void)
{
  Serial.begin(9600);
  imuSetup();
}

void loop(void) {
  delay(500);
  Serial.println(right());
  // Serial.println(getHeading());
}