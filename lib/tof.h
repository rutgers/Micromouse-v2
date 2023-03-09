#ifndef tof_h
#define tof_h

#include <Arduino.h>
#include <VL53L1X.h>
#include <VL6180X.h>
#include <Adafruit_NeoPixel.h>


//this will be returned with sensor readings
struct {
  uint16_t left;
  uint16_t frontLeft;
  uint16_t front;
  uint16_t frontRight;
  uint16_t right;
} sensorReadings;

//pins lined 23-19 left to right on teensy
#define xshutW 23 // S1
#define xshutNW 20 // S2
#define xshutN 19 // S5
#define xshutNE 21 // S3
#define xshutE 22 // S4

// TODO: neopixel???
//probably will be a separate library for this that we'll have to pass values to?
#define LED_PIN 69
#define LED_COUNT 69


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
      struct readDistance();
      struct checkAddresses();
        
    private:
      //put declarations here

};

#endif
