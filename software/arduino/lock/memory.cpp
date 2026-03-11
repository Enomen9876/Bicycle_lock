#include "memory.h"

uint8_t storedUIDs[MAX_CARDS][UID_LEN];
uint8_t currentCardCount = 0;

void loadCards() {
  if (EEPROM.read(0) != MAGIC_BYTE) {
    EEPROM.write(0, MAGIC_BYTE);
    EEPROM.write(1, 0); 
  }
  
  currentCardCount = EEPROM.read(1);
  if (currentCardCount > MAX_CARDS) currentCardCount = 0;

  for (int i = 0; i < currentCardCount; i++) {
    for (int j = 0; j < UID_LEN; j++) {
      storedUIDs[i][j] = EEPROM.read(EEPROM_START_ADDR + 1 + (i * UID_LEN) + j);
    }
  }
}

void saveCard(uint8_t *uid) {
  if (currentCardCount >= MAX_CARDS) return;

  int addr = EEPROM_START_ADDR + 1 + (currentCardCount * UID_LEN);
  for (int j = 0; j < UID_LEN; j++) {
    EEPROM.write(addr + j, uid[j]);
    storedUIDs[currentCardCount][j] = uid[j];
  }
  currentCardCount++;
  EEPROM.write(1, currentCardCount);
}

bool isApproved(uint8_t *uid, uint8_t len) {
  // 1. Kontrola Master karet v kódu
  for (uint8_t i = 0; i < MASTER_COUNT; i++) {
    bool match = true;
    for (uint8_t j = 0; j < UID_LEN; j++) {
      if (uid[j] != masterUIDs[i][j]) { match = false; break; }
    }
    if (match) return true;
  }

  // 2. Kontrola uložených karet v EEPROM
  for (uint8_t i = 0; i < currentCardCount; i++) {
    bool match = true;
    for (uint8_t j = 0; j < UID_LEN; j++) {
      if (uid[j] != storedUIDs[i][j]) { match = false; break; }
    }
    if (match) return true;
  }

  return false;
}