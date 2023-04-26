#include"tofAF.h"
tofAF::tofAF(){
  xshut[0] = 0;
  xshut[1] = 1;
  xshut[2] = 2;
  xshut[3] = 3;
  xshut[4] = 4;
  address[0] = 0xB8;
  address[1] = 0x1E;
  address[2] = 0x72;
  address[3] = 0xCF;
  address[4] = 0xA3;
  for(uint8_t i = 0; i < 5; i++){
    pinMode(xshut[i], OUTPUT);
    digitalWrite(xshut[i],LOW);
  } 
  for(uint8_t j = 0; j < 3; j++){
    pinMode(xshut[j],INPUT);
    delay(10);
    vl53l1x[j].setTimeout(500);
    if (!vl53l1x[j].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(j);
      while (1);
    }
    vl53l1x[j].setAddress(address[j]);

    vl53l1x[j].startContinuous(50);
  }
  for(uint8_t j = 0; j < 2; j++){
    vl6180x[j].init();
    vl6180x[j].configureDefault();

    vl6180x[j].writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    vl6180x[j].writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);

    vl6180x[j].setTimeout(500);

    vl6180x[j].stopContinuous();
    delay(300);
    vl6180x[j].setAddress(address[j+3]);

    vl6180x[j].startInterleavedContinuous(100);
  }
}
readings tofAF::getreadings(){
  readings temp;
  temp.west = vl53l1x[0].read();
  temp.northwest = vl6180x[0].readRangeContinuous();
  temp.north = vl53l1x[1].read();
  temp.northeast = vl6180x[1].readRangeContinuous();
  temp.east = vl53l1x[2].read();
  return temp;

}
connected tofAF::isconnected(){
  connected temp;
  temp.west =vl53l1x[0].getAddress() == address[0];
  temp.northwest=vl6180x[0].getAddress() == address[1];
  temp.north =vl53l1x[1].getAddress() == address[2];
  temp.northeast=vl6180x[1].getAddress() == address[3];
  temp.east=vl53l1x[2].getAddress() == address[4];
  return temp;
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