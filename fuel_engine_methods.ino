#include "fonts/YanoneKaffeesatzFont.h"
#include "fonts/YanoneKaffeesatzFont_15pt.h"
#include "fonts/Calculator.h"

extern TFT_eSPI tft;
extern String left_tank_fuel;
extern String right_tank_fuel;
extern String bingo_fuel;
extern String rightRpm;
extern String leftRpm;
extern boolean is_aoe_indexer_active;
uint16_t black = tft.color565(0, 0, 0);
uint16_t maroon = tft.color565(46, 41, 32);

void drawFuelInformation(){
  tft.fillScreen(maroon);  
  tft.setFreeFont(&YanoneKaffeesatz_Regular20pt7b);
  tft.setCursor(100, 30); // Set the cursor to the top-left corner
  tft.setTextColor(tft.color565(255, 255, 255)); // Set the text color to white
  tft.setTextSize(1); // Set the text size to 1
  
  tft.fillRect(0, 0, tft.width(), 40, black);
  tft.setCursor(135, 30); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setTextSize(1); // Text size
  tft.print("FUEL");

  tft.fillRect(0, 40, tft.width(), 300, black);
  tft.fillRoundRect(21, 45, tft.width()-40, 290, 4, maroon);

  tft.fillRoundRect(26, 55, tft.width()-50, 60, 8, black);
  tft.setCursor(45, 95); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print("T - " + left_tank_fuel);

  tft.fillRoundRect(26, 125, tft.width()-50, 60, 8, black);
  tft.setCursor(45, 165); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print("I - " + right_tank_fuel);

  tft.fillRoundRect(87, 200, tft.width()-170, 50, 8, black);
  tft.setCursor(135, 235); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&YanoneKaffeesatz_Regular15pt7b);
  tft.setTextSize(1); // Text size
  tft.print("BINGO");

  tft.fillRoundRect(45, 255, tft.width()-80, 60, 8, black);
  tft.setCursor(60, 295); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print(bingo_fuel);
}

void updateRightTankFuel(String newRightFuel){
  if(right_tank_fuel == newRightFuel || is_aoe_indexer_active){
    return;
  }

  tft.fillRoundRect(26, 125, tft.width()-50, 60, 8, black);
  tft.setCursor(45, 165); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print("I - " + newRightFuel);
  right_tank_fuel = newRightFuel;
}

void updateLeftTankFuel(String newLeftFuel){
  if(left_tank_fuel == newLeftFuel || is_aoe_indexer_active){
    return;
  }

  tft.fillRoundRect(26, 55, tft.width()-50, 60, 8, black);
  tft.setCursor(45, 95); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print("T - " + newLeftFuel);

  left_tank_fuel = newLeftFuel;
}

void updatebingoTankFuel(String newBingoFuel){
  if(bingo_fuel == newBingoFuel || is_aoe_indexer_active){
    return;
  }

  tft.fillRoundRect(45, 255, tft.width()-80, 60, 8, black);
  tft.setCursor(60, 295); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print(newBingoFuel);

  bingo_fuel = newBingoFuel;
}


void drawRpmInformation(){
  tft.fillRect(0, 340, tft.width(), 140, black);
  tft.setFreeFont(&YanoneKaffeesatz_Regular20pt7b);
  tft.setCursor(135, 380); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setTextSize(1); // Text size
  tft.print("RPM");

  tft.fillRoundRect(21, 400, tft.width()-40, 70, 4,  maroon);

  tft.fillRoundRect(28, 408, tft.width()-190, 55, 8, black);
  tft.setCursor(45, 450); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print(leftRpm);

  tft.fillRoundRect(165, 408, tft.width()-190, 55, 8, black);
  tft.setCursor(180, 450); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print(rightRpm);
}

void updateLeftRpm(String newLeftRpm){
  if(newLeftRpm == leftRpm || is_aoe_indexer_active){
    return;
  }

  tft.fillRoundRect(165, 408, tft.width()-190, 55, 8, black);
  tft.setCursor(180, 450); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print(newLeftRpm);
  leftRpm = newLeftRpm;
}

void updateRightRpm(String newRightRpm){
  if(newRightRpm == rightRpm || is_aoe_indexer_active){
    return;
  }
  
  tft.fillRoundRect(28, 408, tft.width()-190, 55, 8, black);
  tft.setCursor(45, 450); // Position the cursor inside the rectangle with padding
  tft.setTextColor(tft.color565(255, 255, 255)); // White text color
  tft.setFreeFont(&Calculator20pt7b);
  tft.setTextSize(1); // Text size
  tft.print(newRightRpm);

  rightRpm = newRightRpm;
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

void onIfeiRpmLChange(char* newValue) {
   updateRightRpm(newValue);
}

void onIfeiRpmRChange(char* newValue) {
    updateLeftRpm(newValue);
}
