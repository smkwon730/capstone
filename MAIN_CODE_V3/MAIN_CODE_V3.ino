// MAIN_CODE_V2.ino

#include "InOut.h"
#include "Clock.h"
#include "RFID.h"

//unsigned long previous_time = 0;
//const long delay_time = 1000*60*5;   // 5 min
const long delay_time = 1000*10;   // 30 sec (for test)
unsigned long start_time = 0;
bool rfidCheckInitiated = false; // Declaration of rfidCheckInitiated

enum State {
  IDLE,
  OUT_OF_HOME,
  RETURNED_HOME,
  RFID_CHECK,
  RFID_AUTHORIZED,
  RFID_DENIED
};

State currentState = IDLE;
int previousUart = 0;
int uart = 0;

void setup() {
  InOutsetup();
  Clocksetup();
  RFIDsetup();
  Serial.begin(9600);
}

void loop() {
  clockLoop();
  int inoutResult = inout();

  switch (currentState) {
    case IDLE:
      if (getAlarmState() == 1) {
        if (inoutResult == 1) {
          currentState = OUT_OF_HOME;
          Serial.println("Out of home");
        } else if (inoutResult == 2) {
          currentState = RETURNED_HOME;
          Serial.println("Returned home");
          start_time = millis();
          rfidCheckInitiated = false; // Reset RFID check initiation flag
        }
      }
      break;

    case OUT_OF_HOME:
      // Handle case when out of home
      // ...
      break;

    case RETURNED_HOME:
      Serial.print("Time elapsed since return: ");
      Serial.println(millis() - start_time);
      if (millis() - start_time >= delay_time && !rfidCheckInitiated && inoutResult == 2) {
        currentState = RFID_CHECK;
        Serial.println("RFID check initiated");
        rfidCheckInitiated = true; // Set RFID check initiation flag
      } else {
        // Add this debug statement
        Serial.println("Still in returned home state");
      }
      break;

    case RFID_CHECK:
      clockLoop();
      int rfidResult = rfidCheck();

      if (rfidResult == 1) {
        currentState = RFID_AUTHORIZED;
        Serial.println("RFID authorized");
      } else if (rfidResult == 2) {
        currentState = RFID_DENIED;
        Serial.println("RFID denied");
      }

      break;

    case RFID_AUTHORIZED:
      // Handle case when RFID is authorized
      // ...

      currentState = IDLE;
      Serial.println("Back to idle");
      break;

    case RFID_DENIED:
      // Handle case when RFID is denied
      // ...

      currentState = IDLE;
      Serial.println("Back to idle");
      break;
  }
}







  /*//======= inout() test =========//
  inout();
  Serial.println(inout());
  //======= Alarm() test =========//
  while (Alarm()) {
    Serial.println("alarm: 1");
  }
  Serial.println("alarm: 0");
  //====== rfidCheck() test ======//
  unsigned long current_time = millis();

  while(current_time - previous_time >= delay_time) {
    Serial.println(current_time);
    previous_time = current_time;

    if (rfidCheck() == 1) {     // Authorized access
      Serial.println("Authorized access");
      break;
    }
    if (rfidCheck() == 2) {     // Access denied
      Serial.println("Access denied");
      break;
    }
  }
  if (rfidCheck() != 1 || rfidCheck() != 2) {                      // no access in 5 min
    if (current_time - previous_time >= delay_time) {
      Serial.println("no access in 5 min");
    }
  }*/
