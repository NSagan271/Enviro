#include "Barometer.h"

void Barometer::init(){
  barometer.begin();
}

float Barometer::getData(){
  return barometer.readPressure()/100;
}

