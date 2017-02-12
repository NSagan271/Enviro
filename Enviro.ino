#include <Time.h>
#include "Barometer.h"
#include "TempHumid.h"
#include "CO2.h"
#include "SpecSensor.h"

#define SPEC 4
#define OUT_TO_V 5.0/1023
#define TIMES 80

#define DUST A3
#define DUST_LED_POW 3
#define DUST_PIN2 6

#define DUST_DELAY1 280
#define DUST_DELAY2 40
#define DUST_OFF 9680

byte buff[64];
time_t lastTimeAdjust = 0;
byte len = 0;

Barometer b;
TempHumid t;
CO2 c;

//SpecSensors
SpecSensor o3;
SpecSensor so2;
SpecSensor co;
SpecSensor no2;

SpecSensor *s[SPEC] = {&o3, &so2, &co, &no2};
void setup() {
  // put your setup code here, to run once:
  pinMode(DUST_PIN2,OUTPUT);
  pinMode (DUST_LED_POW,OUTPUT);
  pinMode (DUST,INPUT);
  Serial.begin(9600);

  delay(2000);
  Serial.println(F("AT"));
  waitForOk();
  Serial.println(F("AT+CGACT=1,1"));
  waitForOk();
  Serial.println(F("AT+SAPBR=3,1,\"APN\",\"truphone.com\""));
  waitForOk();
  Serial.println(F("AT+SAPBR=1,1"));
  waitForOk();
  Serial.println(F("AT+HTTPINIT"));
  waitForOk();
  Serial.println(F("AT+HTTPPARA=\"URL\",\"http://enviro-nsagan271.c9users.io/d\""));
  waitForOk();
  Serial.println(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""));
  waitForOk();
  Serial.println(F("AT+HTTPPARA=\"CID\",1"));
  waitForOk();

  b.initSensor();
  t.initSensor();
  c.initSensor();
  for (int i = 0; i < SPEC; i++){
    s[i]->initSensor(i);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  b.updateData();
  t.updateData();
  c.updateData();
  for (int i = 0; i < SPEC; i++){
    s[i]->updateData();
  }
  logData();
}
float getDust(){
  float dust = 0;
  int count = 0;
  while(count<TIMES){
    digitalWrite(DUST_LED_POW,LOW);
    delayMicroseconds(DUST_DELAY1);
    dust = ((analogRead(DUST)*OUT_TO_V)-.1)/5;
    delayMicroseconds(DUST_DELAY2);
    digitalWrite(DUST_LED_POW,HIGH);
    delayMicroseconds(DUST_OFF);
    delay(62.5);
    count++;
  }
  count = 0;
  return dust/TIMES;
}

void logData(){
  
  if(lastTimeAdjust == 0 || now() - lastTimeAdjust >= SECS_PER_HOUR){
    Serial.println(F("AT+CIPGSMLOC=2,1"));
    while(len < 33 || !(buff[len-26] == 'L' && buff[len-3] == ':' )){
      if(Serial.available() > 0){
        buff[len] = Serial.read();
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
  

  
  Serial.println(F("AT+HTTPDATA=500,1000"));
  while(len < 8 || !(buff[len-8] == 'D' && buff[len-1] == 'D')){
    if(Serial.available() > 0){
      buff[len] = Serial.read();
      len++;
    }
  }
  len = 0;
  
  
  Serial.print(F("{\"document\":{\"location\":\"blc\",\"time\":"));
  Serial.print(now());
  Serial.print(F(",\"sensor\":{\"temp\":"));
  Serial.print(t.temp);
  Serial.print(F(",\"humid\":"));
  Serial.print(t.humidity);
  Serial.print(F(",\"barometer\":"));
  Serial.print(b.pressure);
  Serial.print(F(",\"co\":"));
  Serial.print(co.gasLevel);
  Serial.print(F(",\"co2\":"));
  Serial.print(c.co2);
  Serial.print(F(",\"dust\":"));
  Serial.print(getDust());
  Serial.print(F(",\"no2\":"));
  Serial.print(no2.gasLevel);
  Serial.print(F(",\"o3\":"));
  Serial.print(o3.gasLevel);
  Serial.print(F(",\"so2\":"));
  Serial.print(so2.gasLevel);
  Serial.println(F("}}}"));

  waitForOk();
  Serial.println(F("AT+HTTPACTION=1"));

}
void waitForOk(){
  while((len < 3 || !(buff[len-3] == 'O' && buff[len-2] == 'K')) && (len < 5 || !(buff[len-5] == 'E' && buff[len-1] == 'R'))){
    if(Serial.available() > 0){
      buff[len] = Serial.read();
      len++;
    }
  }
  len = 0;
}
