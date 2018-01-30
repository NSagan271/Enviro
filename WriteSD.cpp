#include "WriteSD.h"

#define RESET 11
SoftwareSerial logger(8,9);

void WriteSD::init(){
  pinMode(RESET, OUTPUT);
  logger.begin(9600);
  //Reset OpenLog

  logger.listen();
  digitalWrite(RESET, LOW);
  delay(100);
  digitalWrite(RESET, HIGH);
  delay(1000);

  //go into command mode
  logger.write(26);
  logger.write(26);
  logger.write(26);

  
  
  //Wait for OpenLog to respond with '>' to indicate we are in command mode
  timeout = 0;
  while(1) {
    delay(100);
    timeout++;
    if (timeout > 100) return;
    if(logger.available())
      if(logger.read() == '>') break;
  }
  
  //make directory AirQ if it doesn't already exist
  logger.print(F("md AirQ"));
  logger.write(13);
  while(1) {
    delay(100);
    if(logger.available())
      if(logger.read() == '>') break;
  }


  //go to directory AirQ
  logger.write(13);
  while(1) {
    delay(100);
    if(logger.available())
      if(logger.read() == '>') break;
  }

  
  setUpFile("test.csv");
}

void WriteSD::writeData(float temp, float humid, float baro, unsigned int co2, unsigned int co, unsigned int o3, unsigned int so2, unsigned int no2, unsigned int dust, time_t t){
  logger.listen();
 
  logger.print(F("Test, "));
  logger.print(t);
  logger.print(F(" ,"));
  logger.print(temp);
  logger.print(F(" ,"));
  logger.print(humid);
  logger.print(F(" ,"));
  logger.print(baro);
  logger.print(F(" ,"));
  logger.print(co);
  logger.print(F(" ,"));
  logger.print(co2);
  logger.print(F(" ,"));
  logger.print(dust);
  logger.print(F(" ,"));
  logger.print(no2);
  logger.print(F(" ,"));
  logger.print(o3);
  logger.print(F(" ,"));
  logger.println(so2);
}

void WriteSD::setUpFile(char filename[12]){
  
   boolean newfile = false;
  //See if [filename] exists
  logger.print(F("size "));
  logger.print(filename);
  logger.write(13);

  while(1) {
    delay(100);
    if(logger.available()){
      //indicates that the file does not exist
      byte byteRead = logger.read();
      if(byteRead == '!'){
        newfile = true;
      }
      if(byteRead == '>')break;
    }
  }


  if (newfile){
    logger.print(F("new "));
    logger.print(filename);
    logger.write(13);
    delay(100);
    
    while(1) {
      delay(100);
      if(logger.available()){
        if(logger.read() == '>') break;
      }
        
    }
  }
  //start writing to the file
  logger.print("append ");
  logger.print(filename);
  logger.write(13); //This is \r

  //Wait for OpenLog to indicate file is open and ready for writing
  while(1) {
    delay(100);
    if(logger.available())
      if(logger.read() == '<') break;
  }


  if(newfile){
    logger.println(F("Location, Time, Temperature (*C), Humidity (%), Pressure (hPa), CO (PPB), CO2 (PPM), PM10 (ug/m3), NO2 (PPB), O3 (PPB), SO2 (PPB)"));
  }

}

