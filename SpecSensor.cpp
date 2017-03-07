#include "SpecSensor.h"
#define TIMES 80
#define OUT_TO_V 5.03/1023


SpecSensor::SpecSensor(int pin, double m, double ref){
  gasSum = 0;
  this->pin = pin;
  this->m = m;
  this->ref = ref;
  pinMode(pin,INPUT);
}
double next;
void SpecSensor::updateData(){
  next = (analogRead(pin))*OUT_TO_V; 
  if (next>0) gasSum+=next;
  gasSumCount++;
}
double lev;
double SpecSensor::getGas(){
  lev = gasSum/gasSumCount;
  gasSumCount = 0;
  gasSum = 0;
  return lev;
}

