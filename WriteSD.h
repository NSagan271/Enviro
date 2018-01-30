#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>

#ifndef WriteSD_h
#define WriteSD_h

class WriteSD
{
  public:
    void init();
    void writeData(float, float, float, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, time_t);
    void setUpFile(char[12]);
  private:
    int timeout = 0;
};

#endif
