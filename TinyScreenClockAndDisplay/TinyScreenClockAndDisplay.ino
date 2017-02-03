#define  BLACK           0x00
#define BLUE            0xE0
#define RED             0x03
#define GREEN           0x1C
#define DGREEN           0x0C
#define YELLOW          0x1F
#define WHITE           0xFF
#define ALPHA           0xFE
#define BROWN           0x32
// Define colors

#include "SSD1331.h"
#include <SPI.h>
#include <Wire.h>
#include "DSRTCLib.h"
// Define libraries

#include <avr/power.h>
#include <avr/sleep.h>
SSD1331 display = SSD1331(0);
int ledPin =  13;
int INT_PIN = 3;
int int_number = 1;
// Defins Pins

DS1339 RTC = DS1339();

int displayState = 0;
int buttonReleased = 0;
int countdown = 0;
int messages = 0;
int menu = 0;
// Defines integers being used


void setup()
{
 start_coreservices();
 start_effects();
 display_battery();
 Serial.begin(9600); // maybe add debug mode to save battery?
}
void loop()
{
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char month;
  unsigned char day;
  unsigned int year;
  unsigned char hr;
  unsigned int weekday;
  char* dayOfWeekName[7];
  display.fontColor(BLACK,BLACK);
  RTC.readTime();
  second = RTC.getSeconds();
  minute = RTC.getMinutes();
  hour = RTC.getHours();        
  day = RTC.getDays();
  month = RTC.getMonths();        
  year = RTC.getYears();
  weekday = RTC.getDayOfWeek();
  hr = RTC.getHours();
  Serial.println(RTC.getDayOfWeek());
  byte buttons = display.getButtons();
  display.setFont(liberationSans_10ptFontInfo);
  display.fontColor(WHITE,BLACK);
  display.setCursor(1,1);

  if (buttons != 0)
  {
    displayState = 1;
    display.on();
    countdown = 90;
    messages++;
  }
  if (buttons == 0) 
  {
    countdown = countdown-1;
  }
  if (countdown<=0)
  {
  display.clearWindow(0,0,0,0);
  displayState = 0;
  display.off();
  }
  
  if (hour > 12) {
  hour = hour - 12;
  }
/*  display.print(hour);
  display.print(":");
  if (minute < 10) {
    display.print("0" + minute);
  }*/
 

// Date  
  display.setFont(liberationSans_10ptFontInfo);
  display.fontColor(WHITE,BLACK);
  display.setCursor(1,1);
  // Months
//  rtc_months();
  //display.print(" ");
  //rtc_days();
}
void start_coreservices()
{
  Wire.begin();
  display.begin();
  display.setFlip(1);
  display.setBrightness(5);
  RTC.start();
}
void display_battery()
{
  const long InternalReferenceVoltage = 1100L;
  ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
  delay(10);
  ADCSRA |= _BV( ADSC );
  while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
  int result = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L;
  result=constrain(result-300,0,120);
  int x=72;
  int y=2;
  int height=8;
  int length=20;
  int amtActive=(result*length)/110;
  int red,green,blue;
  for(int i=0;i<length;i++){
  if (i<amtActive) 
  {
    red=63-((63/length)*i);
    green=((63/length)*i);
    blue=0;
    } 
    else {
      red=32;
      green=32;
      blue=32;
    }
    display.drawLine(x+i,y,x+i,y+height,red,green,blue);
  }
}
void start_effects()
{
  display.fontColor(RED,BLACK);
  display.setFont(liberationSans_16ptFontInfo);
  display.setCursor(0,10);
  display.print("Nitrogen");
  delay(2000);
  displayState = 1;
  display.on();
  countdown = 90;
  display.clearWindow(0,0,0,0);
}
void rtc_months()
{
  // pretty sure i can use a for instance to clean this up
  unsigned char month;
  month = RTC.getMonths();  
  switch (month) {
  case 1:
  display.print("Jan");
  break;
  case 2:
  display.print("Feb");
  break;
  case 3:
  display.print("March");
  break;
  case 4:
  display.print("April");
  break;
  case 5:
  display.print("May");
  break;
  case 6:
  display.print("June");
  break;
  case 7:
  display.print("July");
  break;
  case 8:
  display.print("Aug");
  break;
  case 9:
  display.print("Sept");
  break;
  case 10:
  display.print("Oct");
  break;
  case 11:
  display.print("Nov");
  break;
  case 12:
  display.print("Dec");
  break;
  }
}
void rtc_days()
{  
  unsigned char day;
  day = RTC.getDays();
  switch (day) {
  case 1:
  display.print("1st");
  break;
  case 2:
  display.print("2nd");
  break;
  case 3:
  display.print("3rd");
  break;
  case 21:
  display.print("21st");
  break;
  case 22:
  display.print("22nd");
  break;
  case 23:
  display.print("23rd");
  break;
  case 31:
  display.print("31st");
  break;
  }
  if ((day >= 24) && (day <= 30) || (day >=4) && (day<=20))
  {
  display.print(day);
  display.print("th");
  }
}
  void rtc_ampm()
  {
  unsigned char hr;
  hr = RTC.getHours();
  display.setFont(liberationSans_12ptFontInfo);
  if (hr >= 12)
  {
  display.print("PM");
  } else {
  display.print("AM");
  }
}

