#include <Arduino.h>
#include "tof.h"

//user should be able to declare a single sensor object, and access all five sensors from that one object


//constructor
tof::tof() {
  //sensor positions are as follows
  VL53L1X L; 
  VL6180X FL;
  VL53L1X F; 
  VL6180X FR;
  VL53L1X R;

  //basically what would be in the setup function run when the object is created, presumably
  
  //TODO?
  //we need to give arguments to the following functions?
  //probably not since the constructor already has all the objects it needs?
  setAddresses();
  setup();
}

struct tof::readDistance() {
  
  //TODO
  // vl53l1x
  //vl53l1x.read();

  // vl6180x
  //vl6180.readRangeContinuousMillimeters();

  sensorReadings.left = L.read();
  sensorReadings.frontLeft = FL.readRangeContinuousMillimeters();
  sensorReadings.front = F.read();
  sensorReadings.frontRight = FR.readRangeContinuousMillimeters();
  sensorReadings.right = R.read();

  return sensorReadings;
}


void tof::setAddresses()
{
    //standby all
    digitalWrite(xshutW, LOW);
    digitalWrite(xshutNW, LOW);
    digitalWrite(xshutN, LOW);
    digitalWrite(xshutNE, LOW);
    digitalWrite(xshutE, LOW);

        //un-standby
    digitalWrite(xshutW, HIGH);
    delay(50);
    L.init();
    L.configureDefault();
    L.setAddress(0x2A);
    digitalWrite(xshutW, LOW);

    //un-standby second tof
    digitalWrite(xshutNW, HIGH);
    delay(50);
    FL.init();
    FL.configureDefault();
    FL.setAddress(0x2B);
    digitalWrite(xshutNW, LOW); // re-standby

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

    digitalWrite(xshutN, HIGH);
    delay(50);
    F.init();
    F.configureDefault();
    F.setAddress(0x2C); 
    digitalWrite(xshutN, LOW);

    digitalWrite(xshutNE, HIGH);
    delay(50);
    FR.init();
    FR.configureDefault();
    FR.setAddress(0x2D);
    digitalWrite(xshutNE, LOW);

    digitalWrite(xshutE, HIGH);
    delay(50);
    R.init();
    R.configureDefault();
    R.setAddress(0x2E);
    digitalWrite(xshutE, LOW);

    //re-unstandby all sensors
    digitalWrite(xshutW, HIGH);
    digitalWrite(xshutNW, HIGH);
    digitalWrite(xshutN, HIGH);
    digitalWrite(xshutNE, HIGH);
    digitalWrite(xshutE, HIGH);
}


void tof::setup() {
  // put your setup code here, to run once:

  // TODO
  //repeat this for each sensor object  
  //the fl and fr vl6180x sensors
  FL.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 20);
  FL.startRangeContinuous(50);

  FR.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 20);
  FR.startRangeContinuous(50);
  
  //the front, left, and right vl531x sensors
  F.setDistanceMode(VL53L1X::Long);
  F.setMeasurementTimingBudget(33000);
  F.startContinuous(33);

  L.setDistanceMode(VL53L1X::Long);
  L.setMeasurementTimingBudget(33000);
  L.startContinuous(33);

  R.setDistanceMode(VL53L1X::Long);
  R.setMeasurementTimingBudget(33000);
  R.startContinuous(33);
}


// TODO, led response for addressing
//     strip.setPixelColor(numberPixel, red, green, blue);
//or //strip.setPixelColor(numberPixel, red, green, blue, white);

struct {
  bool left;
  bool frontLeft;
  bool front;
  bool frontRight;
  bool right;
} addressCheck;

struct tof::checkAddresses() {
  Serial.println(L.getAddress());
  Serial.println(FL.getAddress());
  Serial.println(F.getAddress());
  Serial.println(FR.getAddress());
  Serial.println(R.getAddress());

  addressCheck.left = false;
  addressCheck.frontLeft = false;
  addressCheck.front = false;
  addressCheck.frontRight = false;
  addressCheck.right = false;

  if(L.getAddress == 0x2A) addressCheck.left = true;
  if(FL.getAddress == 0x2B) addressCheck.frontLeft = true;
  if(F.getAddress == 0x2C) addressCheck.front = true;
  if(FR.getAddress == 0x2D) addressCheck.frontRight = true;
  if(R.getAddress == 0x2E) addressCheck.right = true;

  return addressCheck;
}
