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
    float currA = ENCA.readAndReset() * MM_PER_TICK;
    float currB = ENCB.readAndReset() * MM_PER_TICK;
    oldErrorW = errorW;
    errorW = wConst - ((currA - currB) * DEG_PER_MM_DIFF);
    float outputW = (KpW * errorW) + (KdW * (errorW - oldErrorW));
    setPWMA(constrain(200 + outputW, -1023, 1023));
    setPWMB(constrain(200 - outputW, -1023, 1023));
}
