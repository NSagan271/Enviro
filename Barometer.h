#include <Adafruit_BMP280.h>

// Code by Jiahao Kuang
#include <Wire.h>

#ifndef Barometer_h
#define Barometer_h

class Barometer
{
public:
  float getData();
  void init();
private:
  Adafruit_BMP280 barometer;
};


#endif

