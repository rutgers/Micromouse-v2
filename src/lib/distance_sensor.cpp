#include "distance_sensor.h"
const uint8_t sensorCount = 1;


// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshutPins[sensorCount] = { 22 };

VL53L1X sensors[sensorCount];


void sensorInit() {

    // The number of sensors in your system.
    
    // while (!Serial) {}
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    // Disable/reset all sensors by driving their XSHUT pins low.
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        pinMode(xshutPins[i], OUTPUT);
        digitalWrite(xshutPins[i], LOW);
    }

    // Enable, initialize, and start each sensor, one by one.
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        // Stop driving this sensor's XSHUT low. This should allow the carrier
        // board to pull it high. (We do NOT want to drive XSHUT high since it is
        // not level shifted.) Then wait a bit for the sensor to start up.
        pinMode(xshutPins[i], INPUT);
        delay(10);

        sensors[i].setTimeout(500);
        if (!sensors[i].init())
        {
        Serial.print("Failed to detect and initialize sensor ");
        Serial.println(i);
        while (1);
        }

        // Each sensor must have its address changed to a unique value other than
        // the default of 0x29 (except for the last one, which could be left at
        // the default). To make it simple, we'll just count up from 0x2A.
        sensors[i].setAddress(0x2A + i);

        sensors[i].startContinuous(50);
    }
    

}

int16_t front() {
    return sensors[0].read();
}


int16_t left()
{
int16_t t = pulseIn(sensorPin1, HIGH);
int16_t d=0;
if (t == 0)
{
// pulseIn() did not detect the start of a pulse within 1 second.
Serial.println("timeout");
}
else if (t > 1850)
{
// No detection.
return(9999);
}
else
{
// Valid pulse width reading. Convert pulse width in microseconds to distance in millimeters.
int16_t d = (t - 1000) * 3 / 4;


// Limit minimum distance to 0.
if (d < 0) { d = 0; } 

// Serial.println(d);
}
return d;
}

int16_t right()
{
int16_t t = pulseIn(sensorPin2, HIGH);
int16_t d = 0;
if (t == 0)
{
// pulseIn() did not detect the start of a pulse within 1 second.
Serial.println("timeout");
}
else if (t > 1850)
{
// No detection.
return(9999);
}
else
{
// Valid pulse width reading. Convert pulse width in microseconds to distance in millimeters.
 d = (t - 1000) * 3 / 4;


// Limit minimum distance to 0.
if (d < 0) { d = 0; } 

Serial.println(d);
}
return d;
}

