// Code by Brent Luker
#include "TempHumid.h"
#define ADDRESS 0x27

void TempHumid::init(){
  Wire.begin();
}

void TempHumid::updateData(){
  
  byte humHigh, humLow, tempHigh, tempLow;
  unsigned int humData, tempData;

  // Request read
  Wire.beginTransmission(ADDRESS);
  Wire.endTransmission();
  delay(100);

  // If we didn't get 4 bytes from the sensor....
  Wire.requestFrom( (int) ADDRESS, (int) 4);
  int timeout = 0;
  while (Wire.available() < 4){
    timeout++;
    delay(50);
    if(timeout > 20)return;
  }

  humHigh  = Wire.read();
  humLow   = Wire.read();
  tempHigh = Wire.read();
  tempLow  = Wire.read();

  // Status can be
  // 0: Normal
  // 1: Stale - this data has already been read
  // 2: Command Mode - the sensor is in command mode
  // 3: Diagnostic - The sensor has had a diagnostic condition and data is
  //    invalid
  
  // Status is the top two bits of the high humidity byte
  Wire.endTransmission();
  status = (humHigh >> 6) & 0x03;
  if(status == 3){
    return; // temp and humidity will be unchanged
  }
  if (status == 1){
    temp = 0;
    humidity = 0;
    return;
  }
  // Keep the rest
  humHigh = humHigh & 0x3f;

  // OR in the low bytes
  humData  = humHigh  << 8 | humLow;
  tempData = tempHigh << 8 | tempLow;

  // The bottom two bits of the low temp byte are invalid, so we'll remove those
  tempData = tempData >> 2;

  humidity = humData * 6.10e-3; // 100 / (2^14 - 1)
  temp = tempData * 1.007e-2 - 40.0; // 165 / (2^14 - 1)
}

