// Code by Jiahao Kuang
#include "Barometer.h"

void Barometer::init(){
  barometer.begin();
}

float Barometer::getData(){
  return barometer.readPressure()/100.0;
}

