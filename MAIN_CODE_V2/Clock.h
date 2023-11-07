#ifndef Clock_h
#define Clock_h

//************libraries**************//
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

//************************************//
extern LiquidCrystal_I2C lcd; // Display  I2C 16 x 6 // SDA A4, SCL A5
extern RTC_DS1307 RTC;                  

//************Button*****************//
const int P1=2; // Button SET MENU'
const int P2=3; // Button +
const int P3=4; // Button -
const int P4=5; // SWITCH Alarm

//**************Alarm***************//
#define buzzer 6



void Clocksetup();
void clockloop();
void DisplayDateTime();
void DisplaySetHour();
void DisplaySetMinute();
void StoreAgg();
void DisplaySetHourAll();
void DisplaySetMinuteAll();
void printAllOn();
void printAllOff();
void Alarm();

#endif
