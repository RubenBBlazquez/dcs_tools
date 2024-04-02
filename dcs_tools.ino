#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <SPI.h>
#include <TFT_eSPI.h> 
#include "dcs_callbacks_definition.h"

TFT_eSPI tft = TFT_eSPI();    

int stroke = 20;
int savedIsAoeIndexerActive = -1;
boolean is_aoe_indexer_active = true;

String left_tank_fuel = "0";
String right_tank_fuel = "0";
String bingo_fuel = "0";
String rightRpm = "0";
String leftRpm = "0";

uint16_t green = tft.color565(0, 255, 0);
uint16_t yellow = tft.color565(255, 255, 0);
uint16_t red = tft.color565(255, 0, 0);

const int SHAPE_WIDTH = 130;
const int V_HEIGHT = 120; 
const int O_RADIUS = 45;
const int O_RADIUS_INNER = 35; 
const int A_HEIGHT = 80;

// aoe indexer coordinates from V O V(inverted)
int v_x1 = 160 - SHAPE_WIDTH / 2;
int v_y1 = 20;
int v_x2 = 160 + SHAPE_WIDTH / 2;
int v_y2 = v_y1;
int v_x_mid = 160;
int v_y_bottom = v_y1 + V_HEIGHT;

int o_x_centre = 160;
int o_y_centre = 240;

int v2_x1 = 160 - SHAPE_WIDTH / 2;
int v2_y1 = 450;
int v2_x2 = 160 + SHAPE_WIDTH / 2;
int v2_y2 = v2_y1;
int v2_x_mid = 160;
int v2_y_bottom = v2_y1 - V_HEIGHT; 

DcsBios::IntegerBuffer aoaIndexerHighBuffer(0x7408, 0x0008, 3, onAoaIndexerHighChange);
DcsBios::IntegerBuffer aoaIndexerLowBuffer(0x7408, 0x0020, 5, onAoaIndexerLowChange);
DcsBios::IntegerBuffer aoaIndexerNormalBuffer(0x7408, 0x0010, 4, onAoaIndexerNormalChange);

DcsBios::StringBuffer<6> ifeiFuelUpBuffer(0x7490, onIfeiFuelUpChange);
DcsBios::StringBuffer<6> ifeiFuelDownBuffer(0x748a, onIfeiFuelDownChange);
DcsBios::StringBuffer<5> ifeiBingoBuffer(0x7468, onIfeiBingoChange);

DcsBios::StringBuffer<3> ifeiRpmRBuffer(0x74a2, onIfeiRpmRChange);
DcsBios::StringBuffer<3> ifeiRpmLBuffer(0x749e, onIfeiRpmLChange);

void onGearLeverChange(unsigned int newValue) {
    // 0 is gear down 1 is up, so if is 0 the aoe_indexer must be active for landing
    is_aoe_indexer_active = newValue == 0;  
}

DcsBios::IntegerBuffer gearLeverBuffer(0x747e, 0x1000, 12, onGearLeverChange);

void setup() {
  DcsBios::setup();
  tft.init();
  tft.setRotation(0);
  pinMode(10, INPUT);
}

void loop() {
  DcsBios::loop();

  if(is_aoe_indexer_active == savedIsAoeIndexerActive){
    return;
  }

  savedIsAoeIndexerActive = is_aoe_indexer_active;

  if (is_aoe_indexer_active){
    drawAOEIndexerPositions();
    return;
  }

  drawFuelInformation();
  drawRpmInformation();
}
