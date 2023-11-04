#pragma once

#define TRIG 9 // TRIG pin
#define ECHO 8 // ECHO pin

int FSR0 = A0;
int FSR1 = A1;
int FSR2 = A2;
int FSR3 = A3;

void FSRsetup() {
	pinMode(FSR0, INPUT);
	pinMode(FSR1, INPUT);
	pinMode(FSR2, INPUT);
	pinMode(FSR3, INPUT);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
}

void inout(int door) {
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
            door = 1;                                                       // 외출
        }
        else if (sensorval_0 < 100 && sensorval_1 < 100 && sensorval_2 < 100) {
            door = 2;                                                       // 귀가
        }
    }
}
