// MAIN_CODE_FINAL.ino

// 자체 라이브러리 포함
#include "InOut.h"
#include "Clock.h"
#include "RFID.h"

const long delay_time = 1000*10;     // RFID 태그 제한시간 10 sec (for test)
const long delay_time = 1000*60*5;   // RFID 태그 제한시간 5 min

//========== flag ==========//
bool inoutTriggered = false;       
bool rfidTriggered = false;
//==========================//

void setup() {
  InOutsetup();        // InOut.cpp 의 setup 호출
  Clocksetup();        // Clock.cpp 의 setup 호출
  RFIDsetup();         // RFID.cpp 의 setup 호출
  Serial.begin(9600);  // 라즈베리파이와 통신 시작
}


void loop() {
  static int previousUart = 0;    // 라즈베리파이에 Case 반복전달 방지 기능 변수 선언
  int uart = 0;                   // 라즈베리파이에 전달할 Case 값

  clockLoop();                    // 시계 및 알람 기능 함수 호출

  int inoutResult = inout();      // inout 함수의 결과를 저장하여 불필요한 호출 방지

  if (getAlarmState() == 1) {     // 설정한 알람 시간 초과
    if (inoutResult == 1)         // 미귀가
      uart = 1;                   // 라즈베리파이에 Case 1 전달
  }
  
  if (inoutResult == 1 && inoutTriggered)          // 외출 시 귀가 조건문 활성화
    inoutTriggered = false;                        // inoutTriggered: 귀가 조건문 무한반복 방지 목적 flag

  if (getAlarmState() == 0 && !inoutTriggered) {   // 설정한 알람 시간 내에
    if (inoutResult == 2) {                        // 귀가

      inoutTriggered = true;                       // 귀가 조건문 비활성화 (반복 방지)
      
      // 제한시간 이내 rfid 태그 코드
      unsigned long start_time = millis();      
      while(millis() - start_time < delay_time) {  // delay_time: 태그 제한시간
        
        int rfidResult = rfidCheck();              // rfidCheck 함수의 결과를 저장하여 불필요한 호출 방지
        
        if (rfidResult == 1) {     // Authorized access
          uart = 3;                // 라즈베리파이에 Case 3 전달
          rfidTriggered = true;    // 태그 여부 true
          break;          
        }
        if (rfidResult == 2) {     // Access denied
          uart = 2;                // 라즈베리파이에 Case 2 전달
          rfidTriggered = true;    // 태그 여부 true
          break;          
        }
      }
      if (!rfidTriggered)          // 제한 시간 초과, 태그 미완료
        uart = 2;                  // 라즈베리파이에 Case 2 전달
    }
  }
  rfidTriggered = false;           // 태그 여부 초기화

  // uart가 0이 아니면서 이전의 uart와 다르고, 0이 아니라면
  // uart를 출력하고 previousUart를 업데이트한다.
  if (uart != previousUart && uart != 0) {
    Serial.println(uart);
    previousUart = uart;
  } else if (uart != previousUart && uart == 0) {
    previousUart = 0;
  }
}
