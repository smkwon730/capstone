// InOut.cpp

#include "Arduino.h"
#include "InOut.h"

//-------------------------------------
// 센서 0, 1, 2: 집 안의 문 손잡이에 위치 (손잡이를 잡았는지 확인)
// 센서 3: 문 틈에 위치  (문이 열렸는지 확인)
int sensorval_0;
int sensorval_1;
int sensorval_2;
int sensorval_3;
//-------------------------------------

// 예외 상황 방지 목적 flag
// ex) 외출할 때, 손잡이를 잡고 문을 연 후 나가면서 손잡이를 놓으면 귀가로 인식됨. flag 기능으로 이를 방지
bool doorTriggered = false;  

void InOutsetup() {
  pinMode(FSR0, INPUT);
  pinMode(FSR1, INPUT);
  pinMode(FSR2, INPUT);
  pinMode(FSR3, INPUT);
}

int inout(void) {
    
    sensorval_0 = analogRead(A0);
    sensorval_1 = analogRead(A1);
    sensorval_2 = analogRead(A2);
    sensorval_3 = analogRead(A3);

    static int val = 0;    // 루프가 돌아도 외출/귀가 상태가 초기화되지 않도록 static 선언

        if (sensorval_0 > 100 || sensorval_1 > 100 || sensorval_2 > 100) {  // 문 손잡이 압력 감지
            if (sensorval_3 > 100 && !doorTriggered){           // 문 열림
            val = 1;                                            // 외출 상태
            doorTriggered = true;                               // flag
            }
        }
        if (sensorval_0 < 100 && sensorval_1 < 100 && sensorval_2 < 100) {  // 문 손잡이 압력 X
            if (sensorval_3 > 100 && !doorTriggered){           // 문 열림
            val = 2;                                            // 귀가 상태
            doorTriggered = true;                               // flag
            }
        }
        if (sensorval_3 < 100) {    // 외출 또는 귀가 후 문이 닫힌 후에
            doorTriggered = false;  // 외출/귀가 판단 조건문 활성화
        }
    return val;  // 외출/귀가 상태 반환
}
