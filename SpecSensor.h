#include "Sensor.h"

#ifndef SpecSensor_h
#define SpecSensor_h

class SpecSensor:public Sensor
{
public:
  double getGas();
  SpecSensor(int);
private:
  int pin;
  double next;
  double gasSum;
  int gasSumCount;
};

#endif
