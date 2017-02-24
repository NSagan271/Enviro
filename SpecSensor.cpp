#include "SpecSensor.h"
#define TIMES 80
#define OUT_TO_V 5.03/1023


SpecSensor::SpecSensor(int pin, double m, double ref){
  Serial.begin(9600);
  gasSum = 0;
  this->pin = pin;
  this->m = m;
  this->ref = ref;
  pinMode(pin,INPUT);
}
void SpecSensor::updateData(){
  Serial.println(pin);
  Serial.println(analogRead(pin)*OUT_TO_V);
  Serial.println(ref);
  gasSum+= 1/m*(analogRead(pin)-ref)*OUT_TO_V;
  gasSumCount++;
}
double lev;
double SpecSensor::getGas(){
  lev = gasSum/gasSumCount;
  gasSumCount = 0;
  gasSum = 0;
  return lev;
}

