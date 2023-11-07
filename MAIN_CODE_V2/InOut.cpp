#include "Arduino.h"
#include "InOut.h"

void InOutsetup() {
  pinMode(FSR0, INPUT);
  pinMode(FSR1, INPUT);
  pinMode(FSR2, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  //Serial.begin(9600);
}

int inout(void) {
    //---------------------------------
    long duration, distance;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH); // config trigger pin to output mode
    distance = duration * 17 / 1000; // config echo pin to input mode

    //-------------------------------------
    int sensorval_0 = analogRead(A0);
    int sensorval_1 = analogRead(A1);
    int sensorval_2 = analogRead(A2);
    int sensorval_3 = analogRead(A3);

    //--------------------------------

    //int door = 0;
    if (sensorval_3 < 50) {         // 문 여닫힘 감지. 문이 열렸을 경우
        //delay(500);

        if (sensorval_0 > 100 || sensorval_1 > 100 || sensorval_2 > 100) {  // 문 손잡이 압력 감지
            //door = 1; 
            return 1;                                                      // 외출
        }
        else if (sensorval_0 < 100 && sensorval_1 < 100 && sensorval_2 < 100) {
            //door = 2;   
            return 2;                                                    // 귀가
        }
    }
}
