#include "SpecSensor.h"
#define TIMES 80
#define OUT_TO_V 5.0/1023
//O3 = 0, SO2 = 1, CO = 2
const float M[4] =  {(499.0*-14.5*pow(10,-6)), (100.0*40.59*pow(10,-6)), (100.0*5.42*pow(10,-6)), (499.0*-44.78*pow(10,-6))};
const float PINS[4] = {A0,A1,A2,A3};

float gasValues[TIMES];
float gasLevel = 0;
int pin = A0;
float m = 1;
void SpecSensor::initSensor(int sensor){
  pin = PINS[sensor];
  m = M[sensor];
  pinMode(pin,INPUT);

  for (int i = 0; i < TIMES; i++){
    gasValues[i] = 0;
  }
}
void SpecSensor::updateData(){
  for (int i = 0; i < TIMES; i++){
    gasValues[i] = 1/m*analogRead(pin)*OUT_TO_V;
    delay(5000.0/TIMES);
  }
  gasLevel = getAvgGasValue();
}
float SpecSensor::getAvgGasValue(){
  float sum = 0;
  for (int i = 0; i < TIMES; i++){
    sum+=gasValues[i];
  }
  return sum/TIMES;
}

