#include "CO2.h"
#define RX 2
#define TX 3
#define SIZE 9
#define IND 2
#define MAXTIME 30

SoftwareSerial sensor(RX,TX);
void CO2::init(){
  sensor.begin(9600);
}
double CO2::getGas(){
   cmd_timeout = 0;
   
   while(sensor.available() == 0){
      sensor.write(command, SIZE);
      cmd_timeout++;
      if (cmd_timeout > MAXTIME) return -203;
      delay(130);
   }
   
   timeout = 0;
   while (sensor.available() < SIZE){
      timeout++;
      if (timeout > 40) break;
      delay(5);
   }
   if (sensor.available() == SIZE){
      for (int i = 0; i < SIZE; i ++){
        packet[i] = sensor.read();
      }
      return 256*(packet[IND]) + packet[IND+1];
   }
   int val = -300;
   while(sensor.available()>0){
      val++;
      sensor.read();
   }
   return val;
   
}
