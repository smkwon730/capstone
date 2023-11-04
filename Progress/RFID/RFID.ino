#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9     // reset핀 설정
#define SS_PIN    10    // 데이터를 주고받는 역할의 핀( SS = Slave Selector )

MFRC522 mfrc522(SS_PIN, RST_PIN);           // 이 코드에서 MFR522를 이용하기 위해 mfrc객체를 생성해 줍니다.

int LED = 4;                            // LED를 4번핀에 연결합니다.


void setup() {
  Serial.begin(9600);                     // 시리얼 통신, 속도는 9600
  SPI.begin();                             // SPI 초기화(SPI : 하나의 마스터와 다수의 SLAVE(종속적인 역활)간의 통신 방식)
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED, OUTPUT);                 // 4번핀을 출력으로 설정

}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {    //  태그 접촉이 되지 않았을때 또는 아이디가 읽혀지지 않았을때
    delay(500);        
    return;                                   // return
  }

  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : "); 
  content.toUpperCase(); 
  

    if (content.substring(1) == "D1 2A D4 1C") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    digitalWrite(LED, HIGH);                // 3번핀 에 연결된 led 켜지기         
    delay(1000);
    digitalWrite(LED, LOW); 
    delay(1000);
    
  }else {                                   // 다른 태그 ID일 경우
    digitalWrite(LED, LOW);               // 4번핀 에 연결된 led 꺼지기
    Serial.println("Access denied");        // 시리얼 모니터에 "Who are you?" 출력 

    delay(500);
  }  
}
