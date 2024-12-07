/* 
Create F-18 multiple (three) display screens (Battery, Hydraulic Pressure & Brake Pressure) on  3 * GC9A01 round 1.28 inch TFT displays driven by ESP32 development board by Tanarg
Gauge consists of multiple background images and a needles

Sketch uses the TFT_eSPI library, so you will need this loaded and configured, in partular ensure that the display, and pin out, is configured in User_Setup.h
*/
const byte colorDepth = 8; // Increasing this beyond 8 may cause graphic image issues

#include "pressureAltBackground.h" 
#include "airSpeedBackground.h" 
#include "vsiBackground.h" 
#include "gaugeNeedle.h" 

#define DCSBIOS_DEFAULT_SERIAL
#define DCSBIOS_DISABLE_SERVO

#define hydPressDisplay 21
#define airSpeedDisplay 22
#define vsiDisplay 5
#define adiPotentiometer 34
#define rwrPotentiometer 27
#define saiCageSwitch 33
#define rightSwitch_1 26
#define rightSwitch_2 25

#include "DcsBios.h"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

// Pressure Altitude Gauge
TFT_eSprite pressureAltGaugeBack = TFT_eSprite(&tft); // Sprite object for background
TFT_eSprite needlePressureAlt = TFT_eSprite(&tft);   // Sprite object for needlePressureAlt

//Brake Pressure Gauge
TFT_eSprite brakePressGaugeBack = TFT_eSprite(&tft); // Sprite object for background
TFT_eSprite needleBrakePress = TFT_eSprite(&tft); // Sprite object for needle

TFT_eSprite backgroundRadarAlt = TFT_eSprite(&tft); // Sprite object for background
TFT_eSprite needleRadarAlt = TFT_eSprite(&tft); // Sprite object for needle
TFT_eSprite minHeightPointer = TFT_eSprite(&tft); // Sprite object for min height pointer
TFT_eSprite lowAltWarning = TFT_eSprite(&tft); //Sprite object for low altitude warning
TFT_eSprite greenLamp = TFT_eSprite(&tft); //Sprite object for green lamp
TFT_eSprite offFlag = TFT_eSprite(&tft); //Sprite object for on/off indicator (+5000 ft on, - 5000 off)

int needleStep = 5;
int currentAltAngle = 0;
int currentVsiAngle = 0;
int currentAirSpeedGaugeAngle = 0;
int angleMapBrake = 0; 
bool scriptInit = false;

#define CUSTOM_COLOR 0x4A2A

void onStbyAsiAirspeedChange(unsigned int newValue) {
  int targetAngle = map (newValue, 0, 65535, 0, 340);

  while (currentAirSpeedGaugeAngle != targetAngle) {
    if (abs(targetAngle - currentAirSpeedGaugeAngle) < needleStep) {
      currentAirSpeedGaugeAngle = targetAngle;
    } else if (targetAngle > currentAirSpeedGaugeAngle) {
      currentAirSpeedGaugeAngle += needleStep;
    } else {
      currentAirSpeedGaugeAngle -= needleStep;
    }

    plotAirSpeedGauge(currentAirSpeedGaugeAngle);
  }

}

DcsBios::IntegerBuffer stbyAsiAirspeedBuffer(0x74f0, 0xffff, 0, onStbyAsiAirspeedChange);

void onVsiChange(unsigned int newValue) {
  int targetAngle =  map (newValue, 0, 65535,  -270, 90);

  while (currentVsiAngle != targetAngle) {
    if (abs(targetAngle - currentVsiAngle) < needleStep) {
      currentVsiAngle = targetAngle;
    } else if (targetAngle > currentVsiAngle) {
      currentVsiAngle += needleStep;
    } else {
      currentVsiAngle -= needleStep;
    }

    plotVsiGauge(currentVsiAngle);
  }
}
DcsBios::IntegerBuffer vsiBuffer(0x7500, 0xffff, 0, onVsiChange);

int alt1000feetNumber = 0;
int alt10000feetNumber = 0;

void onStbyAlt100FtPtrChange(unsigned int newValue) {
  int targetAngle = map (newValue, 0, 65535, 0, 360); // 0-65535 output value from DCS. See bitTest comments for further details

  while (currentAltAngle != targetAngle) {
    if (abs(targetAngle - currentAltAngle) < needleStep) {
      currentAltAngle = targetAngle;
    } else if (targetAngle > currentAltAngle) {
      currentAltAngle += needleStep;
    } else {
      currentAltAngle -= needleStep;
    }

    plotAltPressureGauge(currentAltAngle);
  }
}
DcsBios::IntegerBuffer stbyAlt100FtPtrBuffer(0x74f4, 0xffff, 0, onStbyAlt100FtPtrChange);

void onStbyAlt1000FtCntChange(unsigned int newValue) {
    alt1000feetNumber  = int((newValue / 65535.0) * 10);               // Set text size
    plotAltPressureGauge (currentAltAngle);
}
DcsBios::IntegerBuffer stbyAlt1000FtCntBuffer(0x74f8, 0xffff, 0, onStbyAlt1000FtCntChange);

void onStbyAlt10000FtCntChange(unsigned int newValue) {
    alt10000feetNumber = int((newValue / 65535.0) * 10);
    plotAltPressureGauge (currentAltAngle);
}
DcsBios::IntegerBuffer stbyAlt10000FtCntBuffer(0x74f6, 0xffff, 0, onStbyAlt10000FtCntChange);
DcsBios::Potentiometer rwrRwrIntesity("RWR_RWR_INTESITY",rwrPotentiometer, false);
DcsBios::RotaryEncoder saiSetUp("SAI_SET", "+1600", "+1600", rightSwitch_1, -1);
DcsBios::RotaryEncoder saiSetDown("SAI_SET", "-1600", "-1600", rightSwitch_2, -1);
DcsBios::Switch2Pos ejectionSeatArmed("EJECTION_SEAT_ARMED", saiCageSwitch);
DcsBios::Potentiometer saiSet("SAI_SET_POTENTIOMETER", adiPotentiometer);


void setup() {
  DcsBios::setup();
  Serial.begin(9600); 
  pinMode (hydPressDisplay, OUTPUT);
  pinMode (airSpeedDisplay, OUTPUT);
  pinMode (vsiDisplay, OUTPUT);
  pinMode (saiCageSwitch, INPUT_PULLUP);
  pinMode (rightSwitch_1, INPUT_PULLUP);
  pinMode (rightSwitch_2, INPUT_PULLUP);

  ScreensOn ();
    tft.init ();
    tft.fillScreen (TFT_BLACK);
  ScreensOff ();

  ScreenOn (hydPressDisplay);
    pressureAltGaugeBack.setSwapBytes (true);  // Depending on Sprite image graphics byte order these may, or may not, be required
    needlePressureAlt.setSwapBytes (true);
  ScreenOff (hydPressDisplay);

  ScreenOn (airSpeedDisplay);
    brakePressGaugeBack.setSwapBytes (true); // Depending on Sprite graphics byte order these may, or may not, be required
    needleBrakePress.setSwapBytes (true); 
  ScreenOff (airSpeedDisplay);

  ScreenOn (vsiDisplay);
    backgroundRadarAlt.setSwapBytes (true); // Depending on Sprite graphics byte order these may, or may not, be required
    needleRadarAlt.setSwapBytes (true); 
    minHeightPointer.setSwapBytes (true); 
    lowAltWarning.setSwapBytes (true); 
    greenLamp.setSwapBytes (true); 
    offFlag.setSwapBytes (true); 
  ScreenOff (vsiDisplay);

  vsiBitTest();
  altPressureGaugeBitTest ();  // Will run once on power on. Comment out if you don't want to run
  airSpeedGaugeBitTest ();  // Will run once on power on. Comment out if you don't want to run
}

void loop() 
{
  DcsBios::loop(); 
}

void altPressureGaugeBitTest () // Test full range of both needles 
{
 for (int i = 0; i < 321; i += 5){               // Up (0) is the default starting point for the needle
    int currentAltAngle = map (i, 0, 360, 360, 0);    // We want to start at about 80 deg, but have to represent this as anti-clockwise 280 degrees 

    alt1000feetNumber += 1;

    if (alt1000feetNumber > 9){
      alt10000feetNumber += 1;
      alt1000feetNumber = 0;
    }

    if (alt10000feetNumber > 9){
      alt10000feetNumber = 1;
    }

    plotAltPressureGauge (currentAltAngle);             // For this first part of the test needle 1 will move from min (0) to max (5) on the dial.
  }

  for (int i = 300; i >= 0; i -= 5){              // needlePressureAlt will move from max to min
    int currentAltAngle = map (i, 0, 360, 360, 0);
    alt1000feetNumber += 1;

    if (alt1000feetNumber > 9){
      alt10000feetNumber += 1;
      alt1000feetNumber = 0;
    }

    if (alt10000feetNumber > 9){
      alt10000feetNumber = 1;
    }

    plotAltPressureGauge (currentAltAngle); 
  }

  
  alt10000feetNumber = 0;
  alt1000feetNumber = 0;
  plotAltPressureGauge (currentAltAngle); 
}

void airSpeedGaugeBitTest () // Test full range of needle
{ 
  for (int i = 0; i < 300; i += 5){             // Test range of needle from min to max
    int angleMapBrake = map (i, 0, 360, 0, 350); // Up (0) is the default starting point for the needle
    plotAirSpeedGauge (angleMapBrake);
  }

  for (int i = 300; i >= 0; i -= 5){ // Test range of needle from max to min
    int angleMapBrake = map (i, 0, 360, 0, 340); // Up (0) is the default starting point for the needle
    plotAirSpeedGauge (angleMapBrake);
  }
} 

void plotAltPressureGauge (int16_t angle1) // Function takes both needle 1 & 2 input values, creates needle rotations and pushes them into background (Sprite)
{ 
  ScreenOn (hydPressDisplay);

    createAltPressureBackground (); // Create Sprites
    pressureAltGaugeBack.setTextColor(TFT_WHITE, CUSTOM_COLOR);  // Set text color and background color
    pressureAltGaugeBack.setTextDatum(MC_DATUM);              // Set text alignment (middle center)
    pressureAltGaugeBack.setTextSize(3);  

    pressureAltGaugeBack.drawString(String(alt1000feetNumber), 88, 93);
    pressureAltGaugeBack.drawString(String(alt10000feetNumber), 56, 93);

    createNeedlePressureAlt ();
    needlePressureAlt.pushRotated (&pressureAltGaugeBack, angle1, TFT_TRANSPARENT);  // Push needlePressureAlt at the required angle
    needlePressureAlt.deleteSprite ();

    pressureAltGaugeBack.pushSprite (0, 0, TFT_TRANSPARENT);
    pressureAltGaugeBack.deleteSprite (); // Delete Sprites to free up memory

  ScreenOff (hydPressDisplay);
}

void createAltPressureBackground () //Create hydPress gauge background as full screen Sprite and set pivot point for needles
{ 
  pressureAltGaugeBack.setColorDepth (colorDepth);
  pressureAltGaugeBack.createSprite (240, 240); //Screen size in pixels
  pressureAltGaugeBack.setPivot (120, 120); // Background image pivot point
  pressureAltGaugeBack.fillSprite (TFT_TRANSPARENT);
  pressureAltGaugeBack.pushImage (0, 0, 240, 240, pressureAltBackground); // Push image to background Sprite (x, y, dwidth, dheight, image)
}

void createNeedlePressureAlt () // Create needle from appropriate image and set pivot point
{ 
  needlePressureAlt.setColorDepth(colorDepth); // Set color depth to 16 bits
  needlePressureAlt.createSprite(7, 91); // Size of the needle sprite
  needlePressureAlt.setPivot(3, 98); // Pivot point at the base center of the needle
  needlePressureAlt.pushImage(0, 0, 7, 91, gaugeNeedle); // Load needle image
}

void plotAirSpeedGauge (int16_t angleB) // Function takes needle input value, creates needle rotation and pushes it into background image (Sprite)
{ 
  ScreenOn (airSpeedDisplay);
    createAirSpeedBackground (); // Create Sprites
    createNeedleAirSpeed (); 
    needleBrakePress.pushRotated (&brakePressGaugeBack, angleB, TFT_TRANSPARENT);
    brakePressGaugeBack.pushSprite (0, 0, TFT_TRANSPARENT);
    brakePressGaugeBack.deleteSprite (); // Delete Sprites to free up memory
    needleBrakePress.deleteSprite();
  ScreenOff (airSpeedDisplay);
}

void createAirSpeedBackground ()  //Create battery dial background as full screen Sprite and set pivot point for needles 
{
  brakePressGaugeBack.setColorDepth (colorDepth);
  brakePressGaugeBack.createSprite (240, 240); // Size of the screen in pixels
  brakePressGaugeBack.setPivot (120, 120); // Pivot point in centre of screen (Gauge)
  brakePressGaugeBack.fillSprite (TFT_TRANSPARENT);
  brakePressGaugeBack.pushImage (0, 0, 240, 240, airSpeedBackground); // (x, y, dwidth, dheight, image);
}

void createNeedleAirSpeed ()  // Create needles as Sprites from single graphic and set pivot point
{
  needleBrakePress.setColorDepth(colorDepth); // Set color depth to 16 bits
  needleBrakePress.createSprite(7, 91); // Size of the needle sprite
  needleBrakePress.setPivot(3, 100); // Pivot point at the base center of the needle
  needleBrakePress.pushImage(0, 0, 7, 91, gaugeNeedle); // Load needle image
}
  
void vsiBitTest () // Test full range of needle
{
  for (int i = 0; i < 300; i += 5){             // Test range of needle from min to max
    int currentVsiAngle = map (i, 0, 90, 90, -270); // Up (0) is the default starting point for the needle
    plotVsiGauge (currentVsiAngle);
  }

  for (int i = 300; i >= 0; i -= 5){ // Test range of needle from max to min
    int currentVsiAngle = map (i, 0,  90, 90, -270); // Up (0) is the default starting point for the needle
    plotVsiGauge (currentVsiAngle);
  }
}

void plotVsiGauge (int16_t angleRA) // Function takes needle input value, creates needle rotation and pushes it into background image (Sprite)
{ 

  ScreenOn (vsiDisplay);
  
  createBackgroundVsi (); // Create Sprites
  createNeedleVsi();
  needleRadarAlt.pushRotated(&backgroundRadarAlt, angleRA, TFT_TRANSPARENT);
  backgroundRadarAlt.pushSprite(0, 0, TFT_TRANSPARENT); // Push background with needle
  backgroundRadarAlt.deleteSprite(); 
  needleRadarAlt.deleteSprite();

  ScreenOff (vsiDisplay);
}

void createBackgroundVsi () //Create radar alt dial background as full screen Sprite and set pivot point for needle 
{ 
  backgroundRadarAlt.setColorDepth (colorDepth);
  backgroundRadarAlt.createSprite (240, 240); // Size of the screen in pixels
  backgroundRadarAlt.setPivot (120, 120); // Pivot point in centre of screen (Gauge)
  backgroundRadarAlt.fillSprite (TFT_TRANSPARENT);
  backgroundRadarAlt.pushImage (0, 0, 240, 240, vsiBackground); // (x, y, dwidth, dheight, image);
}

void createNeedleVsi() // Create needle as Sprite and set pivot point
{ 
  needleRadarAlt.setColorDepth(colorDepth); // Set color depth to 16 bits
  needleRadarAlt.createSprite(7, 91); // Size of the needle sprite
  needleRadarAlt.setPivot(3, 98); // Pivot point at the base center of the needle
  needleRadarAlt.pushImage(0, 0, 7, 91, gaugeNeedle); // Load needle image
}


void ScreenOn(uint8_t Screen)  // turn on single display
{
  digitalWrite(Screen,0);     
}

void ScreenOff(uint8_t Screen) // turn off a single display
{ 
  digitalWrite(Screen,1);     
}

void ScreensOn() // turn all displays on
{ 
  digitalWrite(hydPressDisplay,0);    // turn on 
  digitalWrite(airSpeedDisplay,0);    // turn on
  digitalWrite(vsiDisplay,0);    // turn on
}

void ScreensOff() // turn all displays off
{ 
  digitalWrite(hydPressDisplay,1);    // turn off
  digitalWrite(airSpeedDisplay,1);    // turn off
  digitalWrite(vsiDisplay,1);    // turn off
}


