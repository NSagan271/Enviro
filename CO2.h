#include "Sensor.h"
#include <SoftwareSerial.h>

#ifndef CO2_h
#define CO2_h

class CO2: public Sensor
{
public:
  CO2(int, int);
  unsigned int getData();
  void init();
private:
  byte command[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};;
  byte packet[9];
  int cmd_timeout;
  int timeout;
  SoftwareSerial sensor;
};

#endif
