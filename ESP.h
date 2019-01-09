//code by Naomi Sagan
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <Arduino.h>
#include "Constants.h"

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
  
  int authDataLen();
  int streamsDataLen(float, float, float, int, int,  int, int, int, int, time_t);
  int streamsDataL;
  int numChar(int);
  int numChar(long);
  int streamsReqLen(int);

  void printHashedSecret();
  void serialPrintHashedSecret();
  byte byteTo64(byte);
  
  const long BAUD[5] = {115200, 19200, 38400, 57600, 74880};
};

#endif
