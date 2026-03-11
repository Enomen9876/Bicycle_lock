#ifndef LOCK_H
#define LOCK_H

#include <MFRC522.h>

#include "pins.h"
#include "button.h"
#include "memory.h"

void setupLock();
void unlockDoor();
void notAllowed();

#endif