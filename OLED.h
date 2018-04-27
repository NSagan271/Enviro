//code by Naomi Sagan
#include <Wire.h>
#include <SeeedOLED.h>

#ifndef OLED_h
#define OLED_h

class OLED
{
public:
  void wait(); 
  void longwait(); 
  void updateValues(int, int, int, int, int, int, int, int, int);
  void init();
};

#endif
