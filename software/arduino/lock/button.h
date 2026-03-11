#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "pins.h"

//extern PIN_LED_P, PIN_LED_M, PIN_SENSE, PIN_HOLD_GND, PIN_HOLD_VCC
// PIN_SENSE must have the pullup (do not use the pins like A6/A7) - for now it is used as analog

#define BLINK_SLOW_DELAY 400
#define BLINK_FAST_DELAY 80

// button press debounce mechanism
const int WINDOW_SIZE = 5;
const float PRESS_THRESHOLD = 0.5; // min 0 max 1
extern int readings[WINDOW_SIZE];
extern int readIndex;
extern bool lastButtonState; 
extern int pressCount;

// controll the led on button
void setupLed();
void turnLedOn();
void turnLedOff();
void fastBlink(int times);
void slowBlink(int times);

// controll the transistor on button
void setupSwitch();
void turnSwitchOff();

// button controll
void setupButton();
void updateButton();
bool isButtonPressed();

#endif