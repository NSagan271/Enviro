//code by Naomi Sagan
#include <Wire.h>
#include "Constants.h"

#ifndef RTC_h
#define RTC_h

#define byte uint8_t

class RTC
{
public:
  unsigned long getTimestamp();
  void init();
  bool setTime();
  int getHour();
  int getMinute();
  int getSecond();

  int getMonth();
  int getDay();
  int getYear();
private:
  int da;
  int yr;
  short mo;
  short hr;
  int m;
  int s;
  unsigned long temp;
  unsigned long temp2;
  char mon[4];
  int ii;
  byte bcdToDec(byte);
  byte decToBcd(byte);

  char dateString[8];
  char timeString[8];
};


#endif

