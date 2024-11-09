#define DCSBIOS_DEFAULT_SERIAL
#define DCSBIOS_DISABLE_SERVO
#include <DcsBios.h>
#include <SPI.h>
#include <TFT_eSPI.h> 

TFT_eSPI tft = TFT_eSPI();    

// Screen dimensions
const int SCREEN_WIDTH = 170;
const int SCREEN_HEIGHT = 320;

// Scaling factors based on new screen dimensions
const float WIDTH_SCALE = SCREEN_WIDTH / 320.0;
const float HEIGHT_SCALE = SCREEN_HEIGHT / 480.0;

// Adjusted stroke (line thickness)
int stroke = round(20 * WIDTH_SCALE);

// Colors
uint16_t green = tft.color565(0, 255, 0);
uint16_t yellow = tft.color565(255, 255, 0);
uint16_t red = tft.color565(255, 0, 0);

// Adjusted shape dimensions
const int SHAPE_WIDTH = round(130 * WIDTH_SCALE);
const int V_HEIGHT = round(120 * HEIGHT_SCALE); 
const int O_RADIUS = round(45 * WIDTH_SCALE);
const int O_RADIUS_INNER = round(35 * WIDTH_SCALE); 

// Center coordinates
int center_x = SCREEN_WIDTH / 2;
int center_y = SCREEN_HEIGHT / 2;

// Adjusted coordinates for shapes
// Upper 'V' shape
int v_x1 = center_x - SHAPE_WIDTH / 2;
int v_y1 = round(20 * HEIGHT_SCALE);
int v_x2 = center_x + SHAPE_WIDTH / 2;
int v_y2 = v_y1;
int v_x_mid = center_x;
int v_y_bottom = v_y1 + V_HEIGHT;

// Circle
int o_x_centre = center_x;
int o_y_centre = center_y;

// Lower inverted 'V' shape
int v2_x1 = center_x - SHAPE_WIDTH / 2;
int v2_y1 = SCREEN_HEIGHT - round(30 * HEIGHT_SCALE); // Adjusted bottom margin
int v2_x2 = center_x + SHAPE_WIDTH / 2;
int v2_y2 = v2_y1;
int v2_x_mid = center_x;
int v2_y_bottom = v2_y1 - V_HEIGHT; 

void drawThickLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, uint8_t thickness, bool deactivate_line) {
  uint16_t gray = tft.color565(20, 20, 20);
  if (deactivate_line) {
    color = gray;
  }
  for (int i = 0; i < thickness; i++) {
    int offset = i - thickness / 2;
    if (abs(x1 - x0) > abs(y1 - y0)) { // Horizontal-ish line
      tft.drawLine(x0, y0 + offset, x1, y1 + offset, color);
    } else { // Vertical-ish line
      tft.drawLine(x0 + offset, y0, x1 + offset, y1, color);
    }
  }
}

void drawCircleLine(int x, int y, int radius, uint16_t color, uint8_t thickness, bool deactivate_circle) {
  uint16_t gray = tft.color565(20, 20, 20);
  if (deactivate_circle) {
    color = gray;
  }
  for (int i = 0; i < thickness; i++) {
    tft.drawCircle(x, y, radius + i, color);
  }
}

void drawAOEIndexerPositions() {
  tft.fillScreen(TFT_BLACK);  
  drawCircleLine(o_x_centre, o_y_centre, O_RADIUS_INNER, TFT_BLACK, stroke, false);
  drawCircleLine(o_x_centre, o_y_centre, O_RADIUS, yellow, stroke, true);

  drawThickLine(v_x1, v_y1, v_x_mid, v_y_bottom, green, stroke, true);
  drawThickLine(v_x_mid, v_y_bottom, v_x2, v_y2, green, stroke, true);

  drawThickLine(v2_x1, v2_y1, v2_x_mid, v2_y_bottom, red, stroke, true);
  drawThickLine(v2_x_mid, v2_y_bottom, v2_x2, v2_y2, red, stroke, true);
}

void onAoaIndexerHighChange(unsigned int newValue) {
  if (newValue == 1) {
    drawThickLine(v_x1, v_y1, v_x_mid, v_y_bottom, green, stroke, false);
    drawThickLine(v_x_mid, v_y_bottom, v_x2, v_y2, green, stroke, false);
    return;
  }
  drawThickLine(v_x1, v_y1, v_x_mid, v_y_bottom, green, stroke, true);
  drawThickLine(v_x_mid, v_y_bottom, v_x2, v_y2, green, stroke, true);
}

void onAoaIndexerLowChange(unsigned int newValue) {
  if (newValue == 1) {
    drawThickLine(v2_x1, v2_y1, v2_x_mid, v2_y_bottom, red, stroke, false);
    drawThickLine(v2_x_mid, v2_y_bottom, v2_x2, v2_y2, red, stroke, false);
    return;
  }
  drawThickLine(v2_x1, v2_y1, v2_x_mid, v2_y_bottom, red, stroke, true);
  drawThickLine(v2_x_mid, v2_y_bottom, v2_x2, v2_y2, red, stroke, true);
}

void onAoaIndexerNormalChange(unsigned int newValue) {
  if (newValue == 1) {
    drawCircleLine(o_x_centre, o_y_centre, O_RADIUS, yellow, stroke, false);
    return;
  }
  drawCircleLine(o_x_centre, o_y_centre, O_RADIUS, yellow, stroke, true);
}

DcsBios::IntegerBuffer aoaIndexerHighBuffer(0x7408, 0x0008, 3, onAoaIndexerHighChange);
DcsBios::IntegerBuffer aoaIndexerLowBuffer(0x7408, 0x0020, 5, onAoaIndexerLowChange);
DcsBios::IntegerBuffer aoaIndexerNormalBuffer(0x7408, 0x0010, 4, onAoaIndexerNormalChange);

void setup() {
  Serial.begin(250000);
  DcsBios::setup();
  tft.init();
  tft.setRotation(0); // Adjust if needed
  drawAOEIndexerPositions();
}

void loop() {
  DcsBios::loop();
}
