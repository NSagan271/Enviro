#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>

#include "SpecSensor.h"
//REMEMBER TO CHANGE URL FROM SPEC TO ENVIRO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
#define SPEC 5
//Number of samples over a certain time period
#define TIMES 80
#define PERIOD 5000
#define OUT_TO_V 5.03/1023

#define O3AVG 1.677501
#define SO2AVG 1.799569565
#define COAVG 1.676456522
#define NO2AVG 1.56315

SpecSensor o3(A0, -7.2355E-3, O3AVG);
SpecSensor so2(A3, 4.059E-3, SO2AVG);
SpecSensor co(A4, 5.42E-4, COAVG);
SpecSensor no2(A5, -2.2345E-2, NO2AVG);
SpecSensor test(A1, 1, 1 );

SpecSensor *s[SPEC] = {&o3, &so2, &co, &no2, &test};

#define DUST A2
#define DUST_LED_POW 8

#define DUST_DELAY1 280
#define DUST_DELAY2 40
#define DUST_OFF 9680

double dust = 0;
double no2Gas;
double so2Gas;
double o3Gas;
double coGas;

SoftwareSerial fona(13,12);
void setup() {
  pinMode (DUST_LED_POW,OUTPUT);
  pinMode (DUST,INPUT);
  fona.begin(9600);
  // put your setup code here, to run once:
  
  delay(2000);
  fona.println("AT");
  waitForOk();
  
  fona.println(F("AT+CGACT=1,1"));
  waitForOk();
  fona.println(F("AT+SAPBR=3,1,\"APN\",\"fast.tmobile.com\""));
  waitForOk();
  fona.println(F("AT+SAPBR=1,1"));
  waitForOk();
  fona.println(F("AT+HTTPINIT"));
  waitForOk();
  fona.println(F("AT+HTTPPARA=\"URL\",\"http://spec-nsagan271.c9users.io/d\""));
  waitForOk();
  fona.println(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""));
  waitForOk();
  fona.println(F("AT+HTTPPARA=\"CID\",1"));
  waitForOk();
  fona.println("AT+HTTPINIT");
  waitForOk();

}

time_t lastTimeAdjust = 0;

byte buff[255];
byte len = 0;
byte i;

double x[4];
double y;
void loop() {
  for(int i = 0; i < TIMES; i++){
    for (int j = 0; j < SPEC; j++){
      s[j]->updateData();
    }
    delay(PERIOD/TIMES);
  }
  dust = getDust();
  o3Gas = o3.getGas();
  //x[0] = o3Gas/O3AVG;
  no2Gas = no2.getGas();
  //x[1] = no2Gas/NO2AVG;
  so2Gas = so2.getGas();
  //x[2] = so2Gas/SO2AVG;
  coGas = co.getGas();
  //x[3] = coGas/COAVG;

  //y = (x[0]+x[3])/2;
  //   
  /*o3Gas = (x[0]/(y)-1)*O3AVG/-7.2355E-3;
  if (o3Gas < 0) o3Gas = 0;
  no2Gas = (x[1]/(y)-1)*NO2AVG/-2.2345E-2;
  if (no2Gas < 0) no2Gas = 0;
  so2Gas = (x[2]/(y)-1)*SO2AVG/ 4.059E-3;
  if (so2Gas < 0) so2Gas = 0;
  coGas = (x[3]/(y)-1)*COAVG/5.42E-4;
  if (coGas < 0) coGas = 0;*/
  
  fonaLog();
}
void fonaLog(){
  
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
  
  
  fona.print(F("{\"document\":{\"location\":\"blc\",\"time\":"));
  fona.print(now());
  fona.print(F(",\"sensor\":{\"temp\":"));
  fona.print(0);
  fona.print(F(",\"humid\":"));
  fona.print(0);
  fona.print(F(",\"barometer\":"));
  fona.print(0);
  fona.print(F(",\"co\":"));
  fona.print(coGas,4);
  fona.print(F(",\"co2\":"));
  fona.print(0);
  fona.print(F(",\"dust\":"));
  fona.print(dust);
  fona.print(F(",\"no2\":"));
  fona.print(no2Gas,4);
  fona.print(F(",\"o3\":"));
  fona.print(o3Gas,4);
  fona.print(F(",\"so2\":"));
  fona.print(so2Gas,4);
  fona.print(F(",\"test\":"));
  fona.print(test.getGas(),4);
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
double getDust(){
  double d = 0.0;
  int count = 0;
  while(count<TIMES){
    digitalWrite(DUST_LED_POW,LOW);
    delayMicroseconds(DUST_DELAY1);
    d += ((analogRead(DUST)*OUT_TO_V)-.1)/.05;
    delayMicroseconds(DUST_DELAY2);
    digitalWrite(DUST_LED_POW,HIGH);
    delay(62.5);
    count++;
  }
  return d/count;
}
