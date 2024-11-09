const byte colorDepth = 8; // Increasing this beyond 8 may cause graphic image issues

#include "BatteryBackground.h"
#include "Needle.h"

#define DCSBIOS_DEFAULT_SERIAL
#define DCSBIOS_DISABLE_SERVO

#include "DcsBios.h"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite gaugeBack = TFT_eSprite(&tft); // Sprite object for background
TFT_eSprite needleU = TFT_eSprite(&tft); // Sprite object for left needle
TFT_eSprite needleE = TFT_eSprite(&tft); // Sprite object for right needle

int angleMapU = 0; // Declare int value for needle angles
int angleMapE = 0;

DcsBios::Potentiometer radaltHeight("RADALT_HEIGHT", 34);

void onVoltUChange(unsigned int newValue) 
{
  angleMapU = map (newValue, 0, 65535, -150, -30);   // Take input for left needle (battery identifier (U). 0-65535 is input value range from DCS. -150 to -30 is the range of the left needle in degrees
  plotGauge (angleMapU, angleMapE);                 //redraw both needles irrespective if only one input value has changed
}
DcsBios::IntegerBuffer voltUBuffer(0x753c, 0xffff, 0, onVoltUChange);
  
void onVoltEChange(unsigned int newValue) 
{
  angleMapE = map (newValue, 0, 65535, 150, 30); 
  plotGauge (angleMapU, angleMapE);
}
DcsBios::IntegerBuffer voltEBuffer(0x753e, 0xffff, 0, onVoltEChange);

void setup() 
{
  DcsBios::setup();
  tft.init ();
  tft.fillScreen (TFT_BLACK);
  gaugeBack.setSwapBytes (true);
  needleE.setSwapBytes (true); 

  pinMode(16, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(17, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(35, INPUT_PULLUP);

  bitTest();
}

void loop() 
{
  DcsBios::loop(); 

   if (digitalRead(35) == LOW){
    digitalWrite(16, HIGH);
    digitalWrite(17, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(21, HIGH);
    digitalWrite(22, HIGH);
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(14, HIGH);
  }else{
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    digitalWrite(5, LOW);
    digitalWrite(21, LOW);
    digitalWrite(22, LOW);
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    digitalWrite(19, LOW);
    digitalWrite(14, LOW);
  }
}

void bitTest () { 
  for (int i = 0; i < 120; i += 5){             // Test range of both needles from min to max
    int angleMapU = map (i, 0, 120, -150, -30); // Up (0) is the default starting point for the needle. needleU will move clockwise on the left of the gauge
    int angleMapE = map (i, 0, 120, 150, 30);   // needleE will move anti-clockwise on the right of the gauge
    plotGauge (angleMapU, angleMapE);
  }

  for (int i = 119; i >= 0; i -= 5) // Test range of both needles from max to min
  { 
    int angleMapE = map (i, 0, 120, 150, 30);
    int angleMapU = map (i, 0, 120, -150, -30);
    plotGauge (angleMapU, angleMapE);
  }
}

void plotGauge (int16_t angleU, int16_t angleE) // Function takes both needle U & E input values, creates needle rotations and pushes them into background image (Sprite)
{ 
  createBackground (); // Create Sprites
  createNeedleU (); 
  createNeedleE (); 
  needleU.pushRotated (&gaugeBack, angleU, TFT_TRANSPARENT);
  needleE.pushRotated (&gaugeBack, angleE, TFT_TRANSPARENT);
  gaugeBack.pushSprite (0, 0, TFT_TRANSPARENT);
  gaugeBack.deleteSprite (); // Delete Sprites to free up memory
  needleU.deleteSprite();
  needleE.deleteSprite();
}

void createBackground () //Create battery dial background as full screen Sprite and set pivot point for needles 
{ 
  gaugeBack.setColorDepth (colorDepth);
  gaugeBack.createSprite (240, 240); // Size of the screen in pixels
  gaugeBack.setPivot (120, 120); // Pivot point in centre of screen (Gauge)
  gaugeBack.fillSprite (TFT_TRANSPARENT);
  gaugeBack.pushImage (0, 0, 240, 240, Battery); // (x, y, dwidth, dheight, image);
}

void createNeedleU () // Create needles as Sprites from single graphic and set pivot point
{ 
  needleU.setColorDepth (colorDepth);
  needleU.createSprite (15, 88); //Size of the needle (must be accurate)
  needleU.setPivot (7, 84);  // Pivot point of the needle  
  needleU.pushImage (0, 0, 15, 88, Needle); // (x, y, dwidth, dheight, image);
}

void createNeedleE () // Create needles as Sprites from single graphic and set pivot point
{ 
  needleE.setColorDepth (colorDepth);
  needleE.createSprite (15, 88); //Size of the needle (must be accurate)
  needleE.setPivot (7, 84);  // Pivot point of the needle 
  needleE.pushImage (0, 0, 15, 88, Needle); // (x, y, dwidth, dheight, image);
}