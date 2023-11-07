
#ifndef InOut_h
#define InOut_h

#define TRIG 9 // 초음파 보내는 핀
#define ECHO 8 // 초음파 받는 핀

const int FSR0 = A0;
const int FSR1 = A1;
const int FSR2 = A2;
const int FSR3 = A3;

void InOutsetup();
int inout(void);

#endif