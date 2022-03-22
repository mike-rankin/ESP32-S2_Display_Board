//Tools
//USB CDC On Boot: Enabled            ,this brings up the com port after a sketch upload
//USB Firmware MSC On Boot:Disabled   ,this brings up the Windows Explorer window
//USB DFU on Boot:Enabled             ,this reboots the board after sketch upload
//Upload Mode:Internal USB
//PSRAM: Enabled

/*
code	color
0x0000	Black
0xFFFF	White
0xBDF7	Light Gray
0x7BEF	Dark Gray
0xF800	Red
0xFFE0	Yellow
0xFBE0	Orange
0x79E0	Brown
0x07E0	Green
0x07FF	Cyan
0x001F	Blue
0xF81F	Pink
*/

#include "Adafruit_SHTC3.h"
#include <BH1750.h>          //https://github.com/claws/BH1750
#include <TFT_eSPI.h> 
#include <SPI.h>
#include <Wire.h>
#include <NTPClient.h>        //https://github.com/taranais/NTPClient
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include "fonts.h"
#include "Icons.h"

#define RED         2
#define GREEN       15
#define BLUE        14
#define CHG_STATUS  7
#define BAT_LEVEL   3

#define temperaure_offset (9.7) 

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();
TFT_eSPI tft = TFT_eSPI();  
BH1750 lightMeter;

const char* ssid = "SSID";  
const char* password = "PASSWORD";   

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String formattedDate;
String dayStamp;

uint32_t targetTime = 0;  

int CHG_STATE = 0;

const long interval = 60000;  //Update every minute
unsigned long previousMillis = 0;     

//byte omm = 99;
//bool initial = 1;
//byte xcolon = 0;
//unsigned int colour = 0;


void setup(void) 
{ 
  Serial.begin(115200);
  Wire.begin(8,9);
  tft.init();

  lightMeter.begin();
  shtc3.begin(); 
  //Serial.println("SHTC3 test");
  //if (! shtc3.begin()) 
  //{
   //Serial.println("Couldn't find SHTC3");
   //while (1) delay(1);
  //}
  //Serial.println("Found SHTC3 sensor");
 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(CHG_STATUS, INPUT);
  
  digitalWrite(RED, HIGH);  //Off
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);  //Not working
  
  
  tft.setSwapBytes(true);
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextSize(2); 
  Serial.println("Wii Test");
  Serial.print("Connecting to ");
  tft.setCursor (80, 0);  //over, down
  tft.println("Connecting to Wifi");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
   delay(500);
   Serial.print(".");
   tft.setCursor (80, 20);  
   tft.print("Trying...");
  }
  tft.println(" ");
  tft.println(" ");
  tft.setCursor (80, 40);  
  tft.println("WiFi connected.");
  tft.setCursor (80, 60);  
  tft.println("IP address: ");
  tft.setCursor (80, 80);  
  tft.println(WiFi.localIP());
  Serial.println(WiFi.localIP());
  delay(3000);

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(-10800);  //My timezone is -14400 or -10800
  delay(500);
  tft.fillScreen(TFT_BLACK);
  timeClient.update();
}

void loop() 
{
   sensors_event_t humidity, temp;
   shtc3.getEvent(&humidity, &temp);
  
   unsigned long currentMillis = millis(); 
   int currentHour;
   int currentMinute;
   int currentDay;

   tft.setTextColor(TFT_GREEN, TFT_BLACK);
   tft.setTextSize(2);  //2  
   //tft.setFreeFont(&Orbitron_Medium_18);
  //if (currentMillis - previousMillis >= interval) 
   //{
    //previousMillis = currentMillis; 
    //timeClient.update();
    //Serial.println("Time Updated");
    //tft.fillScreen(TFT_BLACK);     //This might rid the screen of garbage every minute
   //}

    //tft.setTextSize(2);
    //tft.setCursor (80, 0);  //over, down
    //tft.print(__DATE__);
    //tft.drawString(__DATE__,80,0,1); 
      
    currentHour = timeClient.getHours();
    currentMinute = timeClient.getMinutes(); 

    byte xpos = 160;  //Center the time
    byte ypos = 5; 

    if (currentHour<10) xpos+= tft.drawChar(' ',xpos,ypos,1); // leading zero for hours
    if (currentHour>12) currentHour=currentHour-12;
    xpos = xpos + tft.drawNumber(currentHour,xpos,ypos,1); // hours
    xpos = xpos + tft.drawChar(':',xpos,ypos,1); 

    if (currentMinute<10) xpos+= tft.drawChar('0',xpos,ypos,1); // leading zero for minutes
    xpos = xpos + tft.drawNumber(currentMinute,xpos,ypos,1); 
       
    //if (currentHour > 12)
    //{
     //currentHour = currentHour - 12;
     //tft.setCursor (180, 0);  //180
     //tft.print("PM"); 
    //}
    
    //tft.setCursor (155, 0);  //165
    //tft.print(currentHour);
    
    //tft.setCursor (178, 0);  //over, down
    //tft.print(":");

    //if (currentMinute<10)
    //{ 
     //tft.setCursor (190, 0);  
     //tft.print("0");
     //tft.setCursor (204, 0);  //208
     //tft.print(currentMinute);  
    //}

    //else
    //{
     //tft.setCursor (190, 0);  
     //tft.print(currentMinute);  
    //} 
     //tft.setCursor (208, 0);
     //tft.print("AM");
     delay(100);
     //Serial.print(".");
     WifiIcon(); 
     Battery_Level();

     //Sensor
     //tft.setFreeFont(&Orbitron_Medium_18);
     tft.setTextSize(1);
     //tft.setFreeFont(&Orbitron_Light_24);
     //tft.setFreeFont(&DSEG7_Classic_Bold_12);
     //tft.setFreeFont(&DSEG7_Classic_Bold_30);
     //tft.setFreeFont(&DSEG7_Classic_Bold_18);
     tft.setFreeFont(&DSEG14_Classic_Bold_18);
         //tft.setFreeFont(&DSEG7_Classic_Bold_24);
     //tft.setTextColor(0xBDF7,TFT_BLACK);
    
     tft.setTextColor(0xFBE0,TFT_BLACK);
     tft.drawString("Temp:",80,80);
     tft.drawString(String(((float)temp.temperature-temperaure_offset),1),180,80);
     //tft.print("รทC");
     tft.setCursor(230, 98);
     //tft.print(" C");
     tft.print(" รทC");  

     tft.drawString("Humid:",80,120);
     tft.drawString(String(((float)humidity.relative_humidity),1),180,120);
     tft.setCursor(230,138);
     tft.print(" %rH");

     tft.drawString("Light:",80,160);
     tft.drawString(String(((float)lightMeter.readLightLevel()),1),180,160);
     tft.setCursor(250,178); 
     tft.print(" lx");
 
     //Battery Level     
     //Serial.println(readBattery());
     
     //Serial.print("Temperature: "); Serial.print(temp.temperature-temperaure_offset); Serial.println(" degrees C");
     tft.setTextFont(1);
     delay(100);
}


void WifiIcon() 
{
 int32_t dbmicon = WiFi.RSSI();
 Serial.println(dbmicon);
   
 if (dbmicon <= -80 )  //-85 is greater than -80
 {
  tft.pushImage(245,0,16,16,wifi_1);  //was 225
 } 

 else if (dbmicon <= -50 && dbmicon >= -79)
 {
  tft.pushImage(245,0,16,16,wifi_2);  //was 225
 }
 
 else if (dbmicon <= -20 && dbmicon >= -49)
 {
  tft.pushImage(245,0,16,16,wifi_3);
 }

  else if (dbmicon >= -19)  
 {
  tft.pushImage(245,0,16,16,wifi_4);
 }
 
}

void Battery_Level()
{
 uint8_t percentage = 100;
   
 CHG_STATE = digitalRead(CHG_STATUS); 
 
 if (CHG_STATE == LOW)
 {
  Serial.println("Charging");
  tft.pushImage(285,5,32,16,charging); 
  delay(500); 
 }

 else
 {
  Serial.println("Charged"); 
  tft.pushImage(280,5,32,16,charged); 
  delay(500);
 }

  float voltage = analogRead(BAT_LEVEL) / 4096.0 * 7.23;      // 100K+100K voltage divider
  Serial.println("Voltage = " + String(voltage));
  percentage = 2808.3808 * pow(voltage, 4) - 43560.9157 * pow(voltage, 3) + 252848.5888 * pow(voltage, 2) - 650767.4615 * voltage + 626532.5703;
  if (voltage > 4.19) percentage = 100;
  else if (voltage <= 3.50) percentage = 0;
  Serial.println(percentage);
  Serial.println(" %");
  tft.drawNumber(percentage,275,30,1);
 
}
