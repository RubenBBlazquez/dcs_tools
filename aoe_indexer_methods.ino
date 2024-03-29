extern TFT_eSPI tft;

void drawThickLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, uint8_t thickness, bool desactivate_line) {
  // Calculate the difference between the start and end points
  int16_t deltaX = x1 - x0;
  int16_t deltaY = y1 - y0;
  uint16_t gray = tft.color565(20, 20, 20);

  if (desactivate_line){
      color = gray;
  }

  // Calculate the step for the thickness
  for (int i = 0; i < thickness; i++) {
    int offset = i - thickness / 2;

    if (abs(deltaX) > abs(deltaY)) { // Horizontal-ish line
      tft.drawLine(x0, y0 + offset, x1, y1 + offset, color);
    } else { // Vertical-ish line
      tft.drawLine(x0 + offset, y0, x1 + offset, y1, color);
    }
  }
}

void drawCircleLine(int x, int y, int radius, uint16_t color, uint8_t thickness, bool desactivate_circle) {
  uint16_t gray = tft.color565(20, 20, 20);

  if (desactivate_circle){
      color = gray;
  }

  // Calculate the step for the thickness
  for (int i = 0; i < thickness; i++) {
      tft.drawCircle(x, y, radius +i, color); // Outer circle
  }
}

void drawAOEIndexerPositions(){
  tft.fillScreen(TFT_BLACK);  
  drawCircleLine(o_x_centre, o_y_centre, O_RADIUS_INNER,TFT_BLACK, stroke, false);
  drawCircleLine(o_x_centre, o_y_centre, O_RADIUS, yellow, stroke, true);

  drawThickLine(v_x1, v_y1, v_x_mid, v_y_bottom, green, stroke, true);
  drawThickLine(v_x_mid, v_y_bottom, v_x2, v_y1, green, stroke, true);
      
  drawThickLine(v2_x1, v2_y1, v2_x_mid, v2_y_bottom, red, stroke, true);
  drawThickLine(v2_x_mid, v2_y_bottom, v2_x2, v2_y1, red, stroke, true);
}

void onAoaIndexerHighChange(unsigned int newValue) {
  if (!is_aoe_indexer_active){
      return;
  }

  if(newValue == 1){
    drawThickLine(v_x1, v_y1, v_x_mid, v_y_bottom, green, stroke, false);
    drawThickLine(v_x_mid, v_y_bottom, v_x2, v_y1, green, stroke, false);
    return;
  }

  drawThickLine(v_x1, v_y1, v_x_mid, v_y_bottom, green, stroke, true);
  drawThickLine(v_x_mid, v_y_bottom, v_x2, v_y1, green, stroke, true);
}

void onAoaIndexerLowChange(unsigned int newValue) {
  if (!is_aoe_indexer_active){
      return;
  }

  if (newValue == 1){
    drawThickLine(v2_x1, v2_y1, v2_x_mid, v2_y_bottom, red, stroke, false);
    drawThickLine(v2_x_mid, v2_y_bottom, v2_x2, v2_y1, red, stroke, false);
    return;
  }

  drawThickLine(v2_x1, v2_y1, v2_x_mid, v2_y_bottom, red, stroke, true);
  drawThickLine(v2_x_mid, v2_y_bottom, v2_x2, v2_y1, red, stroke, true);
}
void onAoaIndexerNormalChange(unsigned int newValue) {
    if (!is_aoe_indexer_active){
      return;
    }

    if (newValue == 1){
      drawCircleLine(o_x_centre, o_y_centre, O_RADIUS, yellow, stroke, false);
      return;
    }

    drawCircleLine(o_x_centre, o_y_centre, O_RADIUS, yellow, stroke, true);
}
