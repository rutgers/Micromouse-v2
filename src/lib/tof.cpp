#include"tof.h"

tof::tof(){
  
  //xshut[0] = 23; // left
  //xshut[1] = 22; // front
  //xshut[2] = 21; // right
  //xshut[3] = 14; // frontleft
  //xshut[4] = 15; // frontright

  xshut[0] = 15; // left
  xshut[1] = 20; // front
  xshut[2] = 23; // right
  xshut[3] = 21; // frontleft
  xshut[4] = 14; // frontright

  address[0] = 0xB8; // left
  address[1] = 0x1E; // front
  address[2] = 0x72; // right
  address[3] = 0xCF; // frontleft
  address[4] = 0xA3; // frontright

  for(uint8_t i = 0; i < 5; i++){
    pinMode(xshut[i], OUTPUT);
    digitalWrite(xshut[i],LOW);
  } 

  for(uint8_t j = 0; j < 3; j++){
    pinMode(xshut[j],INPUT);
    delay(10);
    vl53l1x[j].setTimeout(500);

    //Serial.print("error code: ");
    //Serial.println(vl53l1x[j].init());

    if (!(vl53l1x[j].init())) {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(j);
      //while (1);
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

int tof::getLeftDistance() {
  return vl53l1x[0].read(false);
}
int tof::getFrontDistance() {
  return vl53l1x[1].read(false);
}
int tof::getRightDistance() {
  return vl53l1x[2].read(false);
}
int tof::getFrontLeftDistance() {
  return vl6180x[0].readRangeContinuous();
}
int tof::getFrontRightDistance() {
  return vl6180x[1].readRangeContinuous();
}

void tof::isConnected(){
  isworking.left =vl53l1x[0].getAddress() == address[0];
  isworking.frontleft=vl6180x[0].getAddress() == address[1];
  isworking.front =vl53l1x[1].getAddress() == address[2];
  isworking.frontright=vl6180x[1].getAddress() == address[3];
  isworking.right=vl53l1x[2].getAddress() == address[4];
}

tof* tof_instance = new tof();

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