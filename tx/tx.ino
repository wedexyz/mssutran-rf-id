#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Setup Wifi
const char *ssid     = "S50";
const char *password = "widhiwinata";
const int timeZone = 7; 
const char *timeServer = "id.pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, timeServer, timeZone * 3600);
char daysOfTheWeek[7][12] = {"minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};


LiquidCrystal_I2C lcd(0x27, 16, 4);

String dataIn;
String dt[10];
int i;
boolean parsing=false;

void setup(){
  Serial.begin(9600);
  //setup parsing
  dataIn="";
  //setup wifi
  WiFi.begin(ssid, password);
  //setup waktu
  //setup lcd
  Wire.begin(2,0);
  lcd.begin(); 
  lcd.backlight(); 
  lcd.print("Absensi RF ID"); 
  delay(1000);
  lcd.clear();
}


void parsingData(){
int j=0;
Serial.print("data masuk : ");                          //kirim data yang telah diterima sebelumnya
Serial.print(dataIn);
Serial.print("\n");
dt[j]="";                                             //inisialisasi variabel, (reset isi variabel)

for(i=1;i<dataIn.length();i++){                      //proses parsing data
if ((dataIn[i] == '#') || (dataIn[i] == ','))       //pengecekan tiap karakter dengan karakter (#) dan (,)
{
j++;                                              //increment variabel j, digunakan untuk merubah index array penampung
dt[j]="";                                        //inisialisasi variabel array dt[j]
}
else{
dt[j] = dt[j] + dataIn[i];                     //proses tampung data saat pengecekan karakter selesai.
  }
}
/*
Serial.print("data 1 : ");
Serial.print(dt[0].toInt());
Serial.print("\n");
Serial.print("data 2 : ");
Serial.print(dt[1].toInt());
Serial.print("\n\n");
*/

//lcd print
lcd.setCursor(0,0);lcd.print(dataIn[28]);
lcd.setCursor(1,0);lcd.print(dataIn[29]);
lcd.setCursor(2,0);lcd.print(dataIn[30]);
lcd.setCursor(3,0);lcd.print(dataIn[31]);
lcd.setCursor(4,0);lcd.print(dataIn[32]);
lcd.setCursor(5,0);lcd.print(dataIn[33]);
delay(1000);
lcd.setCursor(0,0);lcd.print("Terima Kasih");
delay(1000);
lcd.clear();
}


void loop() {
if(Serial.available()>0) {
    char inChar = (char)Serial.read();
    dataIn += inChar;
    if (inChar == '\n') {
    parsing = true;
  }
}
 
if(parsing){
    parsingData();
    parsing=false;
    dataIn="";
  }
  waktu();
}
void waktu(){
timeClient.update();
  lcd.setCursor(9,1);lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.setCursor(0,1);lcd.print(timeClient.getHours());
  lcd.setCursor(2,1); lcd.print(':');
  lcd.setCursor(3,1);lcd.print(timeClient.getMinutes());
  lcd.setCursor(5,1);lcd.print(':');
  lcd.setCursor(6,1);lcd.print(timeClient.getSeconds());
  
if (timeClient.getSeconds()==59){lcd.clear();}
if (timeClient.getMinutes()==59){lcd.clear();}
if (timeClient.getHours()==12){lcd.clear();}

  }







