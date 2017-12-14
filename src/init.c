#include "main.h"
void initializeIO() {
}
void initialize() {
  lcdInit(uart1);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}
