#ifndef RFID_h
#define RFID_h

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9     // reset핀 설정
#define SS_PIN    10    // 데이터를 주고받는 역할의 핀( SS = Slave Selector )

extern MFRC522 mfrc522; 
const int LED = 8;                            // LED를 4번핀에 연결합니다.

void RFIDsetup();
int rfidCheck(void);

#endif
