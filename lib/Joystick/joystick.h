// Joystick.h
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>

// Declaration of variables
extern uint8_t LeftRight;
extern uint8_t UpDown;
extern int LeftRightVal;
extern int UpDownVal;
extern int prevDirLeftRightVal;
extern int prevDirUpDownVal;
extern int neutralUpDown;
extern int neutralLeftRight;
extern int deadZone;
extern int delayAtZero;
extern int DirLeftRightVal;
extern int DirUpDownVal;

// Function declarations
void setupJoystick();
void readAndUpdateJoystick();

#endif