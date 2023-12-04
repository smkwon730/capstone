// RFID.cpp

#include "Arduino.h"
#include "RFID.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);  // RFID 모듈 핀 지정

void RFIDsetup() {
  SPI.begin();          // SPI 초기화 (SPI : 하나의 마스터와 다수의 SLAVE(종속적인 역활)간의 통신 방식)
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED, OUTPUT);

int rfidCheck(void) {
        if (  mfrc522.PICC_IsNewCardPresent() &&  mfrc522.PICC_ReadCardSerial() ){  // RFID가 태그된 경우 RFID의 UID를 읽어옴
          String content= "";
          byte letter;
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          content.toUpperCase(); 
        
          if (content.substring(1) == "10 2C D6 20") {       // 정상적인 RFID가 태그되었을 경우
              return 1;                                      // 1 반환
          }
          else if (content.substring(1) != "10 2C D6 20") {  // 사용자의 RFID UID가 아닐 경우
              return 2;                                      // 2 반환
          }  
    }
  return 0;                                                  // RFID가 태그되지 않았을 경우 0 반환
}
