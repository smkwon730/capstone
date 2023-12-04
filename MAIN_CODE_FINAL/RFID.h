#ifndef RFID_h
#define RFID_h

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9     // reset핀 설정
#define SS_PIN    10    // 데이터를 주고받는 역할의 핀 ( SS = Slave Selector )

extern MFRC522 mfrc522;           // 이 코드에서 MFR522를 이용하기 위해 mfrc객체를 생성

const int LED = 8; 

void RFIDsetup();
int rfidCheck(void);

#endif
