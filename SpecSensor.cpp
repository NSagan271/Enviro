#include "SpecSensor.h"

#define SERIALNUM_LEN 12
#define S1 2
#define S2 3

SoftwareSerial sensor(4,5);
SpecSensor::SpecSensor(int value1, int value2){
  this->value1 = value1;
  this->value2 = value2; 
}
void SpecSensor::init(){
  sensor.begin(9600);
}
unsigned int SpecSensor::getData(){
  pinMode(S1, OUTPUT);
  pinMode (S2, OUTPUT);
  
  digitalWrite(S1, value1);
  digitalWrite(S2, value2);

  sensor.listen();
  
  len = 0;
  int timeout = 0;
  while(sensor.available() < SERIALNUM_LEN){
    sensor.write('\r');
    timeout++;
    if (timeout >20) return 0;
    //Serial.println("Write command"); 
    delay(1000);
  }

  while(sensor.available()){
    out[len] = sensor.read();
    Serial.write(out[len]);
    if (len < 24)len++;
  }
  Serial.println();
  result = 0;
  //Serial.println("Read data");
  start = (SERIALNUM_LEN + 2);
  while (start < len && out[start] != ','){
    if (out[start] == '-')return 0; //you can't have negative PPB!
    result = result*10 + ((int)(out[start]) - '0');
    start++;
  }
  //Serial.println(result);
  return result;
  return 0;
}
