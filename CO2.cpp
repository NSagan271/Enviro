#include "CO2.h"
#define SIZE 9
#define INDEX 2
#define MAXTIME 30

CO2::CO2(int rx,int tx):sensor(rx,tx){} 
void CO2::init(){
  sensor.begin(9600);
}
unsigned int CO2::getData(){
   cmd_timeout = 0;
   sensor.listen();
   
   while(sensor.available() == 0){
      sensor.write(command, SIZE);
      cmd_timeout++;
      if (cmd_timeout > MAXTIME) return 0;
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
      return 256*(packet[INDEX]) + packet[INDEX+1];
   }
   while(sensor.available()>0){
      sensor.read();
   }
   return 0;
   
}
