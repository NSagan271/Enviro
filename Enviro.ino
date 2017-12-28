#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>

#include "SpecSensor.h"
#include "CO2.h"
#include "Dust.h"

#define SENSORS 5
//Number of samples over a certain time period
#define OUT_TO_V 5.03/1023 //conversion factor: analog output to volts

#define O3INDEX 0
#define SO2INDEX 1
#define COINDEX 2
#define NO2INDEX 3
#define CO2INDEX 4
#define DUSTINDEX 5

//sensors
SpecSensor o3(A0);
SpecSensor so2(A3);
SpecSensor co(A4);
SpecSensor no2(A5);
CO2 co2;
//Dust dust = Dust(A2, 8);

Sensor *s[SENSORS] = {&o3, &so2, &co, &no2, &co2/*, &dust*/};

//gas sensor values
double data[SENSORS];//same order as the sensor array

//FONA: communication through cellular network to the server
SoftwareSerial fona(13,12);
void setup() {//set up sensors and FONA
  //fona.begin(9600);
  Serial.begin(9600);
  while(!Serial);
  co2.init();
  delay(2000);
  /*fona.println("AT");
  waitForOk();
  
  fona.println(F("AT+CGACT=1,1"));
  waitForOk();
  fona.println(F("AT+SAPBR=3,1,\"APN\",\"fast.tmobile.com\""));
  waitForOk();
  fona.println(F("AT+SAPBR=1,1"));
  waitForOk();
  fona.println(F("AT+HTTPINIT"));
  waitForOk();
  fona.println(F("AT+HTTPPARA=\"URL\",\"http://enviro-nsagan271.c9users.io/d\""));
  waitForOk();
  fona.println(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""));
  waitForOk();
  fona.println(F("AT+HTTPPARA=\"CID\",1"));
  waitForOk();
  fona.println("AT+HTTPINIT");
  waitForOk();*/

}

time_t lastTimeAdjust = 0;

byte buff[255];
byte len = 0;
byte i;

double x[4];
double y;
void loop() {
  for (int i = 0; i < SENSORS; i++){
    data[i] = s[i]->getGas();
  }
  //fonaLog();
  serialPrint();
}
void fonaLog(){//log data to the server
  
  if(lastTimeAdjust == 0 || now() - lastTimeAdjust >= SECS_PER_HOUR){
    fona.println(F("AT+CIPGSMLOC=2,1"));
    while(len < 33 || !(buff[len-26] == 'L' && buff[len-3] == ':' )){
      if(fona.available() > 0){
        buff[len] = fona.read();
        len++;
      }
    }
    //Get the year (we're saving it in lastTimeAdjust to save space)
    buff[0] =  (buff[len-17] - '0')*10 + (buff[len-16] - '0');
    //Get month (rest of these are using spare bytes from the buffer)
    buff[1] = (buff[len-14] - '0')*10 + buff[len-13] - '0';
    //Get day
    buff[2] = (buff[len-11] - '0')*10 + buff[len-10] - '0';
    //Get hour
    buff[3] = (buff[len-8] - '0')*10 + buff[len-7] - '0';
    //Get minute
    buff[4] = (buff[len-5] - '0')*10 + buff[len-4] - '0';
    //Get second
    buff[5] = (buff[len-2] - '0')*10 + buff[len-1] - '0';
    //Put it into the time function (hour, minute, second, day, month, year)
    setTime(buff[3], buff[4], buff[5], buff[2], buff[1], buff[0]);
    lastTimeAdjust = now();
    waitForOk();
  }
  

  
  fona.println(F("AT+HTTPDATA=500,1000"));
  while(len < 8 || !(buff[len-8] == 'D' && buff[len-1] == 'D')){
    if(fona.available() > 0){
      buff[len] = fona.read();
      len++;
    }
  }
  len = 0;
  
  
  fona.print(F("{\"document\":{\"location\":\"test\",\"time\":"));
  fona.print(now());
  fona.print(F(",\"sensor\":{\"temp\":"));
  fona.print(0);
  fona.print(F(",\"humid\":"));
  fona.print(0);
  fona.print(F(",\"barometer\":"));
  fona.print(0);
  fona.print(F(",\"co\":"));
  fona.print(data[COINDEX],4);
  fona.print(F(",\"co2\":"));
  fona.print(data[CO2INDEX]);
  fona.print(F(",\"dust\":"));
  fona.print(data[DUSTINDEX]);
  fona.print(F(",\"no2\":"));
  fona.print(data[NO2INDEX],4);
  fona.print(F(",\"o3\":"));
  fona.print(data[O3INDEX],4);
  fona.print(F(",\"so2\":"));
  fona.print(data[SO2INDEX],4);
  fona.println(F("}}}"));

  waitForOk();
  fona.println(F("AT+HTTPACTION=1"));

}
void waitForOk(){
  while((len < 3 || !(buff[len-3] == 'O' && buff[len-2] == 'K')) && (len < 5 || !(buff[len-5] == 'E' && buff[len-1] == 'R'))){
    if(fona.available() > 0){
      buff[len] = fona.read();
      len++;
    }
  }
  len = 0;
}

void serialPrint(){
  Serial.print(F("{\"document\":{\"location\":\"test\",\"time\":"));
  Serial.print(now());
  Serial.print(F(",\"sensor\":{\"temp\":"));
  Serial.print(0);
  Serial.print(F(",\"humid\":"));
  Serial.print(0);
  Serial.print(F(",\"barometer\":"));
  Serial.print(0);
  Serial.print(F(",\"co\":"));
  Serial.print(data[COINDEX],4);
  Serial.print(F(",\"co2\":"));
  Serial.print(data[CO2INDEX]);
  Serial.print(F(",\"dust\":"));
  //Serial.print(data[DUSTINDEX]);
  Serial.print(F(",\"no2\":"));
  Serial.print(data[NO2INDEX],4);
  Serial.print(F(",\"o3\":"));
  Serial.print(data[O3INDEX],4);
  Serial.print(F(",\"so2\":"));
  Serial.print(data[SO2INDEX],4);
  Serial.println(F("}}}"));
}

