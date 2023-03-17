#ifndef tof_h
#define tof_h

#include <Arduino.h>
#include <VL53L1X.h>
#include <VL6180X.h>

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

//pins lined 23-19 left to right on teensy
#define xshutW 23 // S1
#define xshutNW 20 // S2
#define xshutN 19 // S5
#define xshutNE 21 // S3
#define xshutE 22 // S4

class tof {
  
  //create each object from sensor library
  //doublecheck which type of sensor is where 
  VL53L1X L; 
  VL6180X FL;
  VL53L1X F; 
  VL6180X FR;
  VL53L1X R;

  public:
    tof(); // constructor

    //put declarations here
    sensorReadings readDistance();
    addressCheck checkAddresses();
        
  private:
    //put declarations here

};



#endif
