/*==============================================
test code for [Arduino -> Raspberry Pi] UART
sends 'rasp' value to operate Pi's function
==============================================*/

int P1 = 6;
int P2 = 7;
int P3 = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600)
}

void loop() {
  int rasp = 0;
  // put your main code here, to run repeatedly:
  if (digitalRead(P1) == LOW) {
        rasp = 1;
    }
  if (digitalRead(P2) == LOW) {
        rasp = 2;
    }
  if (digitalRead(P3) == LOW) {
        rasp = 3;
    }
  Serial.println(rasp);
  
  int delay_time = 500;
  while(delay_time < 500){  
    //Nothing.. just wait
    delay_time++;
  }
}
