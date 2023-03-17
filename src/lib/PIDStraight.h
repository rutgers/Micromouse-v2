#ifndef PIDSTRAIGHT_H
#define PIDSTRAIGHT_H

#include <Arduino.h>

class PIDStraight {
    //this class will contain the relevant code for the PIDStraight command. This command will just use two nested PID loops, the inner
    //using the "black magic" straight drive trick using the IMU heading, and the outer scaling it by the encoder values.
};

#endif