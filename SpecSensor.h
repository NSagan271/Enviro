//code by Naomi Sagan
#include "Sensor.h"
#include <SoftwareSerial.h>

#ifndef CO_h
#define CO_h

class SpecSensor:public Sensor
{
public:
  SpecSensor(int, int);
  int getData();
  void init();
private:
  byte out[25];
  int len;
  long result;
  int temp;
  int start;
  int value1;
  int value2;
};

#endif
