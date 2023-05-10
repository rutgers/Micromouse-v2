#include"tofAF.h"
tofAF::tofAF(){
   while (!Serial) {}
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  resetpins();
  SetupVL531lxsenors(); //setup the VL531 sensors
  SetupVL6180xsensors(); //setup  6180 sensors. 
}
int tofAF::getreadings(int d){
   d++;
 
    switch(d){
      case 1:
        return VL53L1Xsensors[0].readRangeContinuousMillimeters();
        break;
      case 3:
       return VL53L1Xsensors[1].readRangeContinuousMillimeters();
       break;
      case 5:
       return VL53L1Xsensors[2].readRangeContinuousMillimeters();
       break;
      case 2:
       return vl6180x[0].readRangeContinuousMillimeters();
       break;
      case 4:
        return vl6180x[1].readRangeContinuousMillimeters();
        break;
    }
    return 0;
  }
  

void tofAF::SetupVL6180xsensors(){
  for(int i = 0; i < 2; i++){
  pinMode(vl6180xpins[i],INPUT);  
  vl6180x[i].init();
  vl6180x[i].configureDefault();
  vl6180x[i].writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  vl6180x[i].writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  vl6180x[i].setTimeout(500);
  vl6180x[i].stopContinuous();
  delay(300);
  vl6180x[i].setAddress(VL6180xaddress[i]);
  vl6180x[i].startInterleavedContinuous(100);
  }
}
void tofAF::SetupVL531lxsenors(){
   for (uint8_t i = 0; i < 3; i++)
  {
    pinMode(vl53l1xpins[i], INPUT);
    delay(10);

    vl6180x[i].setTimeout(500);
    if (!VL53L1Xsensors[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    VL53L1Xsensors[i].setAddress(VL53l1xxaddress[i]);

    VL53L1Xsensors[i].startContinuous(50);
  }
}
void tofAF::resetpins(){
  for(int i = 0; i < 3; i++){ //sets 531 pins to low for reset
    pinMode(vl53l1xpins[i],OUTPUT);
    digitalWrite(vl53l1xpins[i],LOW);
  }
  for(int i = 0; i < 2; i++){ //sets vl6180x pins to low. 
    pinMode(vl6180xpins[i],OUTPUT);
    digitalWrite(vl6180xpins[i],LOW);
  }
}