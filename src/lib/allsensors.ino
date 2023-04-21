

#include <Wire.h>
#include <VL53L1X.h>
#include <VL6180X.h>

#define xshutW 23 // S1
#define xshutNW 22 // S2
#define xshutN 21 // S5
#define xshutNE 14  // S3
#define xshutE 15 // S4
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



void setup()
{


    digitalWrite(13, HIGH);
    delay(2000);

    digitalWrite(13, LOW);

  while (!Serial) {}
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

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
    vl6180xtof[i].setAddress(0x3D+i);

    vl6180xtof[i].startInterleavedContinuous(100);

  }

}


void loop()
{
  for (uint8_t i = 0; i < vl53l1xCount; i++)
  {
    Serial.print(i);
    Serial.print(" (vl53l1x): ");
    Serial.print(vl53l1xtof[i].read());
    if (vl53l1xtof[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    Serial.print('\t');
  }

for(uint8_t i = 0; i < vl6180xCount; i++) {
  // Serial.print("Ambient: ");
    // Serial.print(sensor.readAmbientContinuous());
    if (vl6180xtof[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  
    Serial.print(i);
    Serial.print(" (vl6180x): ");
    Serial.print(vl6180xtof[i].readRangeContinuousMillimeters());
    if (vl6180xtof[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    Serial.print('\t');


  }

  Serial.println();

  if(vl6180xtof[1].readRangeContinuousMillimeters() < 50) {
    digitalWrite(13, HIGH);
    delay(1000);
  } else {
    digitalWrite(13, LOW);
    delay(1000);
  }

}
