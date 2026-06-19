#include <TFT_22_ILI9225.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "pins.h"
#include "display.h"
#include "sd_manager.h"
#include "ota.h"

unsigned long lastNextPress   = 0;
unsigned long lastFolderPress = 0;
const unsigned long debounceDelay = 200;

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_MOSI, TFT_CLK);

void showCurrentImage();

void showCurrentImage() {
  char path[24];
  getCurrentImagePath(path, sizeof(path));
  Serial.print("Showing: ");
  Serial.println(path);
  drawImageFromSD(path);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_NEXT,   INPUT_PULLUP);
  pinMode(BUTTON_FOLDER, INPUT_PULLUP);

  delay(500);
  tft.begin();
  tft.setOrientation(2);
  tft.fillRectangle(0, 0, 175, 219, COLOR_BLACK);
  tft.setFont(Terminal6x8);

  // init SD
  tft.drawText(10, 100, "Initialising SD...", COLOR_WHITE);
  if (!SD.begin(SD_CS)) {
    tft.fillRectangle(0, 0, 175, 219, COLOR_BLACK);
    tft.drawText(10, 100, "SD failed!", COLOR_RED);
    Serial.println("SD init failed");
    while (true) delay(1000);  // halt
  }
  Serial.println("SD ok");

  // scan folders and images
  scanFolders();
  if (folderCount == 0) {
    tft.fillRectangle(0, 0, 175, 219, COLOR_BLACK);
    tft.drawText(10, 100, "No folders found!", COLOR_RED);
    while (true) delay(1000);  // halt
  }

  scanImages();
  tft.fillRectangle(0, 0, 175, 219, COLOR_BLACK);
  showCurrentImage();
}

void loop() {
  ArduinoOTA.handle();

  // next image button
  if (digitalRead(BUTTON_NEXT) == LOW) {
    unsigned long now = millis();
    if (now - lastNextPress > debounceDelay) {
      lastNextPress = now;
      nextImage();
      showCurrentImage();
      while (digitalRead(BUTTON_NEXT) == LOW) delay(10);
    }
  }

  // next folder button
  if (digitalRead(BUTTON_FOLDER) == LOW) {
    unsigned long now = millis();
    if (now - lastFolderPress > debounceDelay) {
      lastFolderPress = now;
      nextFolder();
      showCurrentImage();
      while (digitalRead(BUTTON_FOLDER) == LOW) delay(10);
    }
  }
}