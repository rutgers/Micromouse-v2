#include "distance_sensor.h"

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

