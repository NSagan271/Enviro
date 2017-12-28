#include "SpecSensor.h"
#define TIMES 80
#define OUT_TO_V 5.03/1023

SpecSensor::SpecSensor(int pin){
  gasSum = 0;
  this->pin = pin;
  pinMode(pin,INPUT);
}
double SpecSensor::getGas(){
  gasSumCount = 0;
  gasSum = 0;
  for (int i = 0; i < TIMES; i++){
    next = (analogRead(pin))*OUT_TO_V; 
    if (next>0) gasSum+=next;
    gasSumCount++;
    delay(62.5);
  }
  return gasSum/gasSumCount;
}
