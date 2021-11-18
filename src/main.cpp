#include "bot_config.h"

void PDFunc();

bool speedset = false;

void setup()
{
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    Wire.begin();
    Wire.setClock(400000);
    bt.begin(115200);
    setupMotorDriver();
    setupToF();
    setupIMU();

    bt.println("Send ready cmd");
    bool check = true;
    while(check) {
        if (bt.available()) if (bt.read() == 'a') check = false;
        delay(1);
    }
    delay(10);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);

    digitalWrite(STBY, HIGH);
    ENCA.write(0);
    ENCB.write(0);
    // MotorTimer.begin(PDFunc, 1000);
    // TimeCheck = 0;
}

void loop()
{
    Serial.println(L_ToF.readRangeContinuousMillimeters());
    Serial.print(",");
    Serial.print(F_ToF.read());
    Serial.print(",");
    Serial.print(R_ToF.readRangeContinuous());
    delay(1);
}

void PDFunc()
{
    oldErrorW = errorW;
    errorW = wConst - (R_ToF.readRangeSingleMillimeters() - L_ToF.readRangeSingleMillimeters());
    float outputW = (KpW * errorW) + (KdW * (errorW - oldErrorW));
    setPWMA(constrain(300 + outputW, -1023, 1023));
    setPWMB(constrain(300 - outputW, -1023, 1023));
}
