
#include "InOut.h"
#include "Clock.h"
#include "RFID.h"

unsigned long previous_time = 0;
const long delay_time = 1000*60*5;   // 5 min


void setup() {
  InOutsetup();
  Clocksetup();
  RFIDsetup();
  Serial.begin(9600);
}

void loop() {

  unsigned long current_time = millis();
  int uart = 0;
  clockloop();

  while (Alarm()) {    
    if (inout() == 1)           // Didn't return home in time
      Serial.println("out");
      uart = 1;
  }
  
  if (inout() == 2) {         // got home
    Serial.println("in");

    // 제한시간(5분) 이내 rfid 태그 코드
    while(current_time - previous_time >= delay_time) {
      previous_time = current_time;
      
      if (rfidCheck() == 1) {     // Authorized access
        uart = 3;
        break;
      }
      if (rfidCheck() == 2) {     // Access denied
        uart = 2;
        break;
      }
      else {                      // no access in 5min
        uart = 2;
      }
    }
  }
  if (uart != 0) Serial.println(uart);
  
  delay(100);
}
