#include "lib/imu.h"
#include "lib/distance_sensor.h"
#include "lib/motors.h"

#include "Arduino.h"

// void setup(void)
// {
//   Serial.begin(9600);
//   imuSetup();
//   motorSetup();
// }

// void loop(void) {
//   delay(500);
//   Serial.println(encLeft.read());
//   Serial.println(encRight.read());



//   // setRightPWM(75);
//   // setLeftPWM(75);

//   // delay(3000);
//   // setRightPWM(0);
//   // setLeftPWM(0);

//   // Serial.println(getHeading());
// }

/* This example demonstrates how to use interleaved mode to
take continuous range and ambient light measurements. The
datasheet recommends using interleaved mode instead of
running "range and ALS continuous modes simultaneously (i.e.
asynchronously)".

In order to attain a faster update rate (10 Hz), the max
convergence time for ranging and integration time for
ambient light measurement are reduced from the normally
recommended defaults. See the section "Continuous mode
limits" and the table "Interleaved mode limits (10 Hz
operation)" in the VL6180X datasheet for more details.

Raw ambient light readings can be converted to units of lux
using the equation in datasheet section "ALS count
to lux conversion".

Example: A VL6180X gives an ambient light reading of 613
with the default gain of 1 and an integration period of
50 ms as configured in this sketch (reduced from 100 ms as
set by configureDefault()). With the factory calibrated
resolution of 0.32 lux/count, the light level is therefore
(0.32 * 613 * 100) / (1 * 50) or 392 lux.

The range readings are in units of mm. */
#include <Wire.h>
#include <VL53L1X.h>
#include "./lib/imu.h"

VL53L1X sensor;

void setup()
{
  imuSetup();
  sensorInit();
}

void loop()
{
  // Serial.print(sensor.read());
  // if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  // Serial.println();
  delay(500);
  // Serial.println(front());
  // Serial.println(left());
  // Serial.println(right());
  // Serial.println(angle());


}