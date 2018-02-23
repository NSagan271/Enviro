#include "OLED.h"

void OLED::init(){
  Wire.begin();
  SeeedOled.init();
  SeeedOled.clearDisplay();           //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();       //Set display to Normal mode
  SeeedOled.setPageMode();            //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("Starting up!");
}
void OLED::wait(){
  SeeedOled.clearDisplay();
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("Waiting for the");
  
  SeeedOled.setTextXY(1,0);
  SeeedOled.putString("sensors to warm");
  
  SeeedOled.setTextXY(2,0);
  SeeedOled.putString("up. This may");
  
  SeeedOled.setTextXY(3,0);
  SeeedOled.putString("take a few");

  SeeedOled.setTextXY(4,0);
  SeeedOled.putString("minutes.");

  SeeedOled.setTextXY(6,0);
  SeeedOled.putString("Please be");
  SeeedOled.setTextXY(7,0);
  SeeedOled.putString("patient.");
}

void OLED::longwait(){
  SeeedOled.clearDisplay();
  SeeedOled.setTextXY(0,0);
  SeeedOled.putString("Waiting for the");
  
  SeeedOled.setTextXY(1,0);
  SeeedOled.putString("sensors to warm");
  
  SeeedOled.setTextXY(2,0);
  SeeedOled.putString("up. This will");
  
  SeeedOled.setTextXY(3,0);
  SeeedOled.putString("take over");

  SeeedOled.setTextXY(4,0);
  SeeedOled.putString("an hour.");

  SeeedOled.setTextXY(6,0);
  SeeedOled.putString("Please be");
  SeeedOled.setTextXY(7,0);
  SeeedOled.putString("patient.");
}
void OLED::updateValues(unsigned int temp, unsigned int humid, unsigned int baro, unsigned int co2, unsigned int co, unsigned int o3, unsigned int so2, unsigned int no2, unsigned int dust){
  SeeedOled.clearDisplay();

  SeeedOled.setTextXY(0,0);
  SeeedOled.putNumber(temp);
  SeeedOled.putString("*C");
  
  SeeedOled.setTextXY(0,5);
  SeeedOled.putString("|");
  SeeedOled.putNumber(humid);
  SeeedOled.putString("% HUMID");
  
  SeeedOled.setTextXY(1,0);
  SeeedOled.putNumber(baro);
  SeeedOled.setTextXY(1,6);
  SeeedOled.putString("hPa");

  SeeedOled.setTextXY(2,0);
  SeeedOled.putNumber(co2);
  SeeedOled.setTextXY(2,6);
  SeeedOled.putString("PPM CO2");

  SeeedOled.setTextXY(3,0);
  SeeedOled.putNumber(co);
  SeeedOled.setTextXY(3,6);
  SeeedOled.putString("PPB CO");

  SeeedOled.setTextXY(4,0);
  SeeedOled.putNumber(o3);
  SeeedOled.setTextXY(4,6);
  SeeedOled.putString("PPB O3");

  SeeedOled.setTextXY(5,0);
  SeeedOled.putNumber(no2);
  SeeedOled.setTextXY(5,6);
  SeeedOled.putString("PPB NO2");

  SeeedOled.setTextXY(6,0);
  SeeedOled.putNumber(so2);
  SeeedOled.setTextXY(6,6);
  SeeedOled.putString("PPB SO2");

  SeeedOled.setTextXY(7,0);
  SeeedOled.putNumber(dust);
  SeeedOled.setTextXY(7,6);
  SeeedOled.putString("uG/M3 PM10");
}
