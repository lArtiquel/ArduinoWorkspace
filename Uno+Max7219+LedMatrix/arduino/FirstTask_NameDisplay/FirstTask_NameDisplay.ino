#include <LedControl.h>

int DIN = 12;
int LOAD =  10;
int CLK = 11;

int dev_count = 4;
LedControl lc = LedControl(DIN, CLK, LOAD, dev_count);

const byte P[8] = {B00000000, B00111100, B00100100, B00111100, B00100000, B00100000, B00100000, B00000000};
const byte U[8] = {B00000000, B00100100, B00100100, B00100100, B00100100, B00100100, B00111100, B00000000};
const byte S[8] = {B00000000, B00111100, B00100000, B00100000, B00111100, B00000100, B00111100, B00000000};

void setup() {
  for (byte i = 0; i < dev_count; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 15);
    lc.clearDisplay(i);
  }
}

void loop() {
    for (byte i = 0; i < dev_count; i++) {lc.clearDisplay(i);}
    delay(1000);
    printText();
    delay(2000);
}

void printText()
{
    printByteArray(P, 0);
    printByteArray(U, 1);
    printByteArray(P, 2);
    printByteArray(S, 3);
}

void printByteArray(byte character [], byte displayNum)
{
  for (byte i = 0; i < 8; i++) lc.setRow(displayNum, i, character[i]);
}
