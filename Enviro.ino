/*
 * SET "LONGSTARTUP" TO TRUE IF THE DEVICE HAS BEEN W/O
 * POWER FOR SEVERAL DAYS OR LONGER!!
 * 
 * 
 * IT WILL TAKE OVER AN HOUR FOR
 * THE SENSORS TO WARM UP.
 * 
 */
#define LONGSTARTUP false
/*
 * 
 * MAKE SURE YOU EDIT:
 * ESP.cpp:
 * WIFI username and password
 * Device location and name
 * 
 * 
 */

#include "Sensor.h"
#include "SpecSensor.h"
#include "CO2.h"
#include "Dust.h"
#include "Barometer.h"
#include "TempHumid.h"
#include "OLED.h"
#include "WriteSD.h"
#include "ESP.h"
#include "RTC.h"

//number of sensors
#define SENSORS 6

//indices of each sensor
#define O3INDEX 0
#define SO2INDEX 2
#define COINDEX 1
#define NO2INDEX 3
#define CO2INDEX 4
#define DUSTINDEX 5

//sensors
Barometer baro;
CO2 co2(6,7);
SpecSensor o3(HIGH,HIGH);
SpecSensor so2(HIGH,LOW);
SpecSensor co(LOW,LOW);
SpecSensor no2(LOW,HIGH);

Dust dust(A0, 8);

Sensor *s[SENSORS] = {&o3, &co, &so2, &no2, &co2, &dust};

//temperature and humidity
TempHumid th;
float temp;
float humid;
float pressure;

//WiFi module
ESP wifi;

//Realtime clock
RTC rtc;
//gas sensor values
unsigned int data[SENSORS];

OLED screen;
WriteSD sd;

void setup() {//set up sensors and FONA
  Serial.begin(57600);

  rtc.init();
  if (Serial) rtc.setTime();
  screen.init();
  Serial.println(F("Screen"));
  for (int i = 0; i < SENSORS; i++){
    s[i]->init();
    Serial.println(i);
  }
  th.init();
  Serial.println(F("TH"));
  sd.init();
  Serial.println(F("SD"));
  wifi.init();
  delay(2000);
  //if (LONGSTARTUP) longWarmUp();
  //else sensorWarmUp();
}


void loop() {
  collectData();
  Serial.println(F("Updated"));
  screen.updateValues((int)temp, (int)humid, (int)pressure, data[CO2INDEX], data[COINDEX], data[O3INDEX], data[SO2INDEX], data[NO2INDEX], data[DUSTINDEX]);
  Serial.println(F("Screen"));
  sd.writeData(temp, humid, pressure, data[CO2INDEX], data[COINDEX], data[O3INDEX], data[SO2INDEX], data[NO2INDEX], data[DUSTINDEX], rtc.getTimestamp());
  Serial.println(F("SD"));
  wifi.postData(temp, humid, pressure, data[CO2INDEX], data[COINDEX], data[O3INDEX], data[SO2INDEX], data[NO2INDEX], data[DUSTINDEX], rtc.getTimestamp());
}

void collectData(){
  th.updateData();
  temp = th.temp;
  humid = th.humidity;
  pressure = baro.getData();
  Serial.println(F("TH"));
  for (int i = 0; i < SENSORS; i++){
    data[i] = s[i]->getData();
    Serial.println(i);
  }
}

void sensorWarmUp(){
  screen.wait();
  for (int i = 0; i < 10; i++){
    collectData();
    delay(2000);
  }
}
void longWarmUp(){
  screen.longwait();
  for (int i = 0; i < 60; i++){
    collectData();
    delay(60000);
  }
}

