// MAIN_CODE_FINAL.ino

#include "InOut.h"
#include "Clock.h"
#include "RFID.h"


const long delay_time = 1000*10;   // 10 sec (for test)
bool inoutTriggered = false;
bool rfidTriggered = false;
extern int blynk = 0;

void setup() {
  InOutsetup();
  Clocksetup();
  RFIDsetup();
  Serial.begin(9600);
}


void loop() {
  static int previousUart = 0;
  int uart = 0;

  clockLoop();

  int inoutResult = inout();      // inout 함수의 결과를 저장하여 불필요한 호출 방지

  if (getAlarmState() == 1) {    
    if (inoutResult == 1)        // Didn't return home in time
      uart = 1;
  }
  
  if (inoutResult == 1 && inoutTriggered)
    inoutTriggered = false;

  if (getAlarmState() == 0 && !inoutTriggered) {
    if (inoutResult == 2) {         // got home

      inoutTriggered = true;
      
      // 제한시간 이내 rfid 태그 코드
      unsigned long start_time = millis();      
      while(millis() - start_time < delay_time) {
        
        int rfidResult = rfidCheck();
        //Serial.println(millis());
        
        if (rfidResult == 1) {     // Authorized access
          uart = 3;
          rfidTriggered = true;
          break;          
        }
        if (rfidResult == 2) {     // Access denied
          uart = 2;
          rfidTriggered = true;
          break;          
        }
      }
      
      if (!rfidTriggered)
        uart = 2;
    }
  }
  rfidTriggered = false;

  // uart가 0이 아니면서 이전의 uart와 다르고, 0이 아니라면
  // uart를 출력하고 previousUart를 업데이트한다.
  if (uart != previousUart && uart != 0) {
    Serial.println(uart);
    previousUart = uart;
  } else if (uart != previousUart && uart == 0) {
    previousUart = 0;
  }
}
