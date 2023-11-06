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

// =================== clock ====================
// check if you press the SET button and increase the menu index
  if(digitalRead(P1)== LOW) 
  {
   menu=menu+1;
  }
  if((digitalRead(P2)== LOW)&&(digitalRead(P3)== LOW))                                                                                                                                                                     
  {
    
    DisplaySetHourAll();
    DisplaySetMinuteAll();
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("ALARM");
    lcd.setCursor(5,1);
    lcd.print(alarmHours, DEC);
    lcd.print(":");
    lcd.print(alarmMinutes, DEC);
    // delay(1000);
    // lcd.clear();
  }
// in which subroutine should we go?
  if (menu==0)
    {
     DisplayDateTime(); // void DisplayDateTime
     //Alarm(); // Alarm control
          }
  if (menu==1)
    {
    DisplaySetHour();
    }
  if (menu==2)
    {
    DisplaySetMinute();
    }
  if (menu==3)
    {
    StoreAgg(); 
    //delay(500);
    menu=0;
    }
    //delay(100);
// ===================================================
  int door = 0;
  int rasp = 0;                 // 라즈베리 전달값
  int limit = 0;
  inout(door);                  // in FSR.h
  Alarm(limit);                 // int click_alarm.h

  //if (limit = 1) {
    rasp = 1;
    if (door = 1) {               //외출
      if (limit = 1) {
      Serial.println("door=1");
      //Serial.println(rasp);       // 1 전송 (case1)
      }
    }

    if (door =2) {                //귀가
      Serial.println("door=2");

      int rfid = 0;
      unsigned long currentMillis = millis();

      while (currentMillis - previousMillis <= delayTime) {   // 5분 제한시간

        RFIDfunc(rfid);

        if (rfid = 1) {               // 본인확인
          rasp = 3
          Serial.println("rfid=1");
          //Serial.println(rfid);     // 3 전송 (case3)
          break;
        }

        if (rfid = 2) {               // 본인확인 실패
          rasp = 2
          Serial.println("rfid=2");
          //Serial.println(rfid);     // 2 전송 (case2)
          break;
        }

        previousMillis = currentMillis;
      }

      if (rfid = 0) {             // 5분 이후 미확인 == 본인확인 실패
        rasp = 2;                 // 본인확인실패와 같은 값 전송
        Serial.println("rfid=0"); // 테스트용
        //Serial.println(rfid);   // 2 전송 (case2)
      }
//    }
  }
}


// void loop() {
//   int door = 0;
//   inout(door);                  // in FSR.h
  
//   if (door = 1) {               //외출
//     Serial.println("door=1");
//     //Serial.println(door);
//   }
//   if (door =2) {                //귀가
//     Serial.println("door=2");
//     //Serial.println(door);
//     //delay(60000*5);
    
//     int check = 1;
//     while (1)
//     RFIDfunc();                   // in RFID.h

//     if (rfid = 1) {               // 본인확인
//       Serial.println("rfid=1");
//       //Serial.println(rfid);
//       int rfid = 0;
//     }

//     if (rfid = 2) {               // 본인확인 실패
//       Serial.println("rfid=2");
//       //Serial.println(rfid);
//       int rfid = 0;
//     }

//     unsigned long currentMillis = millis(); // 현재시간값 가져옴
//     if(currentMillis - previousMillis >= delayTime) { // 1000초 시간이 흘렀는지 체크
//       previousMillis = currentMillis; // 1초가 지나 참임으로 1초 지난 현재시간을 이전시간에 저장
//       if (rfid = 0) {             // 미확인
//         Serial.println("rfid=0");
//         //Serial.println(rfid);
//       }
//     }          
//   }
// }
