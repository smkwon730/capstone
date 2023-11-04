/*#pragma once
//#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <ESP8266WIFI.h>
#include <WIFIUdp.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
	Serial.begin(9600);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	timeClient.begin();
}

void loop() {
	timeClient.update();

	Serial.print(daysOfTheWeek[timeClient.getDay()]);
	Serial.print(", ");
	Serial.print(timeClient.getHours());
	Serial.print(":");
	Serial.print(timeClient.getMinutes());
	Serial.print(":");
	Serial.println(timeClient.getSeconds());
	//Serial.println(timeClient.getFormattedTime());

	delay(1000);
}*/
