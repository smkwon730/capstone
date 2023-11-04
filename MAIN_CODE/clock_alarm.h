#pragma once
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // Display  I2C 20 x 4
RTC_DS1307 RTC;

int P1 = 6; // Button SET MENU'     //��ư
int P2 = 7; // Button +
int P3 = 8; // Button -
int P4 = 9; // SWITCH Alarm

#define LED 13          //�˶�
#define buzzer 10

int hourupg;
int minupg;
int menu = 0;
int setAll = 0;
uint8_t alarmHours = 0, alarmMinutes = 0;  // Holds the current alarm time

void printAllOff() {
    //lcd.setCursor(0, 3);
    lcd.setCursor(0, 1);
    lcd.print("Alarm: Off  ");
}

void RTCsetup() {
    lcd.begin();
    lcd.backlight();
    lcd.clear();

    pinMode(P1, INPUT_PULLUP);
    pinMode(P2, INPUT_PULLUP);
    pinMode(P3, INPUT_PULLUP);
    pinMode(P4, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
    pinMode(buzzer, OUTPUT); // Set buzzer as an output
    printAllOff();
    //Serial.begin(9600);
    Wire.begin();
    RTC.begin();

    if (!RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
        // Set the date and time at compile time
        //RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
      // The default display shows the date and time
    int menu = 0;
}




void DisplayDateTime()
{
    // We show the current date and time
    DateTime now = RTC.now();

    //lcd.setCursor(0, 2);
    lcd.setCursor(0, 0);
    lcd.print("Hour : ");

    if (now.hour() <= 9)
    {
        lcd.print("0");
    }
    lcd.print(now.hour(), DEC);
    hourupg = now.hour();
    lcd.print(":");
    if (now.minute() <= 9)
    {
        lcd.print("0");
    }
    lcd.print(now.minute(), DEC);
    minupg = now.minute();
    lcd.print(":");
    if (now.second() <= 9)
    {
        lcd.print("0");
    }
    lcd.print(now.second(), DEC);
}

void DisplaySetHour()
{
    // time setting
    lcd.clear();
    DateTime now = RTC.now();
    if (digitalRead(P2) == LOW)
    {
        if (hourupg == 23)
        {
            hourupg = 0;
        }
        else
        {
            hourupg = hourupg + 1;
        }
    }
    if (digitalRead(P3) == LOW)
    {
        if (hourupg == 0)
        {
            hourupg = 23;
        }
        else
        {
            hourupg = hourupg - 1;
        }
    }
    lcd.setCursor(0, 0);
    lcd.print("Set time:");
    lcd.setCursor(0, 1);
    lcd.print(hourupg, DEC);
    delay(200);
}

void DisplaySetMinute()
{
    // Setting the minutes
    lcd.clear();
    if (digitalRead(P2) == LOW)
    {
        if (minupg == 59)
        {
            minupg = 0;
        }
        else
        {
            minupg = minupg + 1;
        }
    }
    if (digitalRead(P3) == LOW)
    {
        if (minupg == 0)
        {
            minupg = 59;
        }
        else
        {
            minupg = minupg - 1;
        }
    }
    lcd.setCursor(0, 0);
    lcd.print("Set Minutes:");
    lcd.setCursor(0, 1);
    lcd.print(minupg, DEC);
    delay(200);
}

void StoreAgg()
{
    // Variable saving
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAVING IN");
    lcd.setCursor(0, 1);
    lcd.print("PROGRESS");
    //RTC.adjust(DateTime(yearupg,monthupg,dayupg,hourupg,minupg,0));
    delay(200);
}
void DisplaySetHourAll()// Setting the alarm minutes
{
    while (digitalRead(P1) == HIGH) {

        lcd.clear();

        if (digitalRead(P2) == LOW)
        {
            if (alarmHours == 23)
            {
                alarmHours = 0;
            }
            else
            {
                alarmHours = alarmHours + 1;
            }
        }
        if (digitalRead(P3) == LOW)
        {
            if (alarmHours == 0)
            {
                alarmHours = 23;
            }
            else
            {
                alarmHours = alarmHours - 1;
            }
        }
        lcd.setCursor(0, 0);
        lcd.print("Set HOUR Alarm:");
        lcd.setCursor(0, 1);
        lcd.print(alarmHours, DEC);
        delay(200);
    }
    delay(200);
}

void DisplaySetMinuteAll()// Setting the alarm minutes
{
    while (digitalRead(P1) == HIGH) {

        lcd.clear();
        if (digitalRead(P2) == LOW)
        {
            if (alarmMinutes == 59)
            {
                alarmMinutes = 0;
            }
            else
            {
                alarmMinutes = alarmMinutes + 1;
            }
        }
        if (digitalRead(P3) == LOW)
        {
            if (alarmMinutes == 0)
            {
                alarmMinutes = 59;
            }
            else
            {
                alarmMinutes = alarmMinutes - 1;
            }
        }
        lcd.setCursor(0, 0);
        lcd.print("Set MIN. Alarm:");
        lcd.setCursor(0, 1);
        lcd.print(alarmMinutes, DEC);
        delay(200);
    }
    delay(200);
}
void printAllOn() {
    //lcd.setCursor(0,3);
    lcd.setCursor(0, 1);
    lcd.print("Alarm: ");



    if (alarmHours <= 9)
    {
        lcd.print("0");
    }
    lcd.print(alarmHours, DEC);

    lcd.print(":");
    if (alarmMinutes <= 9)
    {
        lcd.print("0");
    }
    lcd.print(alarmMinutes, DEC);

}

void Alarm() {
    if (digitalRead(P4) == LOW)
    {
        setAll = setAll + 1;
    }
    if (setAll == 0)
    {
        printAllOff();
        noTone(buzzer);
        digitalWrite(LED, LOW);
    }
    if (setAll == 1)
    {

        printAllOn();

        DateTime now = RTC.now();
        if (now.hour() == alarmHours && now.minute() == alarmMinutes)
        {
            lcd.noBacklight();
            DateTime now = RTC.now();
            digitalWrite(LED, HIGH);
            tone(buzzer, 880); //play the note "A5" (LA5)
            //delay(300);
            //tone(buzzer, 698); //play the note "F6" (FA5)
            lcd.backlight();
        }
        else {
            noTone(buzzer);
            digitalWrite(LED, LOW);
        }

    }
    if (setAll == 2)
    {
        setAll = 0;
    }
    delay(200);
}
