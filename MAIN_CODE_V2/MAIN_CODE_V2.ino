#include "InOut.h"
#include "Clock.h"
#include "RFID.h"

void setup() {
  InOutsetup();
  Clocksetup();
  RFIDsetup();
  Serial.begin(9600);
}

void loop() {
  
  clockloop();

  while (Alarm()) {    
      if (inout() == 1)           // Didn't return home in time
        Serial.println("out");
  }
  
  if (inout() == 2) {         // got home
    Serial.println("in");

    // 제한시간(5분) 이내 rfid 태그 코드

  }
    
}
