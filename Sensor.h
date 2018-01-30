#include <Arduino.h>

#ifndef Sensor_h
#define Sensor_h

class Sensor
{
public:
  virtual unsigned int getData() = 0;
  virtual void init() = 0;
};

#endif
