#include "Dust.h"

#define DELAY1 280
#define DELAY2 40
#define OFF 9680

#define TIMES 20
#define OUT_TO_V 5.0/1023

Dust::Dust(int pin, int ledPow){
  sum = 0;
  this->pin = pin;
  this->ledPow = ledPow;
}
void Dust::init(){
  pinMode(pin,INPUT);
  pinMode(ledPow, OUTPUT);
}
unsigned int Dust::getData(){
  sum = 0;
  count = 0;
  while(count<TIMES){
    digitalWrite(ledPow,LOW);
    delayMicroseconds(DELAY1);
    sum += (((analogRead(pin)*OUT_TO_V)-.1)*500/3.6);
    delayMicroseconds(DELAY2);
    digitalWrite(ledPow,HIGH);
    delay(62.5);
    count++;
  }
  return sum/count;
}

