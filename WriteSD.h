//code by Naomi Sagan
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>

#ifndef WriteSD_h
#define WriteSD_h

class WriteSD
{
  public:
    void init(int, int, int);
    void writeData(float, float, float, int, int, int, int, int,  int, int, int, int, int, int, int);
    void setUpFile(int, int, int);
  private:
    int timeout = 0;
    bool commandMode();
};

#endif
