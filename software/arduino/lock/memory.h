#ifndef MEMORY_H
#define MEMORY_H

#include <EEPROM.h>
#include <Arduino.h>


#define UID_LEN 7
#define MASTER_COUNT 2

// our cards used as master cards
const uint8_t masterUIDs[MASTER_COUNT][UID_LEN] = {
  { 0x04, 0x85, 0x3B, 0xFA, 0xEC, 0x6A, 0x80 },
  { 0x04, 0x7F, 0x82, 0xFA, 0xEC, 0x6A, 0x80 },
};

// --- EEPROM LOGICS ---
#define MAGIC_BYTE 0xBD    // starting byte where the cards starts  
#define MAX_CARDS 128      // max count of cards
#define EEPROM_START_ADDR 1  

// prepared structure to load cards
extern uint8_t storedUIDs[MAX_CARDS][UID_LEN];
extern uint8_t currentCardCount;

void loadCards();
void saveCard(uint8_t *uid);
bool isApproved(uint8_t *uid, uint8_t len);

#endif