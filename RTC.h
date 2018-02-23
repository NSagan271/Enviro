#include <Wire.h>

#ifndef RTC_h
#define RTC_h

#define byte uint8_t

class RTC
{
public:
  unsigned long getTimestamp();
  void init();
  bool setTime();
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
};


#endif

