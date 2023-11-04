#pragma once

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9     // reset�� ����
#define SS_PIN    10    // �����͸� �ְ��޴� ������ ��( SS = Slave Selector )

MFRC522 mfrc522(SS_PIN, RST_PIN);           // �� �ڵ忡�� MFR522�� �̿��ϱ� ���� mfrc��ü�� ������ �ݴϴ�.

int rfidLED = 4;                            // LED�� 4���ɿ� �����մϴ�.


void RFIDsetup() {
    Serial.begin(9600);                     // �ø��� ���, �ӵ��� 9600
    SPI.begin();                             // SPI �ʱ�ȭ(SPI : �ϳ��� �����Ϳ� �ټ��� SLAVE(�������� ��Ȱ)���� ��� ���)
    mfrc522.PCD_Init();   // Initiate MFRC522
    pinMode(rfidLED, OUTPUT);                 // 4������ ������� ����

}

void RFIDfunc(int rfid) {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {    //  �±� ������ ���� �ʾ����� �Ǵ� ���̵� �������� �ʾ�����
        delay(500);
        return;                                   // return
    }

    Serial.print("UID tag :");
    String content = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    //Serial.println();
    //Serial.print("Message : ");
    content.toUpperCase();
    

    if (content.substring(1) == "D1 2A D4 1C") //change here the UID of the card/cards that you want to give access
    {
        //Serial.println("Authorized access");
        rfid = 1;                               // ����Ȯ��
        digitalWrite(rfidLED, HIGH);                // 3���� �� ����� led ������         
        //delay(1000);
        //digitalWrite(rfidLED, LOW);
        //delay(1000);

    }
    else {                                   // �ٸ� �±� ID�� ���
        rfid = 2;                               // ħ��
        digitalWrite(rfidLED, LOW);               // 4���� �� ����� led ������
        //Serial.println("Access denied");        // �ø��� ����Ϳ� "Who are you?" ��� 
        //delay(500);
    }
}
