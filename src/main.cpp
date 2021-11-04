#include "bot_config.h"

void PDFunc();

void setup()
{
    Serial.begin(115200);
    setupMotorDriver();
    // setupToF();
    // setupIMU();

    digitalWrite(STBY, HIGH);
    MotorTimer.begin(PDFunc, 1000);
    TimeCheck = 0;
}

void loop()
{
    if (!stopRecording)
    {
        if (TimeCheck > 1000)
        {
            MotorTimer.end();
            setPWMA(0);
            setPWMB(0);
            stopRecording = true;
        }
    }
}

void PDFunc()
{
    oldErrorW = errorW;
    currA = ENCA.read();
    currB = ENCB.read();
    errorW = wConst + (currA - currB);
    int PWMW = (KpW * errorW) + (KdW * (errorW - oldErrorW));
    setPWMA(150 + PWMW);
    setPWMB(150 - PWMW);
}
