#include <WiFi.h>
#include "time.h"
#include <IOXhop_FirebaseESP32.h>
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN    21
#define RST_PIN   22
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define FIREBASE_HOST "user-49128.firebaseio.com"
#define FIREBASE_AUTH "IDY4zJ8JI6SSpkODLNiFP4vNEooA2xQukpsuvuK9"
#define WIFI_SSID "S50"
#define WIFI_PASSWORD "widhiwinata"

MFRC522 mfrc522(SS_PIN, RST_PIN);   
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

const char* ntpServer = "asia.pool.ntp.org";
const long  gmtOffset_sec = 21600;
const int   daylightOffset_sec = 3600;

int second;
int minute;
int hour;
int day;
int month;
int year;
int weekday;
long current;
struct tm timeinfo;

 /*
struct tm
{
int    tm_sec;   //   Seconds [0,60]. 
int    tm_min;   //   Minutes [0,59]. 
int    tm_hour;  //   Hour [0,23]. 
int    tm_mday;  //   Day of month [1,31]. 
int    tm_mon;   //   Month of year [0,11]. 
int    tm_year;  //   Years since 1900. 
int    tm_wday;  //   Day of week [0,6] (Sunday =0). 
int    tm_yday;  //   Day of year [0,365]. 
int    tm_isdst; //   Daylight Savings flag. 
}
 */  
 
void printLocalTime(){
 if(!getLocalTime(&timeinfo)){
 Serial.println("Failed to obtain time");
 return;}
}

void setup() {
  Serial.begin(9600);
  Serial.println("LABEL,Date,Time,Nama");
  SPI.begin();     
  mfrc522.PCD_Init();   
                                             
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {delay(500);}
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

delay(1000);
printLocalTime();
 second = timeinfo.tm_sec;
  minute = timeinfo.tm_min;
  hour = timeinfo.tm_hour;
  day = timeinfo.tm_mday;
  month = timeinfo.tm_mon + 1;
  year = timeinfo.tm_year + 1900;
  weekday = timeinfo.tm_wday +1;
  /*
  Serial.print("Time from variables:  ");
  Serial.print(day);
  Serial.print(".");
  Serial.print(month);
  Serial.print(".");
  Serial.print(year);
  Serial.print(" --- ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
*/
for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
byte block;
byte len;
if ( ! mfrc522.PICC_IsNewCardPresent()) {return;}
if ( ! mfrc522.PICC_ReadCardSerial()) {return;}
Serial.println(F("**Card Detected:**"));
byte buffer1[18];
block = 4;
len = 18;
status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
if (status != MFRC522::STATUS_OK) {return;}
status = mfrc522.MIFARE_Read(block, buffer1, &len);
if (status != MFRC522::STATUS_OK) {return;}
String value = "";
for (uint8_t i = 0; i < 16; i++){value += (char)buffer1[i];}
value.trim();
Serial.print(value);
Serial.println(F("\n**End Reading**\n"));
mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1();

Serial.println( (String) "DATA,DATE,TIME," + value + ",AUTOSCROLL_20" );
if(value=="widhi"){Firebase.pushString("id", String ((String)""+(day)+":"+(month)+":"+(year)+":"+(hour)+":"+(minute)+":"+(second)+" "+" Widhi"+""));}
if(value=="sutran"){Firebase.pushString("id",  String ((String)""+(day)+":"+(month)+":"+(year)+":"+(hour)+":"+(minute)+":"+(second)+" "+" sutran"+""));}
  
  }


