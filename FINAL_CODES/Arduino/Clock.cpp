// Clock.cpp

#include "Arduino.h"
#include "Clock.h"
#include <SoftwareSerial.h>

//************Variables**************//
int hourupg;
int minupg;
int menu =0;
int setAll =0;
int alarmState = 0; // 알람 상태를 저장할 전역 변수

bool alarmTriggered = false;        // 알람 기능 불필요한 반복 방지 목적 flag

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 RTC;

uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time

void Clocksetup() {

    lcd.begin();
    lcd.backlight();
    lcd.clear();

    pinMode(P1,INPUT_PULLUP);
    pinMode(P2,INPUT_PULLUP);
    pinMode(P3,INPUT_PULLUP);
    pinMode(P4,INPUT_PULLUP);
    printAllOff();
    Serial1.begin(9600);        // NodeMCU 와 통신 시작
    Wire.begin();               // LCD와 I2C 통신 시작
    RTC.begin();

    if (! RTC.isrunning()) {
        //Serial.println("RTC is NOT running!");
        RTC.adjust(DateTime(__DATE__, __TIME__)); // RTC 가 동작하고 있지 않을 시, RTC time을 컴파일 시간으로 설정
    }
        //RTC.adjust(DateTime(__DATE__, __TIME__)); 
}

// delay() 없이 지연시간을 줄 수 있는 함수.
// delay() 를 사용하면 프로그램이 중지되어 다른 작업을 수행할 수 없다는 단점이 있음
// 센서의 변화 또한 감지할 수 없어 원하는 기능을 수행할 수 없으므로 이 함수를 사용
void delayWithoutBlocking(unsigned long duration) {    
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    // 대기
  }
}

void clockLoop() { 

    if(digitalRead(P1)== LOW) {    // 1번 버튼(SET button)을 눌러 menu index 증가
        menu=menu+1;
    }
    if((digitalRead(P2)== LOW)&&(digitalRead(P3)== LOW)) {    // 2, 3번 버튼을 동시에 눌러 알람시간을 설정

        DisplaySetHourAll();
        DisplaySetMinuteAll();
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("ALARM");
        lcd.setCursor(5,1);
        lcd.print(alarmHours, DEC);
        lcd.print(":");
        lcd.print(alarmMinutes, DEC);
        delayWithoutBlocking(1000);
        lcd.clear();
    }

    if (menu==0) {            // 초기화면 (현재 시간과 알람 정보를 보여줌)
        DisplayDateTime();    // void DisplayDateTime
        Alarm();              // Alarm control
    }
    if (menu==1){
        DisplaySetHour();     // 현재 시간 설정
    }
    if (menu==2){
        DisplaySetMinute();   // 현재 분 설정
    }
    if (menu==3){             // 설정 시간 저장 후 초기화면으로
        StoreAgg(); 
        delayWithoutBlocking(500);
        menu=0;
    }
    delayWithoutBlocking(100);
}

void DisplayDateTime() {
    // show the current date and time
    DateTime now = RTC.now();

    lcd.setCursor(0, 0);
    lcd.print("Time : ");

    if (now.hour() <= 9) {
        lcd.print("0");
    }
    lcd.print(now.hour(), DEC);
    hourupg = now.hour();
    lcd.print(":");
    if (now.minute() <= 9) {
        lcd.print("0");
    }
    lcd.print(now.minute(), DEC);
    minupg = now.minute();
    lcd.print(":");
    if (now.second() <= 9) {
        lcd.print("0");
    }
    lcd.print(now.second(), DEC);
}

void DisplaySetHour() {
    // time setting
    lcd.clear();
    DateTime now = RTC.now();
    if (digitalRead(P2) == LOW) {
        
        if (hourupg == 23) {
            hourupg = 0;
        }
        else {
            hourupg = hourupg + 1;
        }
    }
    if (digitalRead(P3) == LOW) {

        if (hourupg == 0) {
            hourupg = 23;
        }
        else {
            hourupg = hourupg - 1;
        }
    }
    lcd.setCursor(0, 0);
    lcd.print("Set time:");
    lcd.setCursor(0, 1);
    lcd.print(hourupg, DEC);
    delayWithoutBlocking(200);
}

void DisplaySetMinute() {
    // Setting the minutes
    lcd.clear();
    if (digitalRead(P2) == LOW) {
        
        if (minupg == 59) {
            minupg = 0;
        }
        else {
            minupg = minupg + 1;
        }
    }
    if (digitalRead(P3) == LOW) {

        if (minupg == 0) {
            minupg = 59;
        }
        else {
            minupg = minupg - 1;
        }
    }
    lcd.setCursor(0, 0);
    lcd.print("Set Minutes:");
    lcd.setCursor(0, 1);
    lcd.print(minupg, DEC);
    delayWithoutBlocking(200);
}

void StoreAgg() {
    // Variable saving
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAVING IN");
    lcd.setCursor(0, 1);
    lcd.print("PROGRESS");
    RTC.adjust(DateTime(0,0,0,hourupg,minupg,0));
    delayWithoutBlocking(200);
}
void DisplaySetHourAll() {   // Setting the alarm minutes
    while (digitalRead(P1) == HIGH) {

        lcd.clear();

        if (digitalRead(P2) == LOW)
        {
            if (alarmHours == 23) {
                alarmHours = 0;
            }
            else {
                alarmHours = alarmHours + 1;
            }
        }
        if (digitalRead(P3) == LOW)
        {
            if (alarmHours == 0) {
                alarmHours = 23;
            }
            else {
                alarmHours = alarmHours - 1;
            }
        }
        lcd.setCursor(0, 0);
        lcd.print("Set HOUR Alarm:");
        lcd.setCursor(0, 1);
        lcd.print(alarmHours, DEC);
        delayWithoutBlocking(200);
    }
    delayWithoutBlocking(200);
}

void DisplaySetMinuteAll() {     // Setting the alarm minutes
    while (digitalRead(P1) == HIGH) {

        lcd.clear();
        if (digitalRead(P2) == LOW)
        {
            if (alarmMinutes == 59) {
                alarmMinutes = 0;
            }
            else {
                alarmMinutes = alarmMinutes + 1;
            }
        }
        if (digitalRead(P3) == LOW)
        {
            if (alarmMinutes == 0) {
                alarmMinutes = 59;
            }
            else {
                alarmMinutes = alarmMinutes - 1;
            }
        }
        lcd.setCursor(0, 0);
        lcd.print("Set MIN Alarm:");
        lcd.setCursor(0, 1);
        lcd.print(alarmMinutes, DEC);
        delayWithoutBlocking(200);
    }
    delayWithoutBlocking(200);
}
void printAllOn() {
    lcd.setCursor(0, 1);
    lcd.print("Alarm: ");

    if (alarmHours <= 9) {
        lcd.print("0");
    }
    lcd.print(alarmHours, DEC);

    lcd.print(":");
    if (alarmMinutes <= 9) {
        lcd.print("0");
    }
    lcd.print(alarmMinutes, DEC);
}

void printAllOff() {
  lcd.setCursor(0,1);
  lcd.print("Alarm: Off  ");  
}

int getAlarmState() {   // 외부 코드에서 현재 알람 상태를 읽을 수 있게 함
    return alarmState;
}
void setAlarmState(int state) {     // 알람 상태를 변경할 때 사용
    alarmState = state;
}

int Alarm(void) {

    if (Serial1.available()){
      String blynk = Serial1.readString();
      blynk.trim();
      if (blynk == "on"){
        setAll = 1;
      }
      if (blynk == "off"){
        setAll = 0;
      }
      //Serial.println(blynk);
    }
    if (digitalRead(P4) == LOW) {
        setAll = setAll + 1;
    }
    
    if (setAll == 0) {
        printAllOff();
        noTone(buzzer);
        setAlarmState(0); // 알람 상태 설정 (off)
    }
    if (setAll == 1) {    // 알람 기능 활성화 상태

        printAllOn();
        DateTime now = RTC.now();

        // 설정한 알람 시간과 현재시각 일치 시
        if (now.hour() == alarmHours && now.minute() == alarmMinutes && !alarmTriggered) {
            lcd.noBacklight();
            DateTime now = RTC.now();
            
            lcd.backlight();
            setAlarmState(1);       // 알람 상태를 1로 설정 

            alarmTriggered = true;  // 다음 알람시간까지의 중복동작을 방지하기 위한 flag 
        }
        if (now.second() == 0) {    // 알람이 울리고 1분 후 알람 조건문 활성화
            alarmTriggered = false;
        }
        else {
            setAlarmState(0);       // 알람에 의한 동작을 한 번만 실행한 후 alarm state를 다시 0으로 변경
        }
    }
    if (setAll == 2) {              // 4번 버튼을 눌러 알람기능 off
        setAll = 0;
        setAlarmState(0);
    }
    return;
}
