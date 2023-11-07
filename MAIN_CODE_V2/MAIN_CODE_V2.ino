
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
  if (inout() == 1)
    Serial.println("out");
  if (inout() == 2)
    Serial.println("in");

}
