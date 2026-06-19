#include <TFT_22_ILI9225.h>
#include <SPI.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

#include "pins.h"
#include "display.h"
#include "ota.h"

#include "images/image1_RGB.h"
#include "images/image2_RGB.h"

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_MOSI, TFT_CLK);

const uint16_t* images[] = { image1_RGB, image2_RGB };
const int imageCount = 2;
int currentImage = 0;

unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 200;

void showCurrentImage() {
  drawRGB565(0, 0, images[currentImage], 176, 220);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  delay(500);
  tft.begin();
  tft.setOrientation(2);
  tft.fillRectangle(0, 0, 219, 175, COLOR_BLACK);
  // setupOTA();

  tft.fillRectangle(0, 0, 219, 175, COLOR_BLACK);
  showCurrentImage();
}

void loop() {
  ArduinoOTA.handle();

  if (digitalRead(BUTTON_NEXT) == LOW) {
    unsigned long now = millis();
    if (now - lastPressTime > debounceDelay) {
      lastPressTime = now;
      currentImage = (currentImage + 1) % imageCount;
      showCurrentImage();
      while (digitalRead(BUTTON_NEXT) == LOW) delay(10);
    }
  }
}