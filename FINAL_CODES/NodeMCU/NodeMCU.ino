//NodeMCU.ino

#define BLYNK_TEMPLATE_ID "TMPL6l5EWs1EA"
#define BLYNK_TEMPLATE_NAME "Blynk Test"
#define BLYNK_AUTH_TOKEN "vuI5BDd8l__w_Da30LphlynrrnPtO7uT"

#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SMUFree";//Enter your WIFI name
char pass[] = "";//Enter your WIFI password

BLYNK_WRITE(V0) {
  digitalWrite(D0, param.asInt());
  
  int btnValue = param.asInt();
  
  if(btnValue == 1)
    Serial.println("on");

   if(btnValue == 0)
    Serial.println("off");
}

void setup() {
  //Set the LED pin as an output pin
  pinMode(D0, OUTPUT);
  //Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  Serial.begin(9600);
}

void loop() {
  //Run the Blynk library
  Blynk.run();
}
