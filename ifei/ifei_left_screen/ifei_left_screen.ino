#define DCSBIOS_DEFAULT_SERIAL
#define DCSBIOS_DISABLE_SERVO
#include <DcsBios.h>
#include <TFT_eSPI.h> 
#include "fonts/Calculator.h"
#include "fonts/YanoneKaffeesatzFont_15pt.h"

TFT_eSPI tft = TFT_eSPI();    

int stroke = 20;
static int lastLeftValue = -1; 
static int lastRightValue = -1;
uint16_t black = tft.color565(0, 0, 0);
uint16_t maroon = tft.color565(46, 41, 32);
int largerRadius = 130; 
int topGaugeY = 230;    

// Row positions (vertical alignment)
int row1Y = 5;        // Y position for the first row (RPM)
int row2Y = 68;       // Y position for the second row (TEMP)
int row3Y = 130;       // Y position for the third row (FF x100)
int row4Y = 420;       // Y position for the third row (FF x100)
int boxWidth = 90;     // Width of each value box
int boxHeight = 50;    // Height of each value box
int value1X = 15;   
int colNameX = 115; 
int value2X = 225; 

String left_tank_fuel = "0";
String right_tank_fuel = "0";
String bingo_fuel = "0";
String time_clock = "00 :00 :00";
String chrono_clock = "00 :00 :00";

const byte colorDepth = 8; // Increasing this beyond 8 may cause graphic image issues

void drawLeftRPM(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

  // Row 1 (RPM)
  tft.fillRoundRect(value1X , row1Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(value1X+10, row1Y + 35);
  tft.print(String(value)); 
}

void drawRightRPM(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

  // Row 1 (RPM)
  tft.fillRoundRect(value2X - 10, row1Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(value2X, row1Y + 35);
  tft.print(String(value)); 
}

void drawLeftTemp(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

 // Row 2 (TEMP)
  tft.fillRoundRect(value1X , row2Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(value1X+10, row2Y + 35);
  tft.print(String(value)); 
}
void drawRightTemp(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

  tft.fillRoundRect(value2X - 10, row2Y, boxWidth, boxHeight, 8, black); 
  tft.setCursor(value2X, row2Y + 35);
  tft.print(value);
}

void drawLeftFF(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

 // Row 2 (TEMP)
 tft.fillRoundRect(value1X , row3Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(value1X+10, row3Y + 35);
  tft.print(value); 
}

void drawRightFF(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

  tft.fillRoundRect(value2X - 10, row3Y, boxWidth, boxHeight, 8, black); 
  tft.setCursor(value2X, row3Y + 35);
  tft.print(value);
}

void drawLeftOIL(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

  // Row 2 (TEMP)
  tft.fillRoundRect(value1X , row4Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(value1X+10, row4Y + 35);
  tft.print(value);  
}

void drawRightOIL(char* value = "0"){
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

  tft.fillRoundRect(value2X - 10, row4Y, boxWidth, boxHeight, 8, black); 
  tft.setCursor(value2X, row4Y + 35);
  tft.print(value);
}

void drawDashboard() {
  tft.fillScreen(maroon); // Clear the screen with a maroon background

  // Text settings
  tft.setTextColor(tft.color565(255, 255, 255));
  tft.setFreeFont(&Calculator20pt7b); 

  drawLeftRPM();
  tft.fillRoundRect(colNameX , row1Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(colNameX + 25,  row1Y + 33);
  tft.setFreeFont(&YanoneKaffeesatz_Regular15pt7b);
  tft.print("RPM");
  drawRightRPM();

  
  drawLeftTemp();
  tft.fillRoundRect(colNameX , row2Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(colNameX + 25, row2Y + 33); 
  tft.setFreeFont(&YanoneKaffeesatz_Regular15pt7b);
  tft.print("TEMP");
  drawRightTemp();


  drawLeftFF(); 
  tft.fillRoundRect(colNameX , row3Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(colNameX + 10, row3Y + 33); 
  tft.setFreeFont(&YanoneKaffeesatz_Regular15pt7b);
  tft.print("FF x100");
  drawRightFF();

  // Row 4 (NOZ)
  tft.fillRoundRect(colNameX+ 10 , 320, 70, 50, 8, black);
  tft.setCursor(colNameX + 27, 354); 
  tft.setFreeFont(&YanoneKaffeesatz_Regular15pt7b);
  tft.print("NOZ");

  // Row 5 (OIL)

  drawLeftOIL(); 
  tft.fillRoundRect(colNameX , row4Y, boxWidth, boxHeight, 8, black);
  tft.setCursor(colNameX + 25, row4Y + 33); 
  tft.setFreeFont(&YanoneKaffeesatz_Regular15pt7b);
  tft.print("OIL");
  drawRightOIL();

}

int roundToNearest5(char value) {
  int remainder = value % 5;

  if (remainder < 3) {
    return value - remainder;  // Round down
  }

   return value + (5 - remainder);  // Round up
}

void drawGaugeBase(int centerX, int centerY, int radius, int startAngle, int endAngle, bool reverseNumbers) {
  for (int angle = startAngle; angle <= endAngle; angle += 5) {  // Ticks every 10 degrees
    float rad = radians(angle);
    int xStart, yStart, xEnd, yEnd;
    int value = map(angle, startAngle, endAngle, reverseNumbers ? 0 : 100, reverseNumbers ? 100 : 0);
    value = roundToNearest5(value);

    // Draw tick marks
    xStart = centerX + cos(rad) * (radius - 15);  // Default tick start
    yStart = centerY + sin(rad) * (radius - 15);

    if (value == 0 || value == 50 || value == 100) {
      xStart = centerX + cos(rad) * (radius - 30);  // Start position further inward
      yStart = centerY + sin(rad) * (radius - 30);
    }

    xEnd = centerX + cos(rad) * radius;
    yEnd = centerY + sin(rad) * radius;

    // Draw the tick
    tft.drawLine(xStart, yStart, xEnd, yEnd, TFT_WHITE);

    // Draw the value labels (0, 50, 100)
    if (value == 0 || value == 50 || value == 100) {
      int xText = centerX + cos(rad) * (radius - 45);  // Position the text slightly closer
      int yText = centerY + sin(rad) * (radius - 45);
      tft.setTextDatum(MC_DATUM);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString(String(value), xText, yText, 2);
    }
  }
}

void drawNeedle(int value, int centerX, int centerY, int radius, int startAngle, int endAngle, bool reverseNumbers, int &lastValue) {
  // Skip drawing if the value hasn't changed
  if (abs(value - lastValue) <= 5) {
    return;
  }

  int needleLength = radius - 20;  // Total length of the needle
  int needleBaseWidth = 10;       // Width of the rectangular body
  int needleTipLength = 20;       // Length of the triangular tip

  // Helper function to draw or erase the needle
  auto drawOrEraseNeedle = [&](int needleValue, uint16_t color) {
    float normalizedValue = reverseNumbers
                              ? (needleValue / 100.0)  // Normalized for reverse
                              : (1.0 - (needleValue / 100.0));  // Normalized for normal
    float angle = startAngle + normalizedValue * (endAngle - startAngle);  // Calculate the angle
    float rad = radians(angle);

    // Calculate the base points of the rectangular body
    int xBodyStart = centerX + cos(rad) * (needleLength - needleTipLength);
    int yBodyStart = centerY + sin(rad) * (needleLength - needleTipLength);

    int xBodyEnd1 = xBodyStart + cos(rad + radians(90)) * (needleBaseWidth / 2);
    int yBodyEnd1 = yBodyStart + sin(rad + radians(90)) * (needleBaseWidth / 2);
    int xBodyEnd2 = xBodyStart + cos(rad - radians(90)) * (needleBaseWidth / 2);
    int yBodyEnd2 = yBodyStart + sin(rad - radians(90)) * (needleBaseWidth / 2);

    // Calculate the tip point of the needle
    int xTip = centerX + cos(rad) * needleLength;
    int yTip = centerY + sin(rad) * needleLength;

    // Draw or erase the rectangular body
    tft.fillTriangle(centerX, centerY, xBodyEnd1, yBodyEnd1, xBodyEnd2, yBodyEnd2, color);

    // Draw or erase the triangular tip
    tft.fillTriangle(xTip, yTip, xBodyEnd1, yBodyEnd1, xBodyEnd2, yBodyEnd2, color);
  };

  drawOrEraseNeedle(lastValue, maroon);
  drawOrEraseNeedle(value, TFT_WHITE);

  // Update the lastValue to the current value
  lastValue = value;
}

void demoNozzlePositions(){
  
  for (int angle = 0; angle <= 100; angle += 5) {  // Ticks every 10 degrees 
    drawGaugeBase(20, topGaugeY, largerRadius, 0, 90, true); 
    drawNeedle(angle, 20, topGaugeY, largerRadius, 0, 90, true, lastLeftValue);
    drawGaugeBase(300, topGaugeY, largerRadius, 90, 180, false); 
    drawNeedle(angle, 300, topGaugeY, largerRadius, 90, 180, false, lastRightValue);
    delay(10);  // Adjust for speed
  }

    for (int angle = 100; angle >= 0; angle -= 5) {  // Ticks every 10 degrees 
      drawGaugeBase(20, topGaugeY, largerRadius, 0, 90, true); 
      drawNeedle(angle, 20, topGaugeY, largerRadius, 0, 90, true, lastLeftValue);
      drawGaugeBase(300, topGaugeY, largerRadius, 90, 180, false); 
      drawNeedle(angle, 300, topGaugeY, largerRadius, 90, 180, false, lastRightValue);
      delay(10);  // Adjust for speed
  }


  drawNeedle(30, 20, topGaugeY, largerRadius, 0, 90, true, lastLeftValue);
  drawNeedle(30, 300, topGaugeY, largerRadius, 90, 180, false, lastRightValue);
  drawGaugeBase(300, topGaugeY, largerRadius, 90, 180, false); 
  drawGaugeBase(20, topGaugeY, largerRadius, 0, 90, true); 

}

void onExtNozzlePosRChange(unsigned int newValue) {
  int value = map(newValue, 0, 65535, 0, 100);
  drawNeedle(value, 300, topGaugeY, largerRadius, 90, 180, false, lastRightValue);
  drawGaugeBase(300, topGaugeY, largerRadius, 90, 180, false); 
}
DcsBios::IntegerBuffer extNozzlePosRBuffer(0x7578, 0xffff, 0, onExtNozzlePosRChange);

void onExtNozzlePosLChange(unsigned int newValue) {
  int value = map(newValue, 0, 65535, 0, 100);
  drawNeedle(value, 20, topGaugeY, largerRadius, 0, 90, true, lastLeftValue);
  drawGaugeBase(20, topGaugeY, largerRadius, 0, 90, true); 
}
DcsBios::IntegerBuffer extNozzlePosLBuffer(0x757a, 0xffff, 0, onExtNozzlePosLChange);

void onIfeiRpmLChange(char* newValue) {
  drawLeftRPM(newValue);
}
DcsBios::StringBuffer<3> ifeiRpmLBuffer(0x749e, onIfeiRpmLChange);

void onIfeiRpmRChange(char* newValue) {
  drawRightRPM(newValue);
}
DcsBios::StringBuffer<3> ifeiRpmRBuffer(0x74a2, onIfeiRpmRChange);

void onIfeiTempLChange(char* newValue) {
  drawLeftTemp(newValue);
}
DcsBios::StringBuffer<3> ifeiTempLBuffer(0x74a6, onIfeiTempLChange);

void onIfeiTempRChange(char* newValue) {
  drawRightTemp(newValue);
}
DcsBios::StringBuffer<3> ifeiTempRBuffer(0x74aa, onIfeiTempRChange);

void onIfeiFfLChange(char* newValue) {
    drawLeftFF(newValue);
}
DcsBios::StringBuffer<3> ifeiFfLBuffer(0x7482, onIfeiFfLChange);

void onIfeiFfRChange(char* newValue) {
    drawRightFF(newValue);
}
DcsBios::StringBuffer<3> ifeiFfRBuffer(0x7486, onIfeiFfRChange);

void onIfeiOilPressLChange(char* newValue) {
    drawLeftOIL(newValue);
}
DcsBios::StringBuffer<3> ifeiOilPressLBuffer(0x7496, onIfeiOilPressLChange);

void onIfeiOilPressRChange(char* newValue) {
    drawRightOIL(newValue);
}
DcsBios::StringBuffer<3> ifeiOilPressRBuffer(0x749a, onIfeiOilPressRChange);

void setup() {
  DcsBios::setup();
  tft.begin();
  tft.setRotation(0); 
  drawDashboard();  

  drawNeedle(30, 20, topGaugeY, largerRadius, 0, 90, true, lastLeftValue);
  drawGaugeBase(20, topGaugeY, largerRadius, 0, 90, true); 

  drawNeedle(30, 300, topGaugeY, largerRadius, 90, 180, false, lastRightValue);
  drawGaugeBase(300, topGaugeY, largerRadius, 90, 180, false); 

  demoNozzlePositions();
}

void loop() {
    DcsBios::loop();
}

