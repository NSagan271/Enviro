//code by Naomi Sagan
#include "SpecSensor.h"

#define SERIALNUM_LEN 12
#define S1 2
#define S2 3
#define TIMES 1

SoftwareSerial sensor(4,5);
SpecSensor::SpecSensor(int value1, int value2){
  this->value1 = value1;
  this->value2 = value2; 
}
void SpecSensor::init(){
  sensor.begin(9600);
}
int SpecSensor::getData(){
  pinMode(S1, OUTPUT);
  pinMode (S2, OUTPUT);
  
  digitalWrite(S1, value1);
  digitalWrite(S2, value2);
  delay(50);

  sensor.listen();
  result = 0;
  
  for (int i = 0; i < TIMES; i++){
    len = 0;
    int timeout = 0;
    while(sensor.available() < SERIALNUM_LEN){
      sensor.write('\r');
      timeout++;
      if (timeout >20) return 0;
      //Serial.println("Write command"); 
      delay(600);
    }
  
    while(sensor.available()){
      out[len] = sensor.read();
      Serial.write(out[len]);
      if (len < 24)len++; 
      delay(10);
    }
    Serial.println();
    temp = 0;
    
    start = (SERIALNUM_LEN + 2);
    while (start < len && out[start] != ','){
      if (out[start] == '-'){
        start++;
        temp = temp*10 - ((int)(out[start]) - '0');
      }
      else temp = temp*10 + ((int)(out[start]) - '0');
      start++;
    }
    result+=temp;
    while(sensor.available()){
      sensor.read();
    }
  }
  result = result/TIMES;
  if (result < -100)return 0;
  if (result > 32767) result = 32767;
  return (int)result;
}
