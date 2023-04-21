#ifndef tof_h
#define tof_h
#include <Wire.h>
#include <VL53L1X.h>
#include <VL6180X.h>

#define V1

#ifndef V3
//scuffed bot
#define xshutW 23 // S1
#define xshutNW 22 // S2
#define xshutN 21 // S5
#define xshutNE 14  // S3
#define xshutE 15 // S4

#endif
#ifdef V3
//smexy bot
// #define xshutW 23 // S1
// #define xshutNW 20 // S2
// #define xshutN 19 // S5
// #define xshutNE 21 // S3
// #define xshutE 22 // S4

#endif

// short range sensors vl6180x on the diagonals

// The number of sensors in your system.
const uint8_t vl53l1xCount = 3;

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshutPins[vl53l1xCount] = { xshutW, xshutN, xshutE };
//vl53l1x is named xshut, while vl6180x is named gpio/CE

// The number of sensors in your system.
const uint8_t vl6180xCount = 2;

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t gpioPins[vl6180xCount] = { xshutNW, xshutNE };

VL53L1X vl53l1xtof[vl53l1xCount];

VL6180X vl6180xtof[vl6180xCount];

//this will be returned with sensor readings
struct sensorReadings {
  int left;
  int frontLeft;
  int front;
  int frontRight;
  int right;
};

//returns booleans whether the addresses were set correctly
struct addressCheck {
  bool left;
  bool frontLeft;
  bool front;
  bool frontRight;
  bool right;
};

class tof {
  
  //create each object from sensor library
  //doublecheck which type of sensor is where 
  // VL53L1X L; 
  // VL6180X FL;
  // VL53L1X F; 
  // VL6180X FR;
  // VL53L1X R;

  public:
    tof(); // constructor

    //put declarations here
    sensorReadings readDistance();
    addressCheck checkAddresses();
        
  private:
    //put declarations here

};



#endif
