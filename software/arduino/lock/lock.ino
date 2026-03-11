#include <SPI.h>

#include "pins.h"
#include "button.h"
#include "memory.h"
#include "lock.h"

MFRC522 rfid(SS_PIN, RST_PIN);

unsigned long startTime;
const unsigned long TIMEOUT_MS = 3000;

void enterLearningMode();

void setup() {
  setupSwitch();
  setupLed();

  setupLock();
  setupButton();

  Serial.begin(115200);
  loadCards();

  SPI.begin();
  rfid.PCD_Init();

  if (rfid.PCD_ReadRegister(MFRC522::VersionReg) == 0x00) {
    while(1) { digitalWrite(PIN_LED_P, !digitalRead(PIN_LED_P)); delay(100); }
  }

  Serial.print("running");
  while(isButtonPressed()){
    updateButton();
  }

  startTime = millis();
}

void loop() {
  updateButton();
  bool currentPress = isButtonPressed();

  // Čítač stisků (detekce uvolnění)
  if (lastButtonState == true && currentPress == false) {
    pressCount++;
    Serial.print("Pocet stisku: ");
    Serial.println(pressCount);
    startTime = millis();
  }
  lastButtonState = currentPress;

  digitalWrite(PIN_LED_P, currentPress ? LOW : HIGH);

  // === ČTENÍ RFID (RC522) ===
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    uint8_t *uid = rfid.uid.uidByte;
    uint8_t uidLength = rfid.uid.size;

    if (isApproved(uid, uidLength)) {
      if (pressCount >= 3) {
        enterLearningMode();
      } else {
        Serial.println("Pristup povolen!");
        unlockDoor();
      }

      pressCount = 0;
      startTime = millis();
    } else {
      Serial.println("Nepovoleny pristup!");
      notAllowed();
      pressCount = 0;
    }

    // Ukončení komunikace s kartou
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  // === Automatické vypnutí ===
  if (millis() - startTime > TIMEOUT_MS) {
    Serial.println("Vypinam...");
    turnSwitchOff();
  }
}

void enterLearningMode() {
  Serial.println("LEARNING MODE: Priloz novou kartu...");
  unsigned long learningStart = millis();
  delay(1000);

  while (millis() - learningStart < 10000) {
    // Indikace učení
    digitalWrite(PIN_LED_P, (millis() / 300) % 2);

    // Čekání na kartu
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      uint8_t *newUid = rfid.uid.uidByte;
      uint8_t newUidLength = rfid.uid.size;

      if (isApproved(newUid, newUidLength)) {
        Serial.println("Karta uz je v databazi!");
        fastBlink(6);
        learningStart = millis();
      } else {
        saveCard(newUid);
        Serial.println("Nova karta ulozena!");
        slowBlink(2);
        learningStart = millis();
      }

      // Ukončení komunikace s kartou
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  }

  // Po ukončení learning módu vrátíme LED do výchozího stavu
  turnLedOn();
}
