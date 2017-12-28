#include <Arduino.h>

#ifndef Sensor_h
#define Sensor_h

class Sensor
{
public:
  virtual double getGas() = 0;
};

#endif
