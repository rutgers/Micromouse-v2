#include <Arduino.h>
#include "tof.h"

//user should be able to declare a single sensor object, and access all five sensors from that one object


//constructor
tof::tof() {
    //led test code
    // digitalWrite(13, HIGH);
    // delay(2000);

    // digitalWrite(13, LOW);

  while (!Serial) {}
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

/*

  // Disable/reset all vl53l1xtof by driving their XSHUT pins low.
  for (uint8_t i = 0; i < vl53l1xCount; i++)
  {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
  }

   for (uint8_t i = 0; i < vl6180xCount; i++)
  {
    pinMode(gpioPins[i], OUTPUT);
    digitalWrite(gpioPins[i], LOW);
  }

  // Enable, initialize, and start each sensor, one by one.
  for (uint8_t i = 0; i < vl53l1xCount; i++)
  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshutPins[i], INPUT);
    delay(10);

    vl53l1xtof[i].setTimeout(1000);
    if (!vl53l1xtof[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      // while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    vl53l1xtof[i].setAddress(0x2A + i);

    vl53l1xtof[i].startContinuous(50);
  }
*/

  for (uint8_t i = 0; i < vl6180xCount; i++)
  {
    pinMode(gpioPins[i], OUTPUT);
    digitalWrite(gpioPins[i], LOW);
  }

  for(uint8_t i = 0; i < vl6180xCount; i++) {
  

  pinMode(gpioPins[i], INPUT);
  delay(10);

  //VL6180X specific code
    vl6180xtof[i].init();
    vl6180xtof[i].configureDefault();

    vl6180xtof[i].writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    vl6180xtof[i].writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);

    vl6180xtof[i].setTimeout(500);

    vl6180xtof[i].stopContinuous();
    delay(300);
    vl6180xtof[i].setAddress(0x2A+i);

    vl6180xtof[i].startInterleavedContinuous(100);
    
    
  }
}

sensorReadings tof::readDistance() {
  
  sensorReadings returnStruct;

  returnStruct.left = vl6180xtof[0].readRangeContinuousMillimeters();
  returnStruct.frontLeft = vl6180xtof[1].readRangeContinuousMillimeters();
  returnStruct.front = vl6180xtof[2].readRangeContinuousMillimeters();
  returnStruct.frontRight = vl6180xtof[3].readRangeContinuousMillimeters();
  returnStruct.right = vl6180xtof[4].readRangeContinuousMillimeters();

if (vl6180xtof[0].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl6180xtof[1].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl6180xtof[2].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl6180xtof[3].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
if (vl6180xtof[4].timeoutOccurred()) { Serial.print(" TIMEOUT"); }

Serial.println();
Serial.print("Left: ");
Serial.println(returnStruct.left);
Serial.print("FrontLeft: ");
Serial.println(returnStruct.frontLeft);
Serial.print("Front: ");
Serial.println(returnStruct.front);
Serial.print("FrontRight: ");
Serial.println(returnStruct.frontRight);
Serial.print("Right: ");
Serial.println(returnStruct.right);


  return returnStruct;
}

int tof::readL() {
  return vl6180xtof[0].readRangeContinuousMillimeters();
  if (vl6180xtof[0].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
}


int tof::readFL() {
  return vl6180xtof[1].readRangeContinuousMillimeters();
  if (vl6180xtof[1].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
}

int tof::readF() {
  return vl6180xtof[2].readRangeContinuousMillimeters();
  if (vl6180xtof[2].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
}

int tof::readFR() {
  return vl6180xtof[3].readRangeContinuousMillimeters();
  if (vl6180xtof[3].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
}

int tof::readR() {
  return vl6180xtof[4].readRangeContinuousMillimeters();
  if (vl6180xtof[4].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
}



addressCheck tof::checkAddresses() {
  Serial.println(vl6180xtof[0].getAddress());
  Serial.println(vl6180xtof[1].getAddress());
  Serial.println(vl6180xtof[2].getAddress());
  Serial.println(vl6180xtof[3].getAddress());
  Serial.println(vl6180xtof[4].getAddress());

  addressCheck returnAddresses;

  returnAddresses.left = false;
  returnAddresses.frontLeft = false;
  returnAddresses.front = false;
  returnAddresses.frontRight = false;
  returnAddresses.right = false;

  if(vl6180xtof[0].getAddress() == 0x2A) returnAddresses.left = true;
  if(vl6180xtof[1].getAddress() == 0x2B) returnAddresses.frontLeft = true;
// @@@@@@@@@@@@@@@@@@@@@ &&%%%%%%&&...&&   .,#%%%%%%%%%%%@     .....%@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@&&@&(,**..,     /....  ....(%%%%%%%%%%, ........@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@&,,,,(.../       %   . , ..... &%%%%%%%%# .......*@@@@@@@@@@@@
// @@@@@@@@@@@@@@@&&,....*  ./        (,%      ,...... %#%%&&&&  .......@@@@@@@@@@@
// @@@@@@@@@@@@@@%.. ...%              ,,,(      *   .   &%&&&&&,   ....,%@@@@@@@@@
// @@@@@@@@@@@@@.           .            ...       .       %&&&&&/    ...,&@@@@@@@@
// @@@@@@@@@@@@                            .        .        @&&@@*    ...,&@@@@@@@
// @@@@@@@@@@@                                               .%@@@%*......,,@@@@@@@
// @@@@@@@@@@         ,    %   .                      ...    ..*@@@.#.....,,@@@@@@@
// @@@@@@@@@&   .     (  .(&.                      . ...........#@@,*,..,,,,*@@@@@@
// @@@@@@@@@          & ..&&&          .  ,        ..............@@,**...,,,,@@@@@@
// @@@@@@@@&   ,      % .&%%%&           .  .       ............,.@,,*#...,,,&@@@@@
// @@@@@@@@#          #(*&%%%%&. .           /.   , ....,.........&/,*,.,,,,**@@@@@
// @@@@@@@@(  .   .    &&&&%%%%%,.&            ,.,  ,...,........../,,,,.,.,*/@@@@@
// @@@@@@@@# *.% ... . (&&&&&%%%%&,*,      .   , ..,.#.......,..,..(.,,/.*,&*#&@@@@
// @@@@@@@@@. ..%*..    %%&&%%%%%%%%&&(        ... ..*..,....,,,.*./.,,(,*(@*@@@@@@
// @@@@@@@@@.@. %*(      %&&%%%#(((###&&&   .  .*/.., (.,,..,,,,,#./,,,*,*@@/@@@@@@
// @@@@@@@@@/@&# ./(,  *&% %&&&&&.      ..//..#,,..**(#*(*.,,,,*,(./,,,/,&@@@@@@@@@
// @@@@@@@@@@@@@  /,..  ,,%%&                  /.(/**,#...,,,***((**,,/,*(@@@@@@@@@
// @@@@@@@@@@@@@@*(@ .,.%#,         .             **(.*..,,***#((/**/,#*@@@@@@@@@@@
// @@@@@@@@@@@@@@@/(@&,..,**(                        %....***/((/**/@&*@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@,,,*****      ....          //%...(%//(%/&(@@@&@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@&@@@@%*(&,*&(/             ////*/.&@#%@&%@%@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@&@@@@@@,     &(((.   /*@@@%@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@&//*&&@@@@@@@@@@@@@@@@@@%@*              @&&@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@*,##%%%%%*//(&@@@@@@@@@@@@@@%%&%%%%%%%%%%%%%%%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@*,,#######%%%%(/((##&@@@@@@@%////&&&&&&&&&&&&&&%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// /*,/#########%%%%%###%%#///*(%%%%%&#(%&&&&&&&&%%@&&&%%@@@@@@@@@@@@@@@@*#%%&@@@@@
// **(/####%%%%%%%%%%%%%%%&&#%%%%#%&#&&%%#&&%%%%&&&&&%%@%%&//&@@/,,,,,,,(#%###%&@@@
// /(*#%%%%%%%%%%%%%%%%%%%%&#&&%###&&%%&&%%%%%%&&&%%%%&%%%%%#(((#**,,,*######%%%%@@
// /*###########%&&&&&&&&&&&%&#&&&%&&&%%%&&&&&%%%%%%%%%%%%%%%%####(/&###%%%%%%%%%&&
// */##%%%%%%%%##%%%%%%%%%@&&&&&##&&&&&%%%%&&&&&%%&%%%%&%%%%%%%%%##&//##%%%%%%%%&&&

  if(vl6180xtof[2].getAddress() == 0x2C) returnAddresses.front = true;
  if(vl6180xtof[3].getAddress() == 0x2D) returnAddresses.frontRight = true;
  if(vl6180xtof[4].getAddress() == 0x2E) returnAddresses.right = true;

  return returnAddresses;
}
tof* timeofflight_instance = new tof();
