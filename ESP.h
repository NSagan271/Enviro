//code by Naomi Sagan
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <Arduino.h>

#ifndef ESP_h
#define ESP_h

#define byte uint8_t

class ESP
{
public:
  void init();
  void postData(float, float, float, int, int, int, int, int, int, time_t);
private:
  void setBaudRate();
  bool waitForOK(int);
  void waitForClosed(int);
  byte out[15];
  int len;
  int count;
  int dataLen(float, float, float, int, int,  int, int, int, int, time_t);
  int dataL;
  int numChar(int);
  int reqLen(int);
};

#endif
