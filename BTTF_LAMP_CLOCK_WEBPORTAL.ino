
#include "Adafruit_NeoPixel.h"
#include "TM1637Display.h"
#include "WiFiManager.h"
#include "NTPClient.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 // Strip led DIN

#define red_CLK 16
#define red1_DIO 17
#define red2_DIO 18
#define red3_DIO 19

#define AM 32
#define PM 33

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 48 // Popular NeoPixel ring size

bool res;
int var=0;
int analogPin = 34;
const long utcOffsetInSeconds = 3600; // Offset in second

//========================USEFUL VARIABLES=============================
int UTC = 2; // UTC + value in hour - Summer time
int Display_backlight = 3; // Set displays brightness 0 to 7;
//======================================================================

// When setting up the NeoPixel library, we tell it how many pixels,
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// Setup the red displays
TM1637Display red1(red_CLK, red1_DIO);
TM1637Display red2(red_CLK, red2_DIO);
TM1637Display red3(red_CLK, red3_DIO);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds*UTC);

void setup() {

  pinMode(PIN, OUTPUT);
  pinMode(red_CLK, OUTPUT);
  pinMode(red1_DIO, OUTPUT);
  pinMode(red2_DIO, OUTPUT);
  pinMode(red3_DIO, OUTPUT);
  pinMode(AM, OUTPUT);
  pinMode(PM, OUTPUT);

  pinMode(analogPin, INPUT);
  //manager.resetSettings();

  Serial.begin(9600);
  
    WiFiManager manager;    
     
   manager.setTimeout(180);
  //fetches ssid and password and tries to connect, if connections succeeds it starts an access point with the name called "BTTF_CLOCK" and waits in a blocking loop for configuration
  res = manager.autoConnect("BTTF_LAMP_CLOCK","password");
  
  if(!res) {
  Serial.println("failed to connect and timeout occurred");
  ESP.restart(); //reset and try again
  }
  
  delay(3000);

  timeClient.begin();
  red1.setBrightness(Display_backlight);
  red2.setBrightness(Display_backlight);
  red3.setBrightness(Display_backlight);
  pixels.setBrightness(250);
  
}

void loop() {

  timeClient.update();
  Serial.print("Time: ");
  Serial.println(timeClient.getFormattedTime());
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);
  
  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);
  
red1.showNumberDecEx(currentMonth,0b01000000,true,2,0);
red1.showNumberDecEx(monthDay,0b01000000,true,2,2);
red2.showNumberDecEx(currentYear,0b00000000,true);
red3.showNumberDecEx(timeClient.getHours(),0b01000000,true,2,0);
red3.showNumberDecEx(timeClient.getMinutes(),0b01000000,true,2,2);

if((currentMonth*30 + monthDay) >= 121 && (currentMonth*30 + monthDay) < 331){
timeClient.setTimeOffset(utcOffsetInSeconds*UTC);} // Change daylight saving time - Summer
else {timeClient.setTimeOffset((utcOffsetInSeconds*UTC) - 3600);} // Change daylight saving time - Winter


if(timeClient.getHours()>=13){
  digitalWrite(AM,0);
  digitalWrite(PM,1);}
  
else if(timeClient.getHours()==12){
  digitalWrite(AM,0);
  digitalWrite(PM,1);}

else{
  digitalWrite(AM,1);
  digitalWrite(PM,0);}


  pixels.clear(); // Set all pixel colors to 'off'
  
  if(var>3){var = 0;} // Reset counter

// read the switch
  if(analogRead(analogPin) > 100)
  { 
    var = var+1 ;
    delay(45);
  }

Serial.print("Var=");
Serial.println(var);
Serial.print("Digital=");
Serial.println(analogRead(analogPin));

delay(100);
  switch (var) {
  case 0: 
  for(int i=0; i<6;i++){
  pixels.setPixelColor(i, pixels.Color(255,0,0));
  }
  for(int i=6; i<12;i++){
  pixels.setPixelColor(i, pixels.Color(160,160,0));
  }
  for(int i=12; i<18;i++){
  pixels.setPixelColor(i, pixels.Color(255,0,0));
  }
  pixels.show();
  break;

  case 1:
    for(int i=0; i<6;i++){
  pixels.setPixelColor(i, pixels.Color(0,0,255));
  }
  for(int i=6; i<12;i++){
  pixels.setPixelColor(i, pixels.Color(200,250,255));

  }
  for(int i=12; i<18;i++){
  pixels.setPixelColor(i, pixels.Color(0,0,255));
  }
 pixels.show();
 break;

 case 2:
  pixels.clear();
  for(int i=0; i<6;i++){
  pixels.setPixelColor(i, pixels.Color(255,0,10));
  }
    for(int i=6; i<12;i++){
  pixels.setPixelColor(i, pixels.Color(0,10,255));
  }
    for(int i=12; i<18;i++){
  pixels.setPixelColor(i, pixels.Color(255,0,10));
  }
 pixels.show();
 break;

case 3:
  pixels.clear();
  for(int i=0; i<16;i++){
  pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
 pixels.show();
 break;

  }



}

