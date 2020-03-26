#include <WiFi.h>
#include "time.h"
#include <IOXhop_FirebaseESP32.h>
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN    21
#define RST_PIN   22
#define SIZE_BUFFER     30
#define MAX_SIZE_BLOCK  30
//firebase
#define FIREBASE_HOST "user-49128.firebaseio.com"
#define FIREBASE_AUTH "IDY4zJ8JI6SSpkODLNiFP4vNEooA2xQukpsuvuK9"
//wifi
#define WIFI_SSID "S50"
#define WIFI_PASSWORD "widhiwinata"
//rfid
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
int dayofweek;
int weekday;
long current;
struct tm timeinfo;

void printLocalTime(){
 if(!getLocalTime(&timeinfo)){
 Serial.println("Failed to obtain time");
 return;}
}

void setup() {
  Serial.begin(9600);
  //DAQ
  Serial.println("LABEL,Tanggal,Waktu,Nomor,Nama ");
  //rf id
  SPI.begin();     
  mfrc522.PCD_Init();      
  //wifi                                   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 //waktu
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  //firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
printLocalTime();
second = timeinfo.tm_sec;
minute = timeinfo.tm_min;
hour = timeinfo.tm_hour;
day = timeinfo.tm_mday;
month = timeinfo.tm_mon + 1;
year = timeinfo.tm_year + 1900;
weekday = timeinfo.tm_wday +1;

for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
byte block;
byte len;
if ( ! mfrc522.PICC_IsNewCardPresent()) {return;}
if ( ! mfrc522.PICC_ReadCardSerial()) {return;}
//Serial.println(F("**Card Detected:**"));
byte buffer1[18];

block = 1;
len = 18;
status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
if (status != MFRC522::STATUS_OK) {return;}
status = mfrc522.MIFARE_Read(block, buffer1, &len);
if (status != MFRC522::STATUS_OK) {return;}
String value = "";
for (uint8_t i = 0; i < 16; i++){value += (char)buffer1[i];}
value.trim();


block = 4;
len = 18;
status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
if (status != MFRC522::STATUS_OK) {return;}
status = mfrc522.MIFARE_Read(block, buffer1, &len);
if (status != MFRC522::STATUS_OK) {return;}
String valuex = "";
for (uint8_t i = 0; i < 16; i++){valuex += (char)buffer1[i];}
valuex.trim();

//Serial.print(value);
//Serial.print(valuex);
//Serial.println(F("\n**End Reading**\n"));
mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1();

Serial.println( (String) "DATA,DATE,TIME,"+ valuex+","+value+" "+" "+ " "+" "+" "+" "+" "+" "+" "+" "+" "+",AUTOSCROLL_20" );
Firebase.pushString("id", String ((String)""+" ,"+(value)+" "+(day)+"/"+(month)+"/"+(year)+" Jam "+(hour)+":"+(minute)+":"+(second)+""));
//Serial.println( (String) "" +'*'+(day)+','+(month)+','+(year)+','+(hour)+','+(minute)+','+(second)+","+(value)+','+(valuex)+'#'+"" );

  }

