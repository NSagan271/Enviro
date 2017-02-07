#include <Arduino.h>
class SpecSensor: public Sensor
{
public:
  float gasLevel;
  int pin;
  float M;
  void initSensor(int p, float m);
  virtual void updateData();
private:
  float gasValues[];
  float getAvgGasValue();
};

