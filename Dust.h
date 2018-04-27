//code by Naomi Sagan
#include "Sensor.h"

#ifndef Dust_h
#define Dust_h

class Dust: public Sensor
{
public:
  int getData();
  Dust(int,int);
  void init();
private:
  int pin;
  int ledPow;
  int sum;
  int count;
};

#endif
