// RFID.cpp

#include "Arduino.h"
#include "RFID.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);

void RFIDsetup() {
  SPI.begin();          // SPI 초기화(SPI : 하나의 마스터와 다수의 SLAVE(종속적인 역활)간의 통신 방식)
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED, OUTPUT); // 4번핀을 출력으로 설정
}

int rfidCheck(void) {
        if (  mfrc522.PICC_IsNewCardPresent() &&  mfrc522.PICC_ReadCardSerial() ){
          String content= "";
          byte letter;
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          content.toUpperCase(); 
        
          if (content.substring(1) == "10 2C D6 20") { //change here the UID of the card/cards that you want to give access
              return 1;
          }
          else if (content.substring(1) != "10 2C D6 20") {
              return 2;
          }  
    }
  return 0;
}
