#include "fonts/YanoneKaffeesatzFont.h"
#include "fonts/YanoneKaffeesatzFont_15pt.h"
#include "fonts/Calculator.h"

extern TFT_eSPI tft;
extern String left_tank_fuel;
extern String right_tank_fuel;
extern String bingo_fuel;
extern String time_h;
extern String time_m;
extern String time_s;
extern String clock_h;
extern String clock_m;
extern String clock_s;

uint16_t black = tft.color565(0, 0, 0);
uint16_t maroon = tft.color565(46, 41, 32);



void drawFuelInformation(){
  // Clear the screen and set up background
  tft.fillScreen(maroon);
  tft.setFreeFont(&YanoneKaffeesatz_Regular20pt7b);
  tft.setTextColor(tft.color565(255, 255, 255)); // Set the text color to white
  tft.setTextSize(1); // Set the text size to 1

  // Left tank fuel ("T")
  tft.fillRoundRect(20, 20, tft.width() - 40, 60, 10, black); // Moved up by 10 pixels
  tft.setCursor(30, 60); // Adjusted Y-coordinate for vertical alignment
  tft.setFreeFont(&Calculator20pt7b);
  tft.print("T - " + left_tank_fuel);

  // Right tank fuel ("I")
  tft.fillRoundRect(20, 90, tft.width() - 40, 60, 10, black); // Moved up by 10 pixels
  tft.setCursor(30, 130); // Adjusted Y-coordinate for vertical alignment
  tft.setFreeFont(&Calculator20pt7b);
  tft.print("I - " + right_tank_fuel);

  // "BINGO" block inside the circle
  tft.fillRoundRect(40, 170, tft.width() - 80, 50, 10, black); // Moved up by 10 pixels
  tft.setCursor(130, 205); // Adjusted Y-coordinate for vertical alignment
  tft.setFreeFont(&YanoneKaffeesatz_Regular15pt7b);
  tft.print("BINGO");

  // Bingo fuel display (aligned with "BINGO")
  tft.fillRoundRect(40, 230, tft.width() - 80, 50, 10, black); // Moved up by 10 pixels
  tft.setCursor(50, 265); // Adjusted Y-coordinate for vertical alignment
  tft.setFreeFont(&Calculator20pt7b);
  tft.print(bingo_fuel);

}

void updateRightTankFuel(String newRightFuel){
  if(right_tank_fuel == newRightFuel){
    return;
  }

  // Right tank fuel ("I")
  tft.fillRoundRect(20, 90, tft.width() - 40, 60, 10, black); // Moved up by 10 pixels
  tft.setCursor(30, 130); // Adjusted Y-coordinate for vertical alignment
  tft.setFreeFont(&Calculator20pt7b);

  tft.print("I - " + right_tank_fuel);

  right_tank_fuel = newRightFuel;
}

void updateLeftTankFuel(String newLeftFuel){
  if(left_tank_fuel == newLeftFuel){
    return;
  }

 // Left tank fuel ("T")
  tft.fillRoundRect(20, 20, tft.width() - 40, 60, 10, black); // Moved up by 10 pixels
  tft.setCursor(30, 60); // Adjusted Y-coordinate for vertical alignment
  tft.setFreeFont(&Calculator20pt7b);
  tft.print("T - " + left_tank_fuel);

  left_tank_fuel = newLeftFuel;
}

void updatebingoTankFuel(String newBingoFuel){
  if(bingo_fuel == newBingoFuel){
    return;
  }

  tft.fillRoundRect(40, 230, tft.width() - 80, 50, 10, black); // Moved up by 10 pixels
  tft.setCursor(50, 265); // Adjusted Y-coordinate for vertical alignment
  tft.setFreeFont(&Calculator20pt7b);
  tft.print(bingo_fuel);

  bingo_fuel = newBingoFuel;
}


void drawScaledText(const String &text, int x, int y, float scale, uint16_t color) {
    int spacing = 10 * scale; // Calculate character spacing based on scale
    for (int i = 0; i < text.length(); i++) {
        char c = text.charAt(i);
        int charX = x + i * spacing; // Adjust x-position based on scale
        tft.setCursor(charX, y);    // Set cursor position for each character
        tft.setTextSize(scale);     // Set text size for scaling
        tft.setTextColor(color, black); // White text on black background
        tft.print(c);               // Print the character
  }
}

void drawTimeInformation(){
  String time_clock = clock_h + " :" + clock_m + " :" + clock_s;
  tft.fillRoundRect(28, 375, tft.width()-40, 55, 8, black);
  drawScaledText(time_clock, 80, 415, 1.8, tft.color565(255, 255, 255)); // Draw text at scale 1.5

  String time_timer = time_h + " :" + time_m + " :" + time_s;
  tft.fillRoundRect(33, 435, tft.width()-50, 40, 8, black);
  drawScaledText(time_timer, 100, 465, 1.3, tft.color565(255, 255, 255)); // Draw text at scale 1.5
}

void onIfeiFuelDownChange(char* newValue) {
    updateRightTankFuel(newValue);
}

void onIfeiFuelUpChange(char* newValue) {
    updateLeftTankFuel(newValue);
}

void onIfeiBingoChange(char* newValue) {
    updatebingoTankFuel(newValue);
}

void onIfeiTimerHChange(char* newValue) {
    time_h = newValue;
    drawTimeInformation();
}

void onIfeiTimerMChange(char* newValue) {
    time_m = newValue;
    drawTimeInformation();
}

void onIfeiTimerSChange(char* newValue) {
    time_s = newValue;
    drawTimeInformation();
}

void onIfeiClockHChange(char* newValue) {
    clock_h = newValue;
    drawTimeInformation();
}

void onIfeiClockMChange(char* newValue) {
    clock_m = newValue;
    drawTimeInformation();
}

void onIfeiClockSChange(char* newValue) {
    clock_s = newValue;
    drawTimeInformation();
}