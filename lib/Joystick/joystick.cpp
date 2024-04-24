// Joystick.cpp
#include "Joystick.h"

uint8_t LeftRight = 36;
uint8_t UpDown = 39;
int LeftRightVal = 0;
int UpDownVal = 0;
int prevDirLeftRightVal = 1;
int prevDirUpDownVal = 1;
int neutralUpDown, neutralLeftRight; // Neutral positions for each axis
int deadZone = 0.07 * 4095 / 2;  // 7% of range, but this will be relative to neutral position
int delayAtZero = 500; // Delay at zero
int DirLeftRightVal = 0;  // Initial value for direction on the LeftRight axis
int DirUpDownVal = 0;

void setupJoystick() {
    Serial.begin(115200);
    pinMode(LeftRight, INPUT);
    pinMode(UpDown, INPUT);

    // Determine neutral position for each axis. Adjust as needed.
    for (int i = 0; i < 100; i++) {
        neutralLeftRight += analogRead(LeftRight);
        neutralUpDown += analogRead(UpDown);
        delay(1);
    }

    neutralLeftRight /= 100;
    neutralUpDown /= 100;
}

void readAndUpdateJoystick() {
    LeftRightVal = analogRead(LeftRight);
    UpDownVal = analogRead(UpDown);
    DirLeftRightVal = (LeftRightVal > neutralLeftRight + deadZone) - (LeftRightVal < neutralLeftRight - deadZone);
    DirUpDownVal = (UpDownVal > neutralUpDown + deadZone) - (UpDownVal < neutralUpDown - deadZone);

    int mappedLeftRightVal = (abs(LeftRightVal - neutralLeftRight) < deadZone) ? 0 : map(LeftRightVal, 0, 4095, -400, 400);
    int mappedUpDownVal = (abs(UpDownVal - neutralUpDown) < deadZone) ? 0 : map(UpDownVal, 0, 4095, -400, 400);

    if (DirLeftRightVal != prevDirLeftRightVal && mappedLeftRightVal != 0) {
        mappedLeftRightVal = 0;
        delay(delayAtZero);
    }

    if (DirUpDownVal != prevDirUpDownVal && mappedUpDownVal != 0) {
        mappedUpDownVal = 0;
        delay(delayAtZero);
    }

    prevDirLeftRightVal = DirLeftRightVal;
    prevDirUpDownVal = DirUpDownVal;

    Serial.println("LeftRight Value: " + String(mappedLeftRightVal) + " UpDown Value: " + String(mappedUpDownVal));

    delay(500);
}