#pragma once

#define TRIG 9 // ������ ������ ��
#define ECHO 8 // ������ �޴� ��

int FSR0 = A0;
int FSR1 = A1;
int FSR2 = A2;

void FSRsetup() {
	pinMode(FSR0, INPUT);
	pinMode(FSR1, INPUT);
	pinMode(FSR2, INPUT);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
}

void inout(int door) {
    //-------�����ļ��� ����----------//
    long duration, distance;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH); // ��ü�� �ݻ�Ǿ� ���ƿ� ������ �ð�
    distance = duration * 17 / 1000; // �����ļ��� �Ÿ����� cm�� ȯ��

    //------�з¼��� ����-----------//
    int sensorval_0 = analogRead(A0);
    int sensorval_1 = analogRead(A1);
    int sensorval_2 = analogRead(A2);
    int sensorval_3 = analogRead(A3);


    //---------����-----------//

    //int door = 0;
    if (sensorval_3 < 50) {
        //delay(500);

        if (sensorval_0 > 100 || sensorval_1 > 100 || sensorval_2 > 100) {
            door = 1;                                                       //����
        }
        else if (sensorval_0 < 100 && sensorval_1 < 100 && sensorval_2 < 100) {
            door = 2;                                                       //�Ͱ�
        }
    }
}
