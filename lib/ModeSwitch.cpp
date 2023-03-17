#include "ModeSwitch.h"

const int buttonPin = 6; // pin reading if switch is pushed

bool modeState = false;  // Returns true or false based on the state of the switch
int buttonState = 0;     //
int lastButtonState = 0; //

ModeSwitch::ModeSwitch() { //constructor
    // initialize the button pin as a input:
    pinMode(buttonPin, INPUT);
    Serial.begin(9600);
}

void ModeSwitch::ModeState() {
    // read the button input pin:
    buttonState = digitalRead(buttonPin);
    // only runs if button is in on position
    if (buttonState != lastButtonState) {
        if (buttonState == HIGH) {
            Serial.println("on");
            modeState = true;
            delay(60);
        } else {

            Serial.prinln("off");
            modeState = false;
            delay(60);
        }

        // returns button state to 0, that way if state does not run non stop
        lastButtonState = buttonState;
    }
}