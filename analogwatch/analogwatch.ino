#include<time.h>
#include <Arduino_GFX_Library.h>
#include <WiFi.h>
#include "secrets.h"


/*!!!ESP32 to GC9A01 wiring!!!
  ESP32 GND => GC9A01 GND
  ESP32 VCC => GC9A01 VCC (ensure using 3.3V only!)
  ESP32 GPIO18 => GC9A01 SCL
  ESP32 GPIO23 => GC9A01 SDA
  ESP32 GPIO33 => GC9A01 RES
  ESP32 GPIO27 => GC9A01 DC
  ESP32 GPIO05 => GC9A01 CS
  ESP32 GPIO22 => GC9A01 BLK

  If you are using other wiring or other device ensure you use the right SPI parameters
*/

#define TFT_CS 5
#define TFT_DC 27
#define TFT_RST 33
#define TFT_BL 22

#define TIMECIRCLECOLOR WHITE
#define TIMETICKTHICKINDICATORCOLOR WHITE
#define TIMETICKTHININDICATORCOLOR WHITE
#define HOURCURSORCOLOR RED
#define MINUTECURSORCOLOR YELLOW
#define SECONDCURSORCOLOR BLUE
#define DAYINDICATORTEXTCOLOR GREEN

//round display resolution = 240x240 
#define CENTERPOSITION_X 120
#define CENTERPOSITION_Y 120

// ESP32 hardware SPI, more customizable parameters
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);
 
// GC9A01 IPS LCD 240x240
Arduino_GC9A01 *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

int timezone = 1 * 3600; //GMT+1 if you are in a different time zone adapt accordingly
int dst = 0; //Wintertime
float anglemin=-1.0;
float anglehour=-1.0;

float ConvertDegreesToRadians(float angle)
{
  return angle/57.29577951;
}

void setup(void)
{
    gfx->begin();
    //clearscreen
    gfx->fillScreen(BLACK);

    Serial.begin(115200);
 
    gfx->setTextSize(1);
    gfx->setTextColor(WHITE);
 
    gfx->setCursor(50,60);
    gfx->println("Wifi connecting to ");
    gfx->setCursor(50,70);
    gfx->println( ssid );
 
    WiFi.begin(ssid,password);
    gfx->setCursor(50,80);
    gfx->println("Connecting");

    while( WiFi.status() != WL_CONNECTED )
    {
      delay(500);
      gfx->print(".");
      
    }
    
    gfx->fillScreen(BLACK);
    gfx->setCursor(50,60);
    gfx->println("Wifi Connected!");
    gfx->setCursor(50,70);
    gfx->print("IP:");
    gfx->setCursor(50,80);
    gfx->println(WiFi.localIP() );

 
    configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
    gfx->setCursor(50,90);
    gfx->println("Waiting for NTP...");
 
    while(!time(nullptr))
    {
      Serial.print("*");
      delay(1000);
    }
    
    gfx->setCursor(50,100);
    gfx->println("Time response....OK");
    delay(1000);

    //clearscreen
    gfx->fillScreen(BLACK);
}

void loop()
{
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    int r = 120;

    //draw the mid circle
    gfx->drawCircle(120, 120, 3, TIMECIRCLECOLOR);

    
    //draw the big time tick indicators
    for( int z=0; z < 360;z= z + 30 )
    {
      //Begin at 0° and stop at 360°
      float angle = z;
 
      angle=(angle/57.29577951) ; //Convert degrees to radians
      int x2=(120+(sin(angle)*r));
      int y2=(120-(cos(angle)*r));
      int x3=(120+(sin(angle)*(r-10)));
      int y3=(120-(cos(angle)*(r-10)));
      gfx->drawLine(x2,y2,x3,y3,TIMETICKTHICKINDICATORCOLOR);

    }

    //draw the thin time tick indicators
    for( int z=0; z < 360;z= z + 6 )
    {
      //Begin at 0° and stop at 360°
      float angle = z;
 
      angle=(angle/57.29577951) ; //Convert degrees to radians
      int x2=(120+(sin(angle)*r));
      int y2=(120-(cos(angle)*r));
      int x3=(120+(sin(angle)*(r-5)));
      int y3=(120-(cos(angle)*(r-5)));
      gfx->drawLine(x2,y2,x3,y3,TIMETICKTHININDICATORCOLOR);

    }
    // display second hand
    float anglesec = p_tm->tm_sec*6 ;
    anglesec=ConvertDegreesToRadians(anglesec);
    int x3=(120+(sin(anglesec)*(r-5)));
    int y3=(120-(cos(anglesec)*(r-5)));
    gfx->drawLine(120,120,x3,y3,SECONDCURSORCOLOR);

    float beforeanglesec = (p_tm->tm_sec-1)*6 ;
    beforeanglesec=ConvertDegreesToRadians(beforeanglesec);
    x3=(120+(sin(beforeanglesec)*(r-5)));
    y3=(120-(cos(beforeanglesec)*(r-5)));
    gfx->drawLine(120,120,x3,y3,BLACK);
    
    
    // **** display minute cursor ***
    
    if(p_tm->tm_sec==0) 
    { 
      if(anglemin!=-1.0)
      {//remove old hour indicator by setting to the same color than the screen
          x3=(120+(sin(anglemin)*(r-9)));
          y3=(120-(cos(anglemin)*(r-9)));
          gfx->drawLine(120,120,x3,y3,BLACK);
      }
    }
    
    anglemin = p_tm->tm_min * 6 ;
    anglemin=ConvertDegreesToRadians(anglemin);
    x3=(120+(sin(anglemin)*(r-9)));
    y3=(120-(cos(anglemin)*(r-9)));
    gfx->drawLine(120,120,x3,y3,MINUTECURSORCOLOR);

    Serial.println(p_tm->tm_sec);
    
    // **** display hour cursor ***

    if(p_tm->tm_min==0) 
    { 
        if(anglehour!=-1.0)
        {//remove old hour indicator by setting to the same color than the screen
            x3=(120+(sin(anglehour)*(r-75)));
            y3=(120-(cos(anglehour)*(r-75)));
            gfx->drawLine(120,120,x3,y3,BLACK);
        }
    }
    
    anglehour = p_tm->tm_hour * 30 + int((p_tm->tm_min / 12) * 6 );
    anglehour=ConvertDegreesToRadians(anglehour);
    x3=(120+(sin(anglehour)*(r-35)));
    y3=(120-(cos(anglehour)*(r-35)));
    gfx->drawLine(120,120,x3,y3,HOURCURSORCOLOR);

    gfx->setTextColor(DAYINDICATORTEXTCOLOR);
    gfx->setTextSize(1);
    gfx->setCursor(120+90,120 - 3);
    gfx->print(p_tm->tm_mday);
 
    delay(100);
}



