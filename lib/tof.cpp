#include <Arduino.h>
#include "tof.h"

//user should be able to declare a single sensor object, and access all five sensors from that one object


//constructor
tof::tof() {
  //doublecheck which type of sensor is where 
  VL53L1X L; 
  VL6180X FL;
  VL53L1X F; 
  VL6180X FR;
  VL53L1X R;

  //basically what would be in the setup function run when the object is created
  setAddresses();
  init();
}

struct tof::readDistance() {
  
  //TODO
  // vl53l1x
  vl53l1x.read();

  // vl6180x
  vl53l1x.readRangeContinuous();

  sensorReadings.left = L.
  sensorReadings.frontLeft = 
  sensorReadings.front = 
  sensorReadings.frontRight = 
  sensorReadings.right = 

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


void tof::init() {
  // put your setup code here, to run once:

  // TODO
  //repeat this for each sensor object  
  vl6180x.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 20);
  vl6180x.startRangeContinuous(50);

  vl531x.setDistanceMode(VL53L1X::Long);
  vl531x.setMeasurementTimingBudget(33000);
  vl531x.startContinuous(33);



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
