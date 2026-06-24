#pragma once

#include <TFT_22_ILI9225.h>
#include <SD.h>

#include "pins.h"

extern TFT_22_ILI9225 tft;
extern SPIClass spiSD;

inline void drawImageFromSD(const char* path) {
  Serial.print("Opening: ");
  Serial.println(path);

  File f = SD.open(path);

  if (!f) {
    Serial.println("File open FAILED");

    tft.fillRectangle(0, 0, 175, 219, COLOR_BLACK);
    tft.setFont(Terminal6x8);
    tft.drawText(10, 100, "File not found!", COLOR_RED);

    return;
  }

  Serial.print("File size: ");
  Serial.println(f.size());

  static uint16_t lineBuffer[176];

  for (int16_t row = 0; row < 220; row++) {
    f.read((uint8_t*)lineBuffer, 176 * 2);

    for (int16_t col = 0; col < 176; col++) {
      tft.drawPixel(col, row, lineBuffer[col]);
    }
  }

  f.close();

  Serial.println("Draw complete");
}