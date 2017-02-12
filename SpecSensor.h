#include <Arduino.h>
class SpecSensor
{
public:
  float gasLevel;
  int pin;
  float m;
  void initSensor(int sensor);
  void updateData();
private:
  float gasValues[];
  float getAvgGasValue();
};

