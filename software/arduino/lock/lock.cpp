#include "lock.h"


extern MFRC522 rfid;

void setupLock() {
  pinMode(PIN_STRIKE_GND, OUTPUT);
  pinMode(PIN_STRIKE_VCC, OUTPUT);
  digitalWrite(PIN_STRIKE_GND, HIGH);
  digitalWrite(PIN_STRIKE_VCC, HIGH);
}

void unlockDoor() {
  turnLedOff(); delay(200);
  turnLedOn(); delay(200);
  digitalWrite(PIN_STRIKE_GND, LOW);
  delay(1500);
  digitalWrite(PIN_STRIKE_GND, HIGH);
}

void notAllowed() { fastBlink(2); }