#define DCSBIOS_DEFAULT_SERIAL
#define DCSBIOS_DISABLE_SERVO
#include <DcsBios.h>
#include <TFT_eSPI.h> 
#include "dcs_callbacks_definition.h"
#define  MODE_BUTTON_PIN  32
#define  QTY_BUTTON_PIN  33
#define  UP_ARROW_BUTTON_PIN 27
#define  DOWN_ARROW_BUTTON_PIN  26
#define  ET_BUTTON_PIN  25


TFT_eSPI tft = TFT_eSPI();    

int stroke = 20;

String left_tank_fuel = "0";
String right_tank_fuel = "0";
String bingo_fuel = "0";

String time_h = "00";
String time_m = "00";
String time_s = "00";

String clock_h = "00";
String clock_m = "00";
String clock_s = "00";


DcsBios::StringBuffer<6> ifeiFuelUpBuffer(0x7490, onIfeiFuelUpChange);
DcsBios::StringBuffer<6> ifeiFuelDownBuffer(0x748a, onIfeiFuelDownChange);
DcsBios::StringBuffer<5> ifeiBingoBuffer(0x7468, onIfeiBingoChange);

DcsBios::StringBuffer<2> ifeiTimerSBuffer(0x7478, onIfeiTimerSChange);
DcsBios::StringBuffer<2> ifeiTimerMBuffer(0x7476, onIfeiTimerMChange);
DcsBios::StringBuffer<2> ifeiTimerHBuffer(0x7474, onIfeiTimerHChange);

DcsBios::StringBuffer<2> ifeiClockHBuffer(0x746e, onIfeiClockHChange);
DcsBios::StringBuffer<2> ifeiClockMBuffer(0x7470, onIfeiClockMChange);
DcsBios::StringBuffer<2> ifeiClockSBuffer(0x7472, onIfeiClockSChange);

DcsBios::Switch2Pos ifeiUpBtn("IFEI_UP_BTN", UP_ARROW_BUTTON_PIN);
DcsBios::Switch2Pos ifeiDwnBtn("IFEI_DWN_BTN", DOWN_ARROW_BUTTON_PIN);
DcsBios::Switch2Pos ifeiQtyBtn("IFEI_QTY_BTN", QTY_BUTTON_PIN);
DcsBios::Switch2Pos ifeiModeBtn("IFEI_MODE_BTN", MODE_BUTTON_PIN);
DcsBios::Switch2Pos ifeiEtBtn("IFEI_ET_BTN", ET_BUTTON_PIN);

void setup() {
  DcsBios::setup();
  tft.init();
  tft.setRotation(0);
  drawFuelInformation();
  drawTimeInformation();
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(QTY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(UP_ARROW_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_ARROW_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ET_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  DcsBios::loop();
}
