//========라이브러리==========
//#include <Wire.h>
//#include <RTClib.h>
//#include <LiquidCrystal_I2C.h>
#include "clock_alarm.h"
#include "FSR.h"
#include "ESP.h"
#include "RfID.h"

//**********************************
unsigned long previousMillis = 0; //이전시간
const long delayTime = 1000*60*5;      // 대기시간

void setup() {
    Serial.begin(9600);
    RTCsetup();             // clock
    FSRsetup();             // 출입
}

void loop() {
  int door = 0;
  inout(door);
  if (door = 1) { //외출
    Serial.println("door=1");
    //Serial.println(door);
  }
  if (door =2) {  //귀가
    Serial.println("door=2");
    //Serial.println(door);
    //delay(60000*5);
    int rfid = 0;
    RFIDfunc();
    
    unsigned long currentMillis = millis(); //현재시간값 가져옴
    if(currentMillis - previousMillis >= delayTime){ //1000초 시간이 흘렀는지 체크
      previousMillis = currentMillis; //1초가 지나 참임으로 1초 지난 현재시간을 이전시간에 저장
      
      if (door = 2){
        if (rfid = 1){
          Serial.println("rfid=1");
          //Serial.println(rfid);
        }
        if (rfid = 2) {
          Serial.println("rfid=2");
          //Serial.println(rfid);
        }
      }
    }
  }
}
