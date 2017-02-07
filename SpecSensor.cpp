#include "SpecSensor.h"
#define TIMES 80
#define OUT_TO_V 5.0/1023

gasValues = new float[TIMES];
gasLevel = 0;
int pin;
float M;
void initSensor(int p, float m){
  pin = p;
  M = m;
  setPinMode(pin,INPUT);
}
void updateData(){
  for (int i = 0; i < TIMES; i++){
    gasValues[i] = analogRead(pin)*OUT_TO_V/M;
  }
  gasLevel = getAvgGasValue();
}
private float getAvgGasValue(){
  float sum = 0;
  for (int i = 0; i < TIMES; i++){
    sum+=gasValues[i];
  }
  return sum/TIMES;
}

