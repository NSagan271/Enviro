//code by Naomi Sagan
#include "ESP.h"

/*
 * 
 
 * DEFINE WIFI SSID AND PASSWORD BELOW 
 * 
 * 
 * */
#define SSID "SLHS Guest"
#define PASS "asdfgh!@"
/*
 * 
 
 * DEFINE LOCATION NAME BELOW 
 * 
 * 
$ * */
#define LOC_SHORT "slhs"
#define LOCATION "SLHS"
#define DEVICE_NAME "Ms. Thaler"


//#define DATA1 "{"
#define DATA1 "{\"document\":{\"location\":{\"name\":{\"long\":\""
#define DATA2 "\",\"short\":\""
#define DATA3 "\"}},\"name\":\""
//#define REQ1 "POST /api/v1.6/devices/my-data-source/?token=A1E-KXjQUlToQLatgi7E1MU3fKIvPty3pC HTTP/1.1\r\nHost: "
#define REQ1 "POST /d HTTP/1.1\r\nHOST: "
#define REQ2 "\r\ncontent-type: application/json\r\nconnection: close\r\ncontent-length: "

//#define URL "enviro-nsagan271.c9users.io"
#define URL "magnitude-aq.herokuapp.com"
SoftwareSerial esp(12,13);

void ESP::init(){
  setBaudRate();
}

void ESP::postData(float temp, float humid, float baro, int co2, int co, int o3, int so2, int no2, int dust, time_t timestamp){
  esp.begin(9600);
  esp.listen();
  esp.print(F("AT\r\n"));
  waitForOK(500);
  
  esp.print(F("AT+CWJAP=\""));
  esp.print(SSID);
  esp.print(F("\",\""));
  esp.print(PASS);
  esp.print(F("\"\r\n"));
  
  waitForOK(2000);
  
  esp.print(F("AT+CIPSTART=\"TCP\",\""));
  esp.print(URL);
  esp.print(F("\",80\r\n"));
  
  waitForOK(1000);
  
  //data = "{\"document\":{\"location\":\""+String(LOC_SHORT)+"\",\"name\":\"" +LOCATION + "\",\"time\":" + String(timestamp) + ",\"sensor\":{\"temp\":" + String(temp) + ",\"humid\":" + String(humid) + ",\"barometer\":" + String(baro) + ",\"co\":" + String(co) + ",\"co2\":" + String(co2) + ",\"dust\":" + String(dust) + ",\"no2\":" + String(no2) + ",\"o3\":" + String(o3) + "}}}";
  //req = "POST /test HTTP/1.1\r\nHost: https://enviro-nsagan271.c9users.io\r\nAccept:*/*\r\nContent-Length: 16\r\nContent-Type: application/json\r\n\r\n{\"document\":123}\r\nConnection: close\r\n";

  dataL = dataLen(temp, humid, baro, co2, co, o3, so2, no2, dust, timestamp);
  
  esp.print(F("AT+CIPSEND="));
  esp.println(reqLen(dataL));
  waitForOK(1000);
  
  esp.print(REQ1);
  esp.print(URL);
  esp.print(REQ2);
  esp.print(dataL);
  esp.print("\r\n\r\n");
  esp.print(DATA1);
  esp.print(LOCATION);
  esp.print(DATA2);
  esp.print(LOC_SHORT);
  esp.print(DATA3);
  esp.print(DEVICE_NAME);
  esp.print(F("\",\"time\":"));
  esp.print(timestamp);
  esp.print(F(",\"sensor\":{\"temp\":"));
  //esp.print(F("\"temp\":"));
  esp.print(temp);
  esp.print(F(",\"humid\":"));
  esp.print(humid);
  esp.print(F(",\"barometer\":"));
  esp.print(baro);
  esp.print(F(",\"co\":"));
  esp.print(co);
  esp.print(F(",\"co2\":"));
  esp.print(co2);
  esp.print(F(",\"dust\":"));
  esp.print(dust);
  esp.print(F(",\"no2\":"));
  esp.print(no2);
  esp.print(F(",\"o3\":"));
  esp.print(o3);
  esp.print(F(",\"so2\":"));
  esp.print(so2);
  esp.print(F("}}}"));
  esp.println(F("\r\n\r\n\r\n\r\n"));

   /*Serial.print(REQ1);
  Serial.print(URL);
  Serial.print(REQ2);
  Serial.print(dataL);
  Serial.print(REQ3);
  Serial.print(DATA1);
  //esp.print(LOC_SHORT);
  //esp.print(F("\",\"name\":\""));
  //esp.print(LOCATION);
  //esp.print(F("\",\"time\":"));
  //esp.print(timestamp);
  //esp.print(F(",\"sensor\":{\"temp\":"));
  Serial.print(F("\"temp\":"));
  Serial.print(temp);
  Serial.print(F(",\"humid\":"));
  Serial.print(humid);
  Serial.print(F(",\"barometer\":"));
  Serial.print(baro);
  Serial.print(F(",\"co\":"));
  Serial.print(co);
  Serial.print(F(",\"co2\":"));
  Serial.print(co2);
  Serial.print(F(",\"dust\":"));
  Serial.print(dust);
  Serial.print(F(",\"no2\":"));
  Serial.print(no2);
  Serial.print(F(",\"o3\":"));
  Serial.print(o3);
  Serial.print(F(",\"so2\":"));
  Serial.print(so2);
  //esp.print(F("}}}"));
  Serial.print(F("}"));
  Serial.println(REQ4);
  Serial.println(F("\r\n\r\n\r\n\r\n"));*/
  waitForClosed(600);
}

int ESP::dataLen(float temp, float humid,float baro, int co2, int co, int o3, int so2, int no2, int dust, time_t timestamp){
  return sizeof(DATA1)/sizeof(DATA1[0]) + sizeof(LOC_SHORT)/sizeof(LOC_SHORT[0]) + sizeof(DATA2)/sizeof(DATA2[0]) + sizeof(LOCATION)/sizeof(LOCATION[0]) + sizeof(DATA3)/sizeof(DATA3[0])+ sizeof(DEVICE_NAME)/sizeof(DEVICE_NAME[0])+ 9 + numChar((int)timestamp) +  18 + (numChar((int)temp)+3) + 9 + (numChar((int)humid)+3) + 13 + (numChar((int)baro)+3) + 6 + numChar(co) + 7 + numChar(co2) + 8 + numChar(dust) + 7 + numChar(no2) + 6 + numChar(o3) + 7 + numChar(so2) + 3; 
}

int ESP::reqLen(int dataLength){
  return sizeof(REQ1)/sizeof(REQ1[0]) + sizeof(URL)/sizeof(URL[0]) + sizeof(REQ2)/sizeof(REQ2[0]) + numChar(dataLength) +4+ dataLength;
}

int ESP::numChar(int num){
  return 1 + ((num/10 == 0)?0:numChar(num/10));
}

void ESP::setBaudRate(){
  esp.begin(9600);
  esp.print(F("AT\r\n"));
  if(waitForOK(1000))return;
  esp.begin(115200);
  esp.print(F("AT+CIOBAUD=9600"));
  waitForOK(1000);
  esp.begin(9600);
}

bool ESP::waitForOK(int timeout){
  len = 0;
  count = 0;
  while((len < 3 || !(out[len-3] == 'O' && out[len-2] == 'K')) && (len < 5 || !(out[len-5] == 'E' && out[len-1] == 'R'))){
    count++;
    if (count > timeout) return false;
    if(esp.available() > 0){
      out[len] = esp.read();
      Serial.write(out[len]);
      len++;
      if (len >= 15){
        for (int i = 5; i > 0; i--)
          out[5-i] = out[len-i];
        len = 5;
      }
     
    }
    delay(10);
  }
  return true;
}

void ESP::waitForClosed(int timeout){
  len = 0;
  count = 0;
  while((len < 6 || !(out[len-6] == 'C' && out[len-5] == 'L' && out[len-4] == 'O' &&out[len-3] == 'S' &&out[len-2] == 'E' &&out[len-1] == 'D')) && (len < 5 || !(out[len-5] == 'E' && out[len-1] == 'R'))){
    count++;
    if (count > timeout)break;
    if(esp.available() > 0){
      out[len] = esp.read();
      Serial.write(out[len]);
      len++;
      if (len >= 15){
        for (int i = 6; i > 0; i--)
          out[6-i] = out[len-i];
        len = 6;
      }
    }
    delay(10);
  }
  esp.print(F("AT+CIPCLOSE\r\n"));
  waitForOK(1000);
}


