#include <Wire.h>
#include <SeeedOLED.h>

#ifndef OLED_h
#define OLED_h

class OLED
{
public:
  void wait(); 
  void updateValues(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
  void init();
};

#endif
