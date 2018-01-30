#include "Sensor.h"
#include <SoftwareSerial.h>

#ifndef CO_h
#define CO_h

class SpecSensor:public Sensor
{
public:
  SpecSensor(int, int);
  unsigned int getData();
  void init();
private:
  byte out[25];
  int len;
  unsigned int result;
  int start;
  int value1;
  int value2;
};

#endif
