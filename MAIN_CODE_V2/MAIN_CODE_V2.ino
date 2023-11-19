// MAIN_CODE_V2.ino

#include "InOut.h"
#include "Clock.h"
#include "RFID.h"

//unsigned long previous_time = 0;
//const long delay_time = 1000*60*5;   // 5 min
const long delay_time = 1000*10;   // 30 sec (for test)

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
  int inoutResult = inout(); // inout 함수의 결과를 저장하여 불필요한 호출 방지

  if (getAlarmState() == 1) {    

    if (inoutResult == 1) {           // Didn't return home in time
      //Serial.println("out");
      uart = 1;
    }
    
    if (inoutResult == 2) {         // got home
      //Serial.println("in");
    
    // 제한시간(5분) 이내 rfid 태그 코드
      unsigned long start_time = millis();      
      while(millis() - start_time < delay_time) {
        clockLoop();
        int rfidResult = rfidCheck();
        
        if (rfidResult == 1) {     // Authorized access
          uart = 3;
          break;
        }
        if (rfidResult == 2) {     // Access denied
          uart = 2;
          break;
        }
        if (millis() - start_time >= delay_time) {
          uart = 2;
          break;
        }
      }
      
    }
  }
  // uart가 0이 아니면서 이전의 uart와 다르다면, 그리고 uart가 0이 아니라면
  // uart를 출력하고 previousUart를 업데이트한다.
  if (uart != previousUart && uart != 0) {
    Serial.print("uart: ");
    Serial.println(uart);
    previousUart = uart;
  } else if (uart != previousUart && uart == 0) {
    previousUart = 0;
  }
}







  /*//======= inout() test =========//
  inout();
  Serial.println(inout());
  //======= Alarm() test =========//
  while (Alarm()) {
    Serial.println("alarm: 1");
  }
  Serial.println("alarm: 0");
  //====== rfidCheck() test ======//
  unsigned long current_time = millis();

  while(current_time - previous_time >= delay_time) {
    Serial.println(current_time);
    previous_time = current_time;

    if (rfidCheck() == 1) {     // Authorized access
      Serial.println("Authorized access");
      break;
    }
    if (rfidCheck() == 2) {     // Access denied
      Serial.println("Access denied");
      break;
    }
  }
  if (rfidCheck() != 1 || rfidCheck() != 2) {                      // no access in 5 min
    if (current_time - previous_time >= delay_time) {
      Serial.println("no access in 5 min");
    }
  }*/
