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
  void getTime();
  void postData(float, float, float, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, time_t);
private:
  void setBaudRate();
  bool waitForOK(int);
  void waitForClosed(int);
  byte out[255];
  int len;
  int count;
  int dataLen(float, float, float, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, time_t);
  int dataL;
  int numChar(int);
  int reqLen(int);
};

#endif
