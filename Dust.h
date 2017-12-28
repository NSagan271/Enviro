#include "Sensor.h"

#ifndef Dust_h
#define Dust_h

class Dust: public Sensor
{
public:
  double getGas();
  Dust(int,int);
private:
  int pin;
  int ledPow;
  double sum;
  int count;
};

#endif
