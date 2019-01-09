//code by Naomi Sagan
#include "ESP.h"

//Streams request
#define STREAMS1 "POST /streams/ HTTP/1.1\r\nHOST: "
#define STREAMS2 "\r\ncontent-type: application/json\r\nauthorization: Basic "
#define STREAMS3 "\r\ncontent-length: "
#define STREAMS4 "\r\n\r\n"
#define STREAMSDATA1 "{\"device\":\""
#define STREAMSDATA2 "\",\"meta\":{\"source\":\""
#define STREAMSDATA3 "\",\"username\":\""
#define STREAMSDATA35 "\",\"name\":\""
#define STREAMSDATA4 "\"},\"data\":{\"time\":"
#define STREAMSDATA5 ",\"temp\":"
#define STREAMSDATA6 ",\"humid\":"
#define STREAMSDATA7 ",\"barometer\":"
#define STREAMSDATA8 ",\"co\":"
#define STREAMSDATA9 ",\"co2\":"
#define STREAMSDATA10 ",\"dust\":"
#define STREAMSDATA11 ",\"no2\":"
#define STREAMSDATA12 ",\"o3\":"
#define STREAMSDATA13 ",\"so2\":"
#define STREAMSDATA14 "}}"

SoftwareSerial esp(12,13);

void ESP::init(){
  setBaudRate();
}

void ESP::postData(float temp, float humid, float baro, int co2, int co, int o3, int so2, int no2, int dust, time_t timestamp){
  //Set up ESP
  esp.begin(9600);
  esp.listen();

  //Startup commands
  esp.print(F("AT\r\n"));
  waitForOK(500);

  esp.print(F("AT+CWMODE=1\r\n"));
  waitForOK(500);
  
  //Connect to WiFi
  esp.print(F("AT+CWJAP=\""));
  esp.print(Constants::SSID);
  esp.print(F("\",\""));
  esp.print(Constants::PASS);
  esp.print(F("\"\r\n\r\n"));
  waitForOK(2000);
  
  esp.print(F("AT+CIPSTART=\"TCP\",\""));
  esp.print(Constants::STREAMS_URL);
  esp.print(F("\",80\r\n"));
  waitForOK(1000);

  //Streams request
  streamsDataL = streamsDataLen(temp, humid, baro, co2, co, o3, so2, no2, dust, timestamp);
  
  esp.print(F("AT+CIPSEND="));
  esp.println(streamsReqLen(streamsDataL));
  waitForOK(1000);
  
  esp.print(STREAMS1);
  esp.print(Constants::STREAMS_URL);
  esp.print(STREAMS2);
  printHashedSecret();
  esp.print(STREAMS3);
  esp.print(streamsDataL);
  esp.print(STREAMS4);
  esp.print(STREAMSDATA1);
  esp.print(Constants::DEVICEID);
  esp.print(STREAMSDATA2);
  esp.print(Constants::SOURCE);
  esp.print(STREAMSDATA3);
  esp.print(Constants::USER);
  esp.print(STREAMSDATA35);
  esp.print(Constants::NAME);
  esp.print(STREAMSDATA4);
  esp.print(timestamp);
  esp.print(STREAMSDATA5);
  esp.print(temp);
  esp.print(STREAMSDATA6);
  esp.print(humid);
  esp.print(STREAMSDATA7);
  esp.print(baro);
  esp.print(STREAMSDATA8);
  esp.print(co);
  esp.print(STREAMSDATA9);
  esp.print(co2);
  esp.print(STREAMSDATA10);
  esp.print(dust);
  esp.print(STREAMSDATA11);
  esp.print(no2);
  esp.print(STREAMSDATA12);
  esp.print(o3);
  esp.print(STREAMSDATA13);
  esp.print(so2);
  esp.print(STREAMSDATA14);
  esp.print("\r\n\r\n");

  Serial.print(STREAMS1);
  Serial.print(Constants::STREAMS_URL);
  Serial.print(STREAMS2);
  serialPrintHashedSecret();
  Serial.print(STREAMS3);
  Serial.print(streamsDataL);
  Serial.print(STREAMS4);
  Serial.print(STREAMSDATA1);
  Serial.print(Constants::DEVICEID);
  Serial.print(STREAMSDATA2);
  Serial.print(Constants::SOURCE);
  Serial.print(STREAMSDATA3);
  Serial.print(Constants::USER);
  Serial.print(STREAMSDATA35);
  Serial.print(Constants::NAME);
  Serial.print(STREAMSDATA4);
  Serial.print(timestamp);
  Serial.print(STREAMSDATA5);
  Serial.print(temp);
  Serial.print(STREAMSDATA6);
  Serial.print(humid);
  Serial.print(STREAMSDATA7);
  Serial.print(baro);
  Serial.print(STREAMSDATA8);
  Serial.print(co);
  Serial.print(STREAMSDATA9);
  Serial.print(co2);
  Serial.print(STREAMSDATA10);
  Serial.print(dust);
  Serial.print(STREAMSDATA11);
  Serial.print(no2);
  Serial.print(STREAMSDATA12);
  Serial.print(o3);
  Serial.print(STREAMSDATA13);
  Serial.print(so2);
  Serial.print(STREAMSDATA14);
  Serial.print("\r\n\r\n");

  waitForClosed(2000);
}

int ESP::streamsDataLen(float temp, float humid,float baro, int co2, int co, int o3, int so2, int no2, int dust, time_t timestamp){
  return strlen(STREAMSDATA1) + strlen(Constants::DEVICEID) + strlen(STREAMSDATA2) + strlen(Constants::SOURCE) + strlen(STREAMSDATA3) + strlen(Constants::USER) +
    strlen(STREAMSDATA35) + strlen(Constants::NAME) + strlen(STREAMSDATA4)+ numChar((long)timestamp) +  strlen(STREAMSDATA5) + (numChar((int)temp)+3) + 
    strlen(STREAMSDATA6) + (numChar((int)humid)+3) + strlen(STREAMSDATA7) + (numChar((int)baro)+3) + strlen(STREAMSDATA8) + numChar(co) + strlen(STREAMSDATA9) + 
    numChar(co2) + strlen(STREAMSDATA10) + numChar(dust) + strlen(STREAMSDATA11) + numChar(no2) + strlen(STREAMSDATA12) + numChar(o3) + 
    strlen(STREAMSDATA13) + numChar(so2) + strlen(STREAMSDATA14); 
}

int ESP::streamsReqLen(int dataLength){
  return strlen(STREAMS1) + strlen(Constants::STREAMS_URL) + strlen(STREAMS2) + 
  ((int) (strlen(Constants::AUTH)/3) + (strlen(Constants::AUTH)%3==0?0:1))*4  + 
  (strlen(Constants::AUTH)%3==0?0: 3 - strlen(Constants::AUTH)%3) +
  strlen(STREAMS3) + numChar(dataLength) + strlen(STREAMS4) + dataLength;
}

int ESP::numChar(int num){
  if (num < 0){
    return 2 + ((num/10 == 0)?0:numChar(-num/10));
  }
  return 1 + ((num/10 == 0)?0:numChar(num/10));
}

int ESP::numChar(long num){
  if (num < 0){
    return 2 + ((num/10 == 0)?0:numChar(-num/10));
  }
  return 1 + ((num/10 == 0)?0:numChar(num/10));
}

void ESP::setBaudRate(){
  esp.begin(9600);
  esp.print(F("AT\r\n"));
  if(waitForOK(1000))return;
  

  for (long i:BAUD){
    Serial.println(i);
    esp.begin(i);
    esp.print(F("AT\r\n"));
    if(waitForOK(1000)){
      esp.print(F("AT+UART_DEF=9600,8,1,0,0\r\n"));
      waitForOK(1000);
      esp.begin(9600);
      break;
    }
  }
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
  while((len < 6 || !(out[len-6] == 'C' && out[len-5] == 'L' && out[len-4] == 'O'  && out[len-3] == 'S' && out[len-2] == 'E' && out[len-1] == 'D'))
  && (len < 5 || !(out[len-5] == 'E' && out[len-1] == 'R'))){
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

void ESP::printHashedSecret(){
  int i;
  for (i = 2; i < strlen(Constants::AUTH); i+=3){
    esp.print((char) byteTo64((Constants::AUTH[i-2] & 0xfc) >> 2));
    esp.print((char) byteTo64(((Constants::AUTH[i-2] & 0x03) << 4) + ((Constants::AUTH[i-1] & 0xf0) >> 4)));
    esp.print((char) byteTo64(((Constants::AUTH[i-1] & 0x0f) << 2) + ((Constants::AUTH[i] & 0xc0) >> 6)));
    esp.print((char) byteTo64(Constants::AUTH[i] & 0x3f));
  }
  if ((i+1) - strlen(Constants::AUTH) == 2){
    esp.print((char) byteTo64((Constants::AUTH[i-2] & 0xfc) >> 2));
    esp.print((char) byteTo64((Constants::AUTH[i-2] & 0x03) << 4));
    esp.print('==');
  }
  else if ((i+1) - strlen(Constants::AUTH) == 1){
    esp.print((char) byteTo64((Constants::AUTH[i-2] & 0xfc) >> 2));
    esp.print((char) byteTo64(((Constants::AUTH[i-2] & 0x03) << 4) + ((Constants::AUTH[i-1] & 0xf0) >> 4)));
    esp.print((char) byteTo64((Constants::AUTH[i-1] & 0x0f) << 2));
    esp.print('=');
  }
}

void ESP::serialPrintHashedSecret(){
  int i;
  for (i = 2; i < strlen(Constants::AUTH); i+=3){
    Serial.print((char) byteTo64((Constants::AUTH[i-2] & 0xfc) >> 2));
    Serial.print((char) byteTo64(((Constants::AUTH[i-2] & 0x03) << 4) + ((Constants::AUTH[i-1] & 0xf0) >> 4)));
    Serial.print((char) byteTo64(((Constants::AUTH[i-1] & 0x0f) << 2) + ((Constants::AUTH[i] & 0xc0) >> 6)));
    Serial.print((char) byteTo64(Constants::AUTH[i] & 0x3f));
  }
  if ((i+1) - strlen(Constants::AUTH) == 2){
    Serial.print((char) byteTo64((Constants::AUTH[i-2] & 0xfc) >> 2));
    Serial.print((char) byteTo64((Constants::AUTH[i-2] & 0x03) << 4));
    Serial.print('==');
  }
  else if ((i+1) - strlen(Constants::AUTH) == 1){
    Serial.print((char) byteTo64((Constants::AUTH[i-2] & 0xfc) >> 2));
    Serial.print((char) byteTo64(((Constants::AUTH[i-2] & 0x03) << 4) + ((Constants::AUTH[i-1] & 0xf0) >> 4)));
    Serial.print((char) byteTo64((Constants::AUTH[i-1] & 0x0f) << 2));
    Serial.print('=');
  }
}


byte ESP::byteTo64(byte b){
  if (b < 26){
    return 'A' + b;
   }
   if (b < 52){
    return 'a' + (b - 26);
   }
   if (b < 62) {
    return '0' + (b - 52);
   }
   if (b == 62){
    return '+';
   }
   return '/';
}


