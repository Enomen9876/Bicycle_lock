#include "button.h"

int readings[WINDOW_SIZE];
int readIndex = 0;
bool lastButtonState = false; 
int pressCount = 0;

/*LED on BUTTON*/
void setupLed()
{
  pinMode(PIN_LED_VCC, OUTPUT);
  pinMode(PIN_LED_GND, OUTPUT);

  digitalWrite(PIN_LED_GND, LOW);
  digitalWrite(PIN_LED_VCC, HIGH);
}

void turnLedOn()
{
digitalWrite(PIN_LED_VCC, HIGH);
}

void turnLedOff()
{
digitalWrite(PIN_LED_VCC, LOW);
}

void fastBlink(int times) {
  for(int i = 0; i < times; ++i) {
    turnLedOff(); delay(BLINK_FAST_DELAY);
    turnLedOn(); delay(BLINK_FAST_DELAY);
  }
}

void slowBlink(int times) {
  for(int i = 0; i < times; ++i) {
    turnLedOff(); delay(BLINK_SLOW_DELAY);
    turnLedOn(); delay(BLINK_SLOW_DELAY);
  }
}

/*BUTTON hold mechanism*/
void setupSwitch()
{
  pinMode(PIN_HOLD_GND, OUTPUT);
  pinMode(PIN_HOLD_VCC, OUTPUT);

  digitalWrite(PIN_HOLD_VCC, HIGH);
  digitalWrite(PIN_HOLD_GND, LOW);
}

void turnSwitchOff()
{
  digitalWrite(PIN_HOLD_GND, HIGH);
  digitalWrite(PIN_LED_VCC, LOW);
}

/*BUTTON controll*/
void setupButton()
{
  pinMode(PIN_SENSE, INPUT_PULLUP);
  // reset the array as pressed
  for (int i = 0; i < WINDOW_SIZE; i++) readings[i] = 0;
}

void updateButton() {
  readings[readIndex] = analogRead(PIN_SENSE) * (5.0 / 1023.0);
  readIndex = (readIndex + 1) % WINDOW_SIZE;
}

bool isButtonPressed() {
  float sum = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) sum += readings[i];
  return (sum / WINDOW_SIZE < PRESS_THRESHOLD);
}

