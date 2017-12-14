#include "main.h"
void initializeIO() {
}
void initialize() {
  lcdInit(uart1);
  LeftEnc = encoderInit(1, 2, false);
  RightEnc = encoderInit(3, 4, true);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}
