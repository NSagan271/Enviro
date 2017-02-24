#include <Arduino.h>
class SpecSensor
{
public:
  double getGas();
  void updateData();
  SpecSensor(int, double, double);
private:
  int pin;
  double m;
  double ref;
  double gasSum;
  long gasSumCount;
};

