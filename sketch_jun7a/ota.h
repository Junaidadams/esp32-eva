#pragma once
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <TFT_22_ILI9225.h>

extern TFT_22_ILI9225 tft;

const char* ssid     = "Home 2.4G/5G";
const char* password = "Nurina*1";

void setupOTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  tft.setFont(Terminal6x8);
  tft.drawText(40, 80, "Connecting...", COLOR_WHITE);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi failed, retrying...");
    delay(5000);
    ESP.restart();
  }

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.setHostname("image-frame");
  ArduinoOTA.setPassword("yourOTApassword");

  ArduinoOTA.onStart([]() {
    tft.fillRectangle(0, 0, 219, 175, COLOR_BLACK);
    tft.setFont(Terminal6x8);
    tft.drawText(40, 80, "Updating...", COLOR_WHITE);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    // draw a simple progress bar
    int barWidth = (progress * 200) / total;
    tft.fillRectangle(10, 100, barWidth, 110, COLOR_GREEN);
    tft.fillRectangle(10 + barWidth, 100, 210, 110, COLOR_BLACK);
  });

  ArduinoOTA.onEnd([]() {
    tft.fillRectangle(0, 0, 219, 175, COLOR_BLACK);
    tft.setFont(Terminal6x8);
    tft.drawText(40, 80, "Done! Rebooting", COLOR_GREEN);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    tft.fillRectangle(0, 0, 219, 175, COLOR_BLACK);
    tft.setFont(Terminal6x8);
    if (error == OTA_AUTH_ERROR)         tft.drawText(40, 80, "Auth failed",    COLOR_RED);
    else if (error == OTA_BEGIN_ERROR)   tft.drawText(40, 80, "Begin failed",   COLOR_RED);
    else if (error == OTA_CONNECT_ERROR) tft.drawText(40, 80, "Connect failed", COLOR_RED);
    else if (error == OTA_RECEIVE_ERROR) tft.drawText(40, 80, "Receive failed", COLOR_RED);
    else if (error == OTA_END_ERROR)     tft.drawText(40, 80, "End failed",     COLOR_RED);
  });

  ArduinoOTA.begin();
}