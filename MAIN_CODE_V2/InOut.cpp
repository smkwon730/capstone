// InOut.cpp

#include "Arduino.h"
#include "InOut.h"

//-------------------------------------
int sensorval_0;
int sensorval_1;
int sensorval_2;
int sensorval_3;
//-------------------------------------

void InOutsetup() {
  pinMode(FSR0, INPUT);
  pinMode(FSR1, INPUT);
  pinMode(FSR2, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  //Serial.begin(9600);
}

int inout(void) {
    
    sensorval_0 = analogRead(A0);
    sensorval_1 = analogRead(A1);
    sensorval_2 = analogRead(A2);
    sensorval_3 = analogRead(A3);

    static int val = 0;

    //int door = 0;
    //if (sensorval_3 < 50) {         // 문 여닫힘 감지. 문이 열렸을 경우
    
    if (sensorval_3 > 100) {         // 테스트용
        //delay(100);

        if (sensorval_0 > 100 || sensorval_1 > 100 || sensorval_2 > 100) {  // 문 손잡이 압력 감지
            val = 1;                                            // 외출
        }
        else if (sensorval_0 < 100 && sensorval_1 < 100 && sensorval_2 < 100) {
            val = 2;                                            // 귀가
        }
    }
    Serial.print("inout: ");
    Serial.println(val);  //test
    return val;
}
