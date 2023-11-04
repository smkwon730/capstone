/*
   Clock and timer developed by Ronald Siemons
   The sketch   is free software and for anybody free to use on his or her own risk, if there would   be.

   TO KNOW!!!!!!!!!!
   Make a choice out of the menu what show up   on the display when pushing the select button. Every
   two secconds an other   choice to make will be shown as long as the select button is pushed. By letting   it go
   the last shown choice will be made.

   What it does:
   The   clock and timer switches on and off a relais.
   The clock is made for stand   alone usesage. So one can set the time and the alarm on the device itself
   without   the need of an USB connection to a computer.
   The general idee:
   The clock   is mate as part of a system to switch on and off the lights in a shop. This part   is made
   for the neon lights at the front of the shop. The lights only work   in the evening and go off every day at the same time.
   So the alarm is set   on the off switch of the lights and as the time off lighting up the lights is known   the timer works
   backwords from the known of switching time back during a certain   period to the starting moment of the lights.
   The period of lighting depends   on the pereod of the year and can be set by hand or automatic.
   As it has to   work all year round without setting and resetting the system every now and again
    daylight saving settings ar built in also. Readjustment of the daylight savings   has to be done
   for the specific country it is going to be used.
*/
#include   <Pushbutton.h> // part of arduino library. Search in manage libraries.
#include   <EEPROM.h>
#include <RTClib.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include   <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x3F, 2,   1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
RTC_DS1307 RTC;

const   int DS1307 = 0x68; // Address of DS1307 see data sheets //??????????
const int   lightAlarm_pin = 6;
const int selectButton_pin = 5;
const int nextButton_pin   = 4;
const int upButton_pin = 3;
const int downButton_pin = 2;
const int   alarmButton_pin = A1; // gets an other function later
const boolean alarmSystem_pin   = 7;
const boolean timerLightOnLED_pin = 8;

Pushbutton selectButton(selectButton_pin);   //equal to pinMode(selectButton_Pin,INPUT_PULLUP);
Pushbutton nextButton(nextButton_pin);
Pushbutton   upButton(upButton_pin);
Pushbutton downButton(downButton_pin);
Pushbutton   alarmButton(alarmButton_pin);

enum mainSelection {
  ALARM_ON_OFF = 1,    //switching alarm on or off            = 1
  ALARM_SEASON,       //change   the time of starting          = 2
  ALARM_SET,          //to set the alarm start   and stop time = 3
  CLOCK_SET,          //to set time and date                 =   4
  AlARM_AUTO,         //Light timer automatic or manual      = 5
  RUN_CLOCK            //Reading and displaying time and date = 6
};
enum mainSelection   key = RUN_CLOCK;

int alarmMinuteStart = 1;   //Time timer starts according   to the settings of alarmPlus and alarmxxxStop
int alarmHourStart = 1;     //Time   timer starts according to the settings of alarmPlus and alarmxxxStop
int alarmMinuteStop   = 2;    //Time the timer has been set to stop
int alarmHourStop = 2;      //Time   teh timer has been set to stop Minimum 6
int alarmPlus = 0;          //Working   hours of the lights Maximum 6
boolean alarmOnOff = false; //Turns the alarm On/Off   //off is false and on is true
boolean autoManual = false;  //Timer automatic   or manual
int alarmOffSet = 6;
int yr = 2000;
int mo = 1;
int dy = 1;
int   hr = 0;
int mn = 0;
int setYr = 00;
int setMo = 1;
int setDy = 1;
int   setHr = 0;
int setMn = 0;
boolean daylightSaving = true;
boolean aBlink   = true;
const int loopCheckMax = 80; //duration of the loops in the settings
int   loopCheck = 0;  //to controll if if there is any interaction
unsigned long  clockDisplayCheck   = millis(); //counter in the loop
int lcdDisplayCheck = 0; //counter for controling   the time the lcd will be on
boolean timerLight = false; //set the relais for   the alarm lights
int timerLightStage = 0;


//((((((((((((((((((((((((((((SETUP))))))))))))))))))))))))))))))))
void   setup() {
  pinMode(lightAlarm_pin, OUTPUT);
  pinMode(alarmSystem_pin, OUTPUT);
   pinMode(timerLightOnLED_pin, OUTPUT);
  lcd.begin(16, 2);
  Wire.begin();
   RTC.begin();
  Serial.begin(9600);
  clockRunning();

  lcd.clear();
   lcd.on();
  displayLcdFirstLine( 2);
  readTimeNow();
  displayTimeNow(2);
   readAlarm();
}
//((((((((((((((((((((((((((((----))))))))))))))))))))))))))))))))
//***************************^LOOP^*******************************
void   loop() {
  key = RUN_CLOCK;
  lightManualyOnOff(); //to switch the relay manualy   when the alarm has been turned off
  lcdLight(); //turns on the lcd on demand
   if ((millis() - clockDisplayCheck) > 500) //Reading and displaying date and time   continuesly every 0.5 second
  {
    clockRunning();
    clockDisplayCheck   = millis(); //sets the timer variable
    readTimeNow(); //reading time out of   the Real Time Clock
    displayTimeNow(2);  //displaying the time now and the   digit to blink
    readAlarm(); //reading the alarm settings out off the Eeprom
     displayLcdFirstLine( 0);  //Basic display line
    alarmActivate(); //setting   the alarm
    timerLightOnOff();
  }

  if (selectButton.getSingleDebouncedPress())
   {
    lcdDisplayCheck = 0; // lcd back to maximum time
    lcdLight(); //turns   lcd on
    mainKeySelection(); // reads keySelection   ALARM_ON_OFF,ALARM_SEASON,ALARM_SET,CLOCK_SET,RUN_CLOCK,
     //  lcd.setCursor(0, 0);

    switch (key) {
      case ALARM_ON_OFF:
         setTimerOnOff();
        break;

      case ALARM_SEASON:
        setTimerPlus();
         break;

      case ALARM_SET:
        setHr = alarmHourStop;
         setMn = alarmMinuteStop;
        lcd.setCursor(0, 0);
        lcd.print("Set   alarm time  ");
        presetClockTime(alarmOffSet); //Gives an off-set to   the alarm stopping time of 6? hours
        if (setHr == 12) { //to prevent the   timer passing mid-night
          setHr = 11;
          setMn = 59;
        }
         saveAlarmSet();
        //   readAlarm();
        break;

      case   CLOCK_SET:
        presetClockDate();
        readTimeNow();  //reading the   current time to display in presetClockTime
        setHr = hr;
        setMn   = mn;
        lcd.setCursor(0, 0);
        lcd.print("Set clock time  ");
         presetClockTime(0); //Time can be set starting at zero. The function is   also in use to set the alarm stopping time
        saveClock();
        break;

       case AlARM_AUTO:
        alarmAutoManual();
        break;
    }
   }
}
//***************************~~*******************************

//function   clock running// A check about the clock being running
void clockRunning() {
   if (!RTC.begin() || !RTC.isrunning()) {
    while (!selectButton.getSingleDebouncedPress   ()) {
      if (digitalRead(alarmSystem_pin) == LOW) digitalWrite(alarmSystem_pin,   HIGH);
      else digitalWrite(alarmSystem_pin, LOW);
      delay(100);
     }
  }
}

//function lcdTimerLights // timer for the lcd lcdDisplayCheck   incrises with every half a second
void lcdLight() {
  if (alarmButton.getSingleDebouncedPress())   lcdDisplayCheck = 0;
  lcdDisplayCheck++;
  if (lcdDisplayCheck < 900) { //times   half a second the display will be on
    lcd.display();
    lcd.backlight();
   }
  else {
    lcd.noDisplay();
    lcd.noBacklight();
    if (lcdDisplayCheck   > 2000) lcdDisplayCheck = 900; //resetting the variable to prevent the figure gets   out of his boundary while the light stays off
  }
}

///function timerLightOnOff   // works in automatic and manual to set the pin for the relais
void timerLightOnOff()   {
  if (timerLight) {
    digitalWrite(lightAlarm_pin, LOW);  // the relay   activates to zero
    digitalWrite(timerLightOnLED_pin, HIGH);
  } else  {
     digitalWrite(lightAlarm_pin, HIGH);
    digitalWrite(timerLightOnLED_pin,   LOW);
  }

}

///function lightManualyOnOff // to set the timer relais   by hand
void lightManualyOnOff() {
  if (!alarmOnOff) {
    if (nextButton.getSingleDebouncedPress())   {
      Serial.print (timerLight);
      timerLight = !timerLight;
    }
   }
}
////function readTimeNow////reading date and time
void readTimeNow()   {
  DateTime now = RTC.now();
  yr = now.year();
  mo = now.month();
   dy = now.day();
  hr = now.hour();
  mn = now.minute();
}

///function   readAlarm////to read the alarm settings from the Eeprom
void readAlarm() {
   alarmHourStop = EEPROM.read(0);
  delay(15);
  alarmMinuteStop = EEPROM.read(1);
   delay(15);
  alarmPlus = EEPROM.read(2);
  delay(15);
  alarmOnOff =   EEPROM.read(3);
  delay(50);
  autoManual = EEPROM.read(4);
  delay(50);
   if (alarmHourStop <= alarmOffSet) alarmHourStop = alarmOffSet + 1; //to prevent   the alarm storting before the day has begun
  alarmHourStart = alarmHourStop   - alarmPlus;
  alarmMinuteStart = alarmMinuteStop;//calculating the timer stopping   time
  if (alarmAutoManual) automaticTimer();  //Sets the timer automaticaly   in the preset time scedule
  dayLightSaving();
}

void alarmActivate()   {
  if (alarmOnOff) {  //Checking if the alarm has to be activated

    if   ((hr - alarmHourStart) == 0)
    {
      if (mn >= alarmMinuteStart)
        timerLight   = true;
      else timerLight = false;
    }
    else if (hr - alarmHourStop   == 0)
    {
      if ( mn <= alarmMinuteStop)
        timerLight = true;
       else timerLight = false;
    }
    else if ((alarmHourStop - hr < alarmPlus)   && (alarmHourStop - hr >= 1)) timerLight = true;
    else timerLight = false;

   }
  // else timerLight = false; // has to e removed ???????????????????????????????????
}

////function   display time now////display time
void displayTimeNow(int digitBlink) {
  char   textDT[16];
  sprintf(textDT, "%02d:%02d %02d/%02d/%04d", hr, mn, dy, mo,   yr); //puts int variabels in the rigth format into textDT
  lcd.setCursor(0,   1);
  lcd.print(textDT); //prints in four and two digits
  lcd.setCursor(digitBlink,   1);
  if (aBlink) lcd.print(" "); else lcd.print(":");
  aBlink = !aBlink;
}


////function   displayLcdFirstLine////displays the first line
void displayLcdFirstLine( int   blinking) { //blinking 0 is back to RUN position

  if (blinking > 0) {
     for (int i = 1; i <= blinking; i++) {
      lcd.setCursor(0, 0);
      lcd.print("                 ");
      lcd.setCursor(0, 0);
      delay(400);
      lcd.print("TIMER-1.00       ");  //Titel
      lcd.setCursor(11, 0);
      if (alarmOnOff == false)   lcd.print("off~ "); else lcd.print(" on~ ");
      lcd.setCursor(15, 0);
       if (autoManual) lcd.print("A"); else lcd.print(alarmPlus);

      delay(400);
     }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("TIMER-1.00");   //Titel
    lcd.setCursor(10, 0);
    if (alarmOnOff == false) lcd.print("   off~ "); else lcd.print("  on~ ");
    lcd.setCursor(15, 0);
    if (autoManual)   lcd.print("A"); else lcd.print(alarmPlus);
    delay(400);
  }
}



////function   mainKeySelection////make a selection to set timer on/off, alarm season, alarm set,   clock set
void mainKeySelection() {
  unsigned long started = millis();
   delay(10);
  unsigned long moment;
  const int laps = 2000;
  int state   = 0;
  int lapTime = 0;

  while (selectButton.isPressed()) {
    moment   = millis();
    delay(100);
    lapTime = moment - started;
    if (((lapTime)   >= (0 * laps)) && ((lapTime) <= (1 * laps))) state = 1;
    else if (((lapTime)   >= (1 * laps)) && ((lapTime) <= (2 * laps))) state = 2;
    else if (((lapTime)   >= (2 * laps)) && ((lapTime) <= (3 * laps))) state = 3;
    else if (((lapTime)   >= (3 * laps)) && ((lapTime) <= (4 * laps))) state = 4;
    else if (((lapTime)   >= (4 * laps)) && ((lapTime) <= (5 * laps))) state = 5;
    else if (((lapTime)   >= (5 * laps)) && ((lapTime) <= (6 * laps))) state = 6;
    delay(500);
    switch   (state) {
      case 1:
        lcd.setCursor(0, 1);
        lcd.print("set   alarm on/off");
        break;
      case 2:
        lcd.setCursor(0,   1);
        lcd.print("Timer hours set ");
        break;
      case   3:
        lcd.setCursor(0, 1);
        lcd.print("Alarm time set! ");
         break;
      case 4:
        lcd.setCursor(0, 1);
        lcd.print("Clock   time set! ");
        break;
      case 5:
        lcd.setCursor(0, 1);
         lcd.print("Alarm   man/auto");
        break;
      case 6:
         lcd.setCursor(0, 1);
        lcd.print("Back to the menu");
        break;
     }
    if ((lapTime) > (6 * laps)) { //back into the same routine
      started   = moment;
    }
  }
  key = state;// keySelection   ALARM_ON_OFF,ALARM_SEASON,ALARM_SET,CLOCK_SET,AlARM_AUTO,RUN_CLOCK,
}

////function   setTimerOnOff////switching On Off status as a flip flop
void setTimerOnOff()   {
  boolean setAlarmOnOff = alarmOnOff;
  lcd.setCursor(0, 0);
  lcd.print("switch   it now   ");
  lcd.setCursor(0, 1);
  lcd.print("Timer is:  ");
  lcd.setCursor(11,   1);
  if (alarmOnOff == false) lcd.print("[off] "); else lcd.print("[on]  ");
   while (!selectButton.getSingleDebouncedPress ()) {
    if (upButton.getSingleDebouncedPress()   || downButton.getSingleDebouncedPress())setAlarmOnOff = !setAlarmOnOff;
    lcd.setCursor(11,   1);
    if (setAlarmOnOff == false) lcd.print("[off] "); else lcd.print("[on]   ");
    delay(30);
  }
  if (alarmOnOff != setAlarmOnOff) {
    EEPROM.write(3,   setAlarmOnOff);
    delay(15);
  }
}

////function setTimerPlus////expedites   the moment of starting the alarm //deze functie functioneert nog niet
void setTimerPlus()   {
  readAlarm();
  int setAlarmPlus = alarmPlus; // tijdelijke variable om   opslaan eeprom te laten werken!!!!!!!!!!!!!!!!!!
  lcd.setCursor(0, 0);
  lcd.print("stop   time: 00:00");
  if (alarmHourStop < 10) lcd.setCursor(12, 0); else lcd.setCursor(11,   0);
  lcd.print(alarmHourStop);
  if (alarmMinuteStop < 10) lcd.setCursor(15,   0); else lcd.setCursor(14, 0);
  lcd.print(alarmMinuteStop);
  lcd.setCursor(0,   1);
  lcd.print("hours  on:      ");
  lcd.setCursor(11, 1);
  lcd.print(setAlarmPlus);

   while (!selectButton.getSingleDebouncedPress ()) {
    lcd.setCursor(11, 1);
     if (alarmHourStop > 6) {  // the stopping hour has to be set later then 6 am
       if (upButton.isPressed() && (setAlarmPlus < 6)) setAlarmPlus++; //The maximum   time for the lights to be on is six hours
      if (downButton.isPressed() &&   (setAlarmPlus > 1)) setAlarmPlus--; // The minimum time for the hours to be on is   one hour
      lcd.print(setAlarmPlus);
    }
    else {
      lcd.setCursor(0,   0);
      lcd.print("Set alarm stop  "); // in case the alarm is set too early   in the day. Minimum after 6 am
      lcd.setCursor(0, 1);
      lcd.print("timer   first !!!!");
    }
    delay(250);
  }
  if (alarmPlus != setAlarmPlus)   {
    EEPROM.write(2, setAlarmPlus);
    delay(15);
  }
}

////function   presetClockDate////setting year, month, day, according to the input
void presetClockDate()   {
  int  loops = 20;
  boolean pBlink = false;
  const int loopMax = 25;   //Moment of refreshing the LCD
  int loopCheck = 0;  //to controll if if there   is any interaction
  enum YMD {YEAR = 1, MONTH, DAY};

  enum YMD YrMoDy   = YEAR;
  setYr = (yr - 2000); setMo = mo; setDy = dy; setHr = hr; setMn = mn;
   lcd.setCursor(0, 0);
  lcd.print("Set Year // Date");

  while (loopCheck   < loopCheckMax) {  //fast running part of the loop
    delay(10);
    loops++;
     if (YrMoDy == YEAR) {
      setYear();
      loopCheck = 0;
    }
     if (YrMoDy == MONTH) {
      setMonth();
      loopCheck = 0;
    }
     if (YrMoDy == DAY) {
      setDay();
      loopCheck = 0;
    }
     if (YrMoDy > DAY)  loopCheck = loopCheckMax;
    if (nextButton.getSingleDebouncedPress())   YrMoDy = YrMoDy + 1;
    if (loops > loopMax) {  // displaying whit a long interval
       lcd.setCursor(0, 1);
      lcd.print("Y-00 Mo-00 Dy-00");
      if   (setYr < 10) {
        lcd.setCursor(2, 1);
        lcd.print("00");
         lcd.setCursor(3, 1);
      } else lcd.setCursor(2, 1);
      lcd.print(setYr);
       if (setMo < 10) {
        lcd.setCursor(8, 1);
        lcd.print("00");
         lcd.setCursor(9, 1);
      }
      else lcd.setCursor(8, 1);
      lcd.print(setMo);
       if (setDy < 10) {
        lcd.setCursor(14, 1);
        lcd.print("00");
         lcd.setCursor(15, 1);
      }
      else lcd.setCursor(14, 1);
       lcd.print(setDy);


      if (YrMoDy == YEAR)lcd.setCursor(2, 1);
       if (YrMoDy == MONTH)lcd.setCursor(8, 1);
      if (YrMoDy == DAY)lcd.setCursor(14,   1);
      if (pBlink)lcd.print("  ");

      pBlink = !pBlink;
      loops   = 0;
      loopCheck++;
    }
    if (selectButton.getSingleDebouncedPress   ()) { //breaks the loop by the select button or by time lapsed
      loopCheck   = loopCheckMax;
    }
    delay(10);
  }
  lcd.setCursor(0, 0);
   lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

////function   presetClockTime////setting hour and minute according to the input
void presetClockTime(int   StartingPointHour) {
  int  loops = 20;
  boolean pBlink = false;
  const   int loopMax = 25;  //Moment of refreshing the LCD
  int loopCheck = 0;  //to   controll if if there is any interaction
  //  setHr = hr; setMn = mn;
  enum   HM {HOUR = 1, MINUTE};
  enum HM HrMn = HOUR;

  while (loopCheck < loopCheckMax)   {  //fast running part of the loop
    delay(10);
    loops++;
    if (HrMn   == HOUR) {  //Setting hour function
      setHr = hourReturn(setHr, StartingPointHour);
       delay(10);
      loopCheck = 0;
    }
    if (HrMn == MINUTE) { //Setting   minute function
      setMn = minuteReturn(setMn);
      delay(10);
      loopCheck   = 0;
    }
    if (HrMn > MINUTE)loopCheck = loopCheckMax;
    if (nextButton.getSingleDebouncedPress())   HrMn = HrMn + 1;
    if (loops > loopMax) {  // displaying whit a long interval
       lcd.setCursor(0, 1);
      lcd.print("hour:00  minu:00");
      if   (setHr < 10) {
        lcd.setCursor(5, 1);
        lcd.print("00");
         lcd.setCursor(6, 1);
      } else lcd.setCursor(5, 1);
      lcd.print(setHr);
       if (setMn < 10) {
        lcd.setCursor(14, 1);
        lcd.print("00");
         lcd.setCursor(15, 1);
      }
      else lcd.setCursor(14, 1);
       lcd.print(setMn);
      if (HrMn == HOUR)lcd.setCursor(5, 1);
      if   (HrMn == MINUTE)lcd.setCursor(14, 1);
      if (pBlink)lcd.print("  ");

       pBlink = !pBlink;
      loops = 0;
      loopCheck++;
    }
    if   (selectButton.getSingleDebouncedPress ()) { //breaks the loop by the select button   or by time lapsed
      loopCheck = loopCheckMax;
    }
    delay(10);
   }
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0,   1);
  lcd.print("                ");
}

////function lastDayOfTheMonth////days   per month Leap Year included
int lastDayOfTheMonth(int setMonth) {
  switch   (setMonth) {
    case 1: //January
      return 31;
      break;
    case   2: //February
      if ((setYr % 4) == 0) return 29; else return 28;
      break;
     case 3: //March
      return 31;
      break;
    case 4: //April
       return 30;
      break;
    case 5: //May
      return 31;
      break;
     case 6: //June
      return 30;
      break;
    case 7: //Juli
       return 31;
      break;
    case 8: //August
      return 31;
       break;
    case 9: //September
      return 30;
      break;
     case 10: //October
      return 31;
      break;
    case 11: //November
       return 30;
      break;
    case 12: //December
      return 31;
       break;
  }
}

////function setYear//
void setYear() {
  int   teller = 0;
  if (setYr < 18) setYr = 17; //to set the minimum year
  while   (upButton.isPressed()) {
    if (setYr < 99) setYr++;
    lcd.setCursor(2,   1);
    lcd.print(setYr);
    teller++;
    if (teller > 7) delay(100);   else delay(250);
  }
  while (downButton.isPressed()) {
    if (setYr >   17) setYr--;
    lcd.setCursor(2, 1);
    lcd.print(setYr);
    teller++;
     if (teller > 7) delay(100); else delay(250);
  }
}

////function   //setMonth
void setMonth() {
  while (upButton.isPressed()) {
    if (setMo   < 12) setMo++;
    lcd.setCursor(8, 1);
    if (setMo < 10) {
      lcd.setCursor(8,   1);
      lcd.print("00");
      lcd.setCursor(9, 1);
    }
    lcd.print(setMo);
     delay(250);
  }
  while (downButton.isPressed()) {
    if (setMo >   1) setMo--;
    lcd.setCursor(8, 1);
    if (setMo < 10) {
      lcd.setCursor(8,   1);
      lcd.print("00");
      lcd.setCursor(9, 1);
    }
    lcd.print(setMo);
     delay(250);
  }
}

////function //setDay
void setDay() {
   int teller = 0;
  int maxD = maxDay();
  while (upButton.isPressed()) {
     if (setDy < maxD) setDy++;
    lcd.setCursor(14, 1);
    if (setDy < 10)   {
      lcd.setCursor(14, 1);
      lcd.print("00");
      lcd.setCursor(15,   1);
    }
    lcd.print(setDy);
    teller++;
    if (teller > 7) delay(100);   else delay(250);
  }

  while (downButton.isPressed()) {
    if (setDy   > 1) setDy--;
    lcd.setCursor(14, 1);
    if (setDy < 10) {
      lcd.setCursor(14,   1);
      lcd.print("00");
      lcd.setCursor(15, 1);
    }
    lcd.print(setDy);
     teller++;
    if (teller > 7) delay(100); else delay(250);
  }
}

////function   //last day of the month
int maxDay() {
  switch (setMo) {
    case 1:
       return 31;
    case 2:
      if ((setYr % 4) == 0)return 29; else return   28;
    case 3:
      return 31;
    case 4:
      return 30;
    case   5:
      return 31;
    case 6:
      return 30;
    case 7:
      return   31;
    case 8:
      return 31;
    case 9:
      return 30;
    case   10:
      return 31;
    case 11:
      return 30;
    case 12:
       return 31;
  }
}

////function hourReturn
int hourReturn(int   setHour, int startingPoint) {
  if (setHour < startingPoint) setHour = startingPoint;   // to set the minimum hour according to set clock or set alarm
  while (upButton.isPressed())   {
    if (setHour < 24) setHour++;
    lcd.setCursor(5, 1);
    if (setHour   < 10) {
      lcd.setCursor(5, 1);
      lcd.print("00");
      lcd.setCursor(6,   1);
    }
    lcd.print(setHour);
    delay(200);
  }
  while (downButton.isPressed())   {
    if (setHour > startingPoint) setHour--;
    lcd.setCursor(5, 1);
     if (setHour < 10) {
      lcd.setCursor(5, 1);
      lcd.print("00");
       lcd.setCursor(6, 1);
    }
    lcd.print(setHour);
    delay(200);
   }
  return setHour;
}

////function minute Return
int minuteReturn(int   setMinute) {
  while (upButton.isPressed()) {
    if (setMinute < 60) setMinute++;
     lcd.setCursor(14, 1);
    if (setMinute < 10) {
      lcd.setCursor(14,   1);
      lcd.print("00");
      lcd.setCursor(15, 1);
    }
    lcd.print(setMinute);
     delay(200);
  }
  while (downButton.isPressed()) {
    if (setMinute   > 1) setMinute--;
    lcd.setCursor(14, 1);
    if (setMinute < 10) {
       lcd.setCursor(14, 1);
      lcd.print("00");
      lcd.setCursor(15,   1);
    }
    lcd.print(setMinute);
    delay(200);
  }
  return   setMinute;
}

////function save clock // saves the clock settings into   the 1307 clock module
void saveClock() {
  boolean saveNew = true;
  lcd.setCursor(0,   1);
  lcd.print("00:00 00/00/2000");  //setHr is still two digets
  if (setHr   < 10) lcd.setCursor(1, 1); else lcd.setCursor(0, 1);
  lcd.print(setHr);
   if (setMn < 10) lcd.setCursor(4, 1); else lcd.setCursor(3, 1);
  lcd.print(setMn);
   if (setDy < 10) lcd.setCursor(7, 1); else lcd.setCursor(6, 1);
  lcd.print(setDy);
   if (setMo < 10) lcd.setCursor(10, 1); else lcd.setCursor(9, 1);
  lcd.print(setMo);
   lcd.setCursor(14, 1);
  lcd.print(setYr);
  lcd.setCursor(0, 0);
  lcd.print("save   clock?: yes");
  while (!selectButton.getSingleDebouncedPress ()) {
    lcd.setCursor(13,   0);
    if (saveNew == false) lcd.print("no "); else lcd.print("yes");
     if ((upButton.getSingleDebouncedPress() == 1) || (downButton.getSingleDebouncedPress()   == 1))saveNew = !saveNew;;
    delay(80);
  }
  if (saveNew = true) {
     int setSec = 0;
    int setWkDy = 1;
    delay(100);
    // The following   codes transmits the data to the RTC
    Wire.beginTransmission(DS1307);
    Wire.write(byte(0));
     Wire.write(decToBcd(setSec));
    Wire.write(decToBcd(setMn));
    Wire.write(decToBcd(setHr));
     Wire.write(decToBcd(setWkDy)); //not in use
    Wire.write(decToBcd(setDy));
     Wire.write(decToBcd(setMo));
    Wire.write(decToBcd(setYr));  //has to be   two digits
    Wire.write(byte(0));
    Wire.endTransmission();
    //   Ends transmission of data
  }
  delay(100);
}

////function decToBcd   // in use for saving the clock settings
byte decToBcd(byte val) {
  return   ((val / 10 * 16) + (val % 10));
}

////function save alarm // saves the   alarm settings into Eeprom
void saveAlarmSet() {
  boolean saveNew = true;
   lcd.setCursor(0, 1);
  lcd.print("hour:00  minu:00");
  if (setHr < 10)   {
    lcd.setCursor(5, 1);
    lcd.print("00");
    lcd.setCursor(6,   1);
  } else lcd.setCursor(5, 1);
  lcd.print(setHr);
  if (setMn < 10)   {
    lcd.setCursor(14, 1);
    lcd.print("00");
    lcd.setCursor(15,   1);
  }
  else lcd.setCursor(14, 1);
  lcd.print(setMn);
  lcd.setCursor(0,   0);
  lcd.print("save Alarm?: yes");
  while (!selectButton.getSingleDebouncedPress   ()) {
    lcd.setCursor(13, 0);
    if (saveNew == false) lcd.print("no ");   else lcd.print("yes");
    if ((upButton.getSingleDebouncedPress() == 1) ||   (downButton.getSingleDebouncedPress() == 1))saveNew = !saveNew;; // dit nog styleren!!!!!!!!!!!!!!!!!!!!!!!!!!!
     delay(80);
  }
  if (saveNew = true) {
    EEPROM.write(0, setHr);
     delay(15);
    EEPROM.write(1, setMn);
    delay(15);
  }
  delay(100);
}

////function   AlarmAutoManual// setting the alarm on automatic or manual
void alarmAutoManual()   {
  boolean autoManualNew = autoManual; //using this variable makes the display   less restless
  lcd.setCursor(0, 1);
  lcd.print("Lights off 00:00");
   if (alarmHourStop < 10) {
    lcd.setCursor(11, 1);
    lcd.print("00");
     lcd.setCursor(12, 1);
  } else lcd.setCursor(11, 1);
  lcd.print(alarmHourStop);
   if (alarmMinuteStop < 10) {
    lcd.setCursor(14, 1);
    lcd.print("00");
     lcd.setCursor(15, 1);
  }
  else lcd.setCursor(14, 1);
  lcd.print(alarmMinuteStop);
   lcd.setCursor(0, 0);
  if (autoManualNew)lcd.print("Timer: automatic"); else   lcd.print("Timer: manual   ");

  while (!selectButton.getSingleDebouncedPress   ()) {
    if (upButton.getSingleDebouncedPress() || downButton.getSingleDebouncedPress())   {
      autoManualNew = !autoManualNew;
      lcd.setCursor(0, 0);
      if   (autoManualNew)lcd.print("Timer: automatic"); else lcd.print("Timer: manual   ");
     }
    delay(80);
  }

  if (autoManualNew != autoManual) {
    EEPROM.write(4,   autoManualNew);
    delay(20);
  }
}

//function automatic Timer   // walks automaticaly trough the timer settings during the year
void automaticTimer()   {
  switch (mo) {
    case 1:
      alarmOffSet = 6;
      break;
     case 2:
      alarmOffSet = 5;
      break;
    case 3:
      alarmOffSet   = 5;
      break;
    case 4:
      alarmOffSet = 4;
      break;
     case 5:
      alarmOffSet = 3;
      break;
    case 6:
      alarmOffSet   = 2;
      break;
    case 7:
      alarmOffSet = 2;
      break;
     case 8:
      alarmOffSet = 3;
      break;
    case 9:
      alarmOffSet   = 4;
      break;
    case 10:
      alarmOffSet = 5;
      break;
     case 11:
      alarmOffSet = 6;
      break;
    case 12:
      alarmOffSet   = 6;
      break;
  }
}
// function dayLightSaving  //Daylight saving   periodes for The Netherlands
void dayLightSaving() {
  boolean daylightSavingNew   = false;

  switch (yr) {
    case 2017:
      if ((mo == 3 && dy >=   26) || (mo > 3 && mo < 10) || (mo == 10 && dy < 29)) daylightSavingNew = true;
       break;
    case 2018:
      if ((mo == 3 && dy >= 25) || (mo > 3 &&   mo < 10) || (mo == 10 && dy < 28)) daylightSavingNew = true;
      break;
     case 2019:
      if ((mo == 3 && dy >= 31) || (mo > 3 && mo < 10) || (mo   == 10 && dy < 27)) daylightSavingNew = true;
      break;
    case 2020:
       if ((mo == 3 && dy >= 29) || (mo > 3 && mo < 10) || (mo == 10 && dy < 25))   daylightSavingNew = true;
      break;
    case 2021:
      if ((mo ==   3 && dy >= 28) || (mo > 3 && mo < 10) || (mo == 10 && dy < 31)) daylightSavingNew   = true;
      break;
    case 2022:
      if ((mo == 3 && dy >= 27) ||   (mo > 3 && mo < 10) || (mo == 10 && dy < 30)) daylightSavingNew = true;
      break;
     case 2023:
      if ((mo == 3 && dy >= 26) || (mo > 3 && mo < 10) || (mo   == 10 && dy < 29)) daylightSavingNew = true;
      break;
    case 2024:
       if ((mo == 3 && dy >= 31) || (mo > 3 && mo < 10) || (mo == 10 && dy < 27))   daylightSavingNew = true;
      break;
    case 2025:
      if ((mo ==   3 && dy >= 30) || (mo > 3 && mo < 10) || (mo == 10 && dy < 26)) daylightSavingNew   = true;
      break;
    case 2026:
      if ((mo == 3 && dy >= 29) ||   (mo > 3 && mo < 10) || (mo == 10 && dy < 25)) daylightSavingNew = true;
      break;
     case 2027:
      if ((mo == 3 && dy >= 28) || (mo > 3 && mo < 10) || (mo   == 10 && dy < 31)) daylightSavingNew = true;
      break;
    case 2028:
       if ((mo == 3 && dy >= 26) || (mo > 3 && mo < 10) || (mo == 10 && dy < 29))   daylightSavingNew = true;
      break;
    case 2029:
      if ((mo ==   3 && dy >= 25) || (mo > 3 && mo < 10) || (mo == 10 && dy < 28)) daylightSavingNew   = true;
      break;
  }
  if (daylightSavingNew != daylightSaving) {
     if (hr > 2) {
      if (daylightSavingNew) hr = hr + 1;
      if (!daylightSavingNew)   hr = hr - 1;
      daylightSaving = daylightSavingNew;
    }
  }
}
