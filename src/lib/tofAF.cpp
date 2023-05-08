#include"tofAF.h"
tofAF::tofAF(){
  address[0] = 0x20;
  address[1] = 0x22;
  address[2] = 0x24;
  address[3] = 0x25;
  address[4] = 0xFF;
  xshut[0] = 0;
  xshut[1] = 1;
  xshut[2] = 2;
  xshut[3] = 3;
  xshut[4] = 4;
  Serial.begin(9600);
  Wire.begin();

  for (int i = 0; i < 5; i++) {
    pinMode(xshut[i], OUTPUT);
    digitalWrite(xshut[i], LOW);
  }
  
  delay(1000);
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(xshut[i], HIGH);
    delay(50);
    vl6180x[i].init();
    vl6180x[i].configureDefault();
    vl6180x[i].setAddress(address[i]);
    vl6180x[i].writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    vl6180x[i].writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
    vl6180x[i].setTimeout(500);
    vl6180x[i].stopContinuous();
    vl6180x[i].setScaling(2);
    delay(300);
    vl6180x[i].startInterleavedContinuous(100);
    delay(100);
  }

  delay(4000);
}
int tofAF::getreadings(int d){
return vl6180x[d].readRangeContinuousMillimeters();
}
/*
sensorReadings tof::readDistance() {
  
  sensorReadings returnStruct;

  returnStruct.left = vl53l1xtof[0].read();
  returnStruct.frontLeft = vl6180xtof[0].readRangeContinuousMillimeters();
  returnStruct.front = vl53l1xtof[1].read();
  returnStruct.frontRight = vl6180xtof[1].readRangeContinuousMillimeters();
  returnStruct.right = vl53l1xtof[2].read();

if (vl53l1xtof[0].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl53l1xtof[1].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl53l1xtof[2].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl6180xtof[0].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl6180xtof[1].timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  return returnStruct;
}
*/