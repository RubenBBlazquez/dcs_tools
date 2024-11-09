#define DCSBIOS_DEFAULT_SERIAL
#define DCSBIOS_DISABLE_SERVO
#include <DcsBios.h>
#include <Wire.h>
#include <SPI.h>
#include <PCA9557.h>
#include "gfx_conf.h"
#include "fonts/Calculator.h"

PCA9557 Out;
LGFX tft;

// Dimensions for each rectangle in pixels
const int rectWidth = 245;   // Width in pixels for each rectangle
const int rectHeight = 106;  // Height in pixels for each rectangle
const int gapX = 30;          // Gap between rectangles in pixels (5.5 mm)
const int gapY = 21;          // Gap between rectangles in pixels (5.5 mm)

// Starting coordinates
const int startX = 0; // Adjust as needed for initial offset
const int startY = 0; // Adjust as needed for initial offset

const char* customFontPath = "./fonts/SourceCodePro-Medium.ttf";

// List of specific words to be displayed in each rectangle
String words[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", 
                  "Eta", "Theta", "Iota", "Kappa", "Lambda", "Mu"};



void drawWord(int x, int y, String word, bool active = false) {
  tft.setTextColor(tft.color565(20, 20, 20), TFT_BLACK);
  
  if (active){
    tft.setTextColor(tft.color565(255, 122,26), TFT_BLACK);
  }

  // Set text datum to center
  tft.setTextDatum(MC_DATUM); // Middle center datum

  // Determine the maximum font size that fits within the width of the rectangle
  int maxTextSize = 1.8; // Start with text size 1
  int textWidth = 0;

  // Increase text size until it exceeds the rectangle width with padding
  int padding = 10; // Padding on each side
  do {
    maxTextSize++;
    tft.setTextSize(maxTextSize);
    textWidth = tft.textWidth(word);
  } while (textWidth < rectWidth - 2 * padding && (tft.fontHeight() < rectHeight / 2));

  // Use the last valid text size that fits
  maxTextSize--;
  tft.setTextSize(maxTextSize);

  // Calculate centered position
  int centerX = x + rectWidth / 2;
  int centerY = y + rectHeight / 2;

  // Draw the word centered in the rectangle
  tft.drawString(word, centerX, centerY);
}

void setup()
{
  DcsBios::setup();

  #if defined (CrowPanel_50) || defined (CrowPanel_70)
  // Touch timing initialization
  Wire.begin(19, 20);
  Out.reset();
  Out.setMode(IO_OUTPUT);
  Out.setState(IO0, IO_LOW);
  Out.setState(IO1, IO_LOW);
  delay(20);
  Out.setState(IO0, IO_HIGH);
  delay(100);
  Out.setMode(IO1, IO_INPUT);
  #endif

  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(tft.color565(255, 122,26), TFT_BLACK); // White text with black background

  tft.setFreeFont(&Calculator20pt7b);

  drawWord(startX, startY, "CK SEAT");
  drawWord(startX + rectWidth + gapX, startY, "APU ACC");
  drawWord(startX + 2 * (rectWidth + gapX), startY,  "BATT SW");

  drawWord(startX, startY + rectHeight + gapY, "FCS HOT");
  drawWord(startX + rectWidth + gapX, startY + rectHeight + gapY,  "GEN TIE");
  drawWord(startX + 2 * (rectWidth + gapX), startY + rectHeight + gapY,"----");

  drawWord(startX, startY + 2 * (rectHeight + gapY), "FUEL LO" );
  drawWord(startX + rectWidth + gapX, startY + 2 * (rectHeight + gapY), "FCES");
  drawWord(startX + 2 * (rectWidth + gapX), startY + 2 * (rectHeight + gapY), "----");

  drawWord(startX, startY + 3 * (rectHeight + gapY), "L GEN");
  drawWord(startX + rectWidth + gapX, startY + 3 * (rectHeight + gapY), "R GEN");
  drawWord(startX + 2 * (rectWidth + gapX), startY + 3 * (rectHeight + gapY), "----");
}

void onClipCkSeatLtChange(unsigned int newValue) {
    drawWord(startX, startY, "CK SEAT",  newValue == 1);
}
DcsBios::IntegerBuffer clipCkSeatLtBuffer(0x74a0, 0x8000, 15, onClipCkSeatLtChange);

void onClipApuAccLtChange(unsigned int newValue) {
    drawWord(startX + rectWidth + gapX, startY, "APU ACC", newValue == 1);
}
DcsBios::IntegerBuffer clipApuAccLtBuffer(0x74a4, 0x0100, 8, onClipApuAccLtChange);

void onClipBattSwLtChange(unsigned int newValue) {
    drawWord(startX + 2 * (rectWidth + gapX), startY, "BATT SW",  newValue == 1);
}
DcsBios::IntegerBuffer clipBattSwLtBuffer(0x74a4, 0x0200, 9, onClipBattSwLtChange);

void onClipFcsHotLtChange(unsigned int newValue) {
   drawWord(startX, startY + rectHeight + gapY, "FCS HOT",  newValue == 1);
}
DcsBios::IntegerBuffer clipFcsHotLtBuffer(0x74a4, 0x0400, 10, onClipFcsHotLtChange);

void onClipGenTieLtChange(unsigned int newValue) {
   drawWord(startX + rectWidth + gapX, startY + rectHeight + gapY, "GEN TIE",  newValue == 1); 
}
DcsBios::IntegerBuffer clipGenTieLtBuffer(0x74a4, 0x0800, 11, onClipGenTieLtChange);

void onClipSpareCtn1LtChange(unsigned int newValue) {
     drawWord(startX + 2 * (rectWidth + gapX), startY + rectHeight + gapY, "----", newValue == 1);
}
DcsBios::IntegerBuffer clipSpareCtn1LtBuffer(0x74a4, 0x1000, 12, onClipSpareCtn1LtChange);

void onClipFuelLoLtChange(unsigned int newValue) {
    drawWord(startX, startY + 2 * (rectHeight + gapY), "FUEL LO", newValue == 1); 
}
DcsBios::IntegerBuffer clipFuelLoLtBuffer(0x74a4, 0x2000, 13, onClipFuelLoLtChange);

void onClipFcesLtChange(unsigned int newValue) {
    drawWord(startX + rectWidth + gapX, startY + 2 * (rectHeight + gapY), "FCES", newValue == 1); 
}
DcsBios::IntegerBuffer clipFcesLtBuffer(0x74a4, 0x4000, 14, onClipFcesLtChange);

void onClipSpareCtn2LtChange(unsigned int newValue) {
    drawWord(startX + 2 * (rectWidth + gapX), startY + 2 * (rectHeight + gapY), "----", newValue == 1); 
}
DcsBios::IntegerBuffer clipSpareCtn2LtBuffer(0x74a4, 0x8000, 15, onClipSpareCtn2LtChange);

void onClipLGenLtChange(unsigned int newValue) {
    drawWord(startX, startY + 3 * (rectHeight + gapY), "L GEN", newValue == 1);
}
DcsBios::IntegerBuffer clipLGenLtBuffer(0x74a8, 0x0100, 8, onClipLGenLtChange);

void onClipRGenLtChange(unsigned int newValue) {
    drawWord(startX + rectWidth + gapX, startY + 3 * (rectHeight + gapY),  "R GEN", newValue == 1);
}
DcsBios::IntegerBuffer clipRGenLtBuffer(0x74a8, 0x0200, 9, onClipRGenLtChange);

void onClipSpareCtn3LtChange(unsigned int newValue) {
     drawWord(startX + 2 * (rectWidth + gapX), startY + 3 * (rectHeight + gapY), "----", newValue == 1);
}
DcsBios::IntegerBuffer clipSpareCtn3LtBuffer(0x74a8, 0x0400, 10, onClipSpareCtn3LtChange);


void loop()
{
  DcsBios::loop();
}