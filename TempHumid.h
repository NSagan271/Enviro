// Code by Brent Luker
#include <Arduino.h>
#include <Wire.h>

#ifndef TempHumid_h
#define TempHumid_h

class TempHumid
{
public:
  uint8_t status = 0;
  //Temperature is in degrees C. Humidity is a %
  void updateData();
  float humidity, temp;
  void init();
};

#endif
