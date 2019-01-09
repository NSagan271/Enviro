//code by Naomi Sagan

#include "WriteSD.h"

/*
 * 
 * DEFINE YOUR LOCATION NAME
 * 
 * 
 */
#define LOC "Thaler"

#define TYPE ".csv"

#define RESET 9
SoftwareSerial logger(10,11);

void WriteSD::init(int day, int mo, int yr){
  pinMode(RESET, OUTPUT);
  logger.begin(9600);
  //Reset OpenLog

  logger.listen();
  digitalWrite(RESET, LOW);
  delay(100);
  digitalWrite(RESET, HIGH);
  delay(1000);

 commandMode();
  
  //make directory DATA if it doesn't already exist
  timeout = 0;
  logger.print(F("md DATA"));
  logger.write(13);
  while(1) {
    delay(100);
    timeout++;
    if (timeout > 100) return;
    if(logger.available())
      if(logger.read() == '>') break;
  }


  //go to directory DATA
  timeout = 0;
  logger.print(F("cd DATA"));
  logger.write(13);
  while(1) {
    delay(100);
    timeout++;
    if (timeout > 100) return;
    if(logger.available())
      if(logger.read() == '>') break;
  }
  setUpFile(day, mo, yr);
}

void WriteSD::writeData(float temp, float humid, float baro, int co2, int co, int o3, int so2, int no2, int dust, int hr, int min, int sec, int day, int mo, int yr){
  logger.listen();

  commandMode();
  setUpFile(day, mo, yr);
  
  logger.print(LOC);
  logger.print(F(", "));
  
  logger.print(day/10);
  logger.print(day%10);
  logger.print('-');
  logger.print(mo/10);
  logger.print(mo%10);
  logger.print('-');
  logger.print(yr/10);
  logger.print(yr%10);
  
  logger.print(F(" "));
  
  logger.print(hr/10);
  logger.print(hr%10);
  logger.print(':');
  logger.print(min/10);
  logger.print(min%10);
  logger.print(':');
  logger.print(sec/10);
  logger.print(sec%10);

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

void WriteSD::commandMode(){
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
      if(logger.read() == '>'){
        Serial.println(F("In command mode"));
        break;
      }
  }
}

void WriteSD::setUpFile(int day, int mo, int yr){
  boolean newfile = false;
  //See if [filename] exists
  logger.print(F("size "));
  logger.print(day/10);
  logger.print(day%10);
  logger.print('-');
  logger.print(mo/10);
  logger.print(mo%10);
  logger.print('-');
  logger.print(yr/10);
  logger.print(yr%10);
  logger.print(TYPE);
  logger.write(13);

  timeout = 0;
  while(1) {
    delay(100);
    timeout++;
    if (timeout > 100){
      newfile = true;
      return;
    }
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
    logger.print(day/10);
    logger.print(day%10);
    logger.print('-');
    logger.print(mo/10);
    logger.print(mo%10);
    logger.print('-');
    logger.print(yr/10);
    logger.print(yr%10);
    logger.print(TYPE);
    logger.write(13);
    delay(100);

    timeout = 0;
    while(1) {
      delay(100);
      timeout++;
      if (timeout > 100){
        newfile = true;
        return;
      }
      if(logger.available()){
        if(logger.read() == '>') break;
      }
        
    }
  }
  //start writing to the file
  logger.print(F("append "));
  logger.print(day/10);
  logger.print(day%10);
  logger.print('-');
  logger.print(mo/10);
  logger.print(mo%10);
  logger.print('-');
  logger.print(yr/10);
  logger.print(yr%10);
  logger.print(TYPE);
  logger.write(13); //This is \r

  //Wait for OpenLog to indicate file is open and ready for writing
  timeout = 0;
  while(1) {
    delay(100);
    timeout++;
    if (timeout > 100) return;
    if(logger.available())
      if(logger.read() == '<') break;
  }


  if(newfile){
    logger.println(F("Location, Time, Temperature (*C), Humidity (%), Pressure (hPa), CO (PPB), CO2 (PPM), PM10 (ug/m3), NO2 (PPB), O3 (PPB), SO2 (PPB)"));
  }

}

