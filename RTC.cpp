#include "RTC.h"
#include <Arduino.h>
#define ADDRESS 0x68
//
//
//
//
//timezone offset
//8 is the default: pacific standard time
//
//PUT 7 FOR DAYLIGHT SAVINGS TIME
//
//
#define TIMEZONE 8

#define USE_OFFSET true

void RTC::init(){
  Wire.begin();
}
bool RTC::setTime(){
  temp2 = getTimestamp();
  if (sscanf(__DATE__, "%s %d %d", mon, &da, &yr) == 3 && sscanf(__TIME__, "%d:%d:%d", &hr, &m, &s)){
    if (mon[2] == 'l') mo = 7;
    else if (mon[1] == 'u') mo = 6;
    else if (mon[0] == 'J') mo = 1;
    else if (mon[0] == 'F') mo = 2;
    else if (mon[2] == 'y') mo = 5;
    else if (mon[0] == 'M') mo = 3;
    else if (mon[1] == 'p') mo = 4;
    else if (mon[0] == 'A') mo = 8;
    else if (mon[0] == 'S') mo = 9;
    else if (mon[0] == 'O') mo = 10;
    else if (mon[0] == 'N') mo = 11;
    else mo = 12;
    yr -= 1970; 
    
    if (USE_OFFSET){
      hr+=TIMEZONE;
      if (hr > 23){
        da++;
        hr += (hr%24);
        if (((yr-30)%4 == 0 && mo == 2 && da > 29) || ((yr-30)%4 >0 && mo == 2 && da>28) || (((ii < 8 && ii%2 == 1) || (ii >=8 && ii%2 == 0)) && da>31) || (((ii < 8 && ii%2 == 0) || (ii >=8 && ii%2 == 1))&& da>30)){
          mo++;
          da = 1;
          if (mo>12){
            yr++;
            mo = 1;
          }
        }
      }
    }
    temp = 0;
    for (ii = 1; ii< mo; ii++){
      if (ii == 2){
        temp+=(28);
        if ((yr-30)%4 == 0)temp++;
      }
      else if ((ii < 8 && ii%2 == 1) || (ii >=8 && ii%2 == 0)) temp+=(31);
      else temp+=(30);
    }
    temp+=(da-1);
    //Serial.println(temp2);
    //Serial.println(((unsigned long)temp*24*3600+((unsigned long)(yr)*365*24*3600 + (unsigned long)(yr + 1)/4*24*3600 + (unsigned long)hr*3600 + m*60 + s )));
    if (((unsigned long)temp*24*3600+((unsigned long)(yr)*365*24*3600 + (unsigned long)(yr + 1)/4*24*3600 + (unsigned long)hr*3600 + m*60 + s )) < temp2)return false;
    temp += (5 + (yr-30)*365 + (yr-27)/4);
    
    Wire.beginTransmission(ADDRESS);
 
    Wire.write((byte)0x00);                           
    Wire.write(decToBcd(s));               
    Wire.write(decToBcd(m));
    Wire.write(decToBcd(hr));      
    Wire.write(decToBcd(temp%7));
    Wire.write(decToBcd(da));
    Wire.write(decToBcd(mo));
    Wire.write(decToBcd(yr));
    Wire.endTransmission(); 

    return true;
  }

  return false;
}

unsigned long RTC::getTimestamp(){
  Wire.beginTransmission(ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(ADDRESS, 7);
  s = bcdToDec(Wire.read() & 0x7f);
  m = bcdToDec(Wire.read());
  hr = bcdToDec(Wire.read() & 0x3f);
  Wire.read();
  da = bcdToDec(Wire.read());
  mo = bcdToDec(Wire.read());
  yr = bcdToDec(Wire.read());
  temp = 0;
  for (ii = 1; ii< mo; ii++){
    if (ii == 2){
      temp+=(28);
      if ((yr-30)%4 == 0)temp+=(24);
    }
    else if ((ii < 8 && ii%2 == 1) || (ii >=8 && ii%2 == 0)) temp+=(31);
    else temp+=(30);
  }
  return temp*24*3600+((unsigned long)(yr)*365*24*3600 + (unsigned long)(yr + 1)/4*24*3600 + (unsigned long)(da-1)*24*3600 + (unsigned long)hr*3600 + m*60 + s);
}

byte RTC::bcdToDec(byte val){
  return ( (val/16*10) + (val%16) );
}

byte RTC::decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
}


