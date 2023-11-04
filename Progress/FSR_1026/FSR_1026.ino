#define TRIG 9 // 초음파 보내는 핀
#define ECHO 8 // 초음파 받는 핀

int FSR0 = A0;
int FSR1 = A1;
int FSR2 = A2;


void setup() {
  pinMode(FSR0, INPUT);
  pinMode(FSR1, INPUT);
  pinMode(FSR2, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
}

void loop() {
  //-------초음파센서 설정----------//
  long duration, distance;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn (ECHO, HIGH); // 물체에 반사되어 돌아온 초음파 시간
  distance = duration*17/1000; // 초음파센서 거리값을 cm로 환산

  //------압력센서 설정-----------//
  int sensorval_0 = analogRead(A0);
  int sensorval_1 = analogRead(A1);
  int sensorval_2 = analogRead(A2);


  //---------출입-----------//
  
  int door = 0 ;
  if (distance < 100) {
    delay(500);
    
    if (sensorval_0 > 100 || sensorval_1 > 100 || sensorval_2 > 100) {
      door = 1;
    }
    if (sensorval_0 < 100 && sensorval_1 < 100 && sensorval_2 < 100) {
      door = 2;
    }
  }


  Serial.println("FSR value : " + String(sensorval_0)
              + " | " + String(sensorval_1)
              + " | " + String(sensorval_2)
              + "\nDistance : " + String(distance)
              + " cm" + "\n[Door] : "+ String(door));
  
  delay(500);
}
