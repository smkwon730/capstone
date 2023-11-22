// Clock.cpp

#include "Arduino.h"
#include "Clock.h"

//************Variables**************//
int hourupg;
int minupg;
int menu =0;
int setAll =0;
int alarmState = 0; // 알람 상태를 저장할 전역 변수

bool alarmTriggered = false;

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
    pinMode(buzzer, OUTPUT); // Set buzzer as an output
    printAllOff();
    //Serial.begin(9600);
    Wire.begin();
    RTC.begin();

    if (! RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
        //RTC.adjust(DateTime(__DATE__, __TIME__)); // Set the date and time at compile time
    }
        RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
        // The default display shows the date and time
        //int menu=0;
}

void delayWithoutBlocking(unsigned long duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    // 대기하고자 하는 시간까지 아무 작업도 수행하지 않음
  }
}

void clockLoop() { 
// check if you press the SET button and increase the menu index
    if(digitalRead(P1)== LOW) {
    menu=menu+1;
    }
    if((digitalRead(P2)== LOW)&&(digitalRead(P3)== LOW)) {

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

    if (menu==0) {
        DisplayDateTime(); // void DisplayDateTime
        Alarm(); // Alarm control
    }
    if (menu==1){
        DisplaySetHour();
    }
    if (menu==2){
        DisplaySetMinute();
    }
    if (menu==3){
        StoreAgg(); 
        delayWithoutBlocking(500);
        menu=0;
    }
    delayWithoutBlocking(100);
}

void DisplayDateTime() {
    // We show the current date and time
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

void DisplaySetMinute()
{
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

void StoreAgg()
{
    // Variable saving
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAVING IN");
    lcd.setCursor(0, 1);
    lcd.print("PROGRESS");
    RTC.adjust(DateTime(0,0,0,hourupg,minupg,0));
    delayWithoutBlocking(200);
}
void DisplaySetHourAll()// Setting the alarm minutes
{
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

void DisplaySetMinuteAll()// Setting the alarm minutes
{
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
    //lcd.setCursor(0,3);
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
  //lcd.setCursor(0, 3);
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
    if (digitalRead(P4) == LOW) {
        setAll = setAll + 1;
    }
    if (setAll == 0) {
        printAllOff();
        noTone(buzzer);
        setAlarmState(0); // 알람 상태 설정
    }
    if (setAll == 1) {

        printAllOn();
        DateTime now = RTC.now();
        
        if (now.hour() == alarmHours && now.minute() == alarmMinutes && !alarmTriggered) {
            lcd.noBacklight();
            DateTime now = RTC.now();
            //tone(buzzer, 880); //play the note "A5" (LA5)
            
            lcd.backlight();
            setAlarmState(1);
            Serial.println("buzzer on");            

            alarmTriggered = true;  // Alarm triggered, prevent further execution until the next alarm time 
        }
        if (now.second() == 0) {
            alarmTriggered = false;
        }
        else {
            noTone(buzzer);
            //Serial.println("buzzer off");
            setAlarmState(0);
        }
    }
    if (setAll == 2) {
        setAll = 0;
        setAlarmState(0);
    }
    //delayWithoutBlocking(200);
    return;
}
