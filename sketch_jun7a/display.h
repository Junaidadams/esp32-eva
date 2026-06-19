#pragma once
#include <TFT_22_ILI9225.h>

extern TFT_22_ILI9225 tft;  // tells the header the tft object exists elsewhere

void drawRGB565(int16_t x, int16_t y, const uint16_t *img, int16_t w, int16_t h) {
  for (int16_t row = 0; row < h; row++) {
    for (int16_t col = 0; col < w; col++) {
      tft.drawPixel(x + col, y + row, img[row * w + col]);
    }
  }
}