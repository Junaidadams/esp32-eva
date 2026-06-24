#pragma once

#include <SD.h>

#include "pins.h"

static char folders[10][8];
static int folderCount = 0;
static int currentFolderIndex = 0;

static char imageFiles[50][12];
static int imageCount = 0;
static int currentImageIndex = 0;

void scanFolders() {
  folderCount = 0;

  File root = SD.open("/");

  if (!root) {
    Serial.println("Failed to open root");
    return;
  }

  File entry;

  while ((entry = root.openNextFile()) && folderCount < 10) {
    if (entry.isDirectory()) {
      strncpy(folders[folderCount], entry.name(), 7);

      folderCount++;

      Serial.print("Found folder: ");
      Serial.println(entry.name());
    }

    entry.close();
  }

  root.close();
}

void scanImages() {
  imageCount = 0;
  currentImageIndex = 0;

  char path[16];
  snprintf(path, sizeof(path), "/%s", folders[currentFolderIndex]);

  File dir = SD.open(path);

  if (!dir) {
    Serial.print("Failed to open folder: ");
    Serial.println(path);
    return;
  }

  File entry;

  while ((entry = dir.openNextFile()) && imageCount < 50) {
    if (!entry.isDirectory()) {
      String name = entry.name();

      if ((name.endsWith(".bin") || name.endsWith(".BIN")) &&
          !name.startsWith("._")) {

        strncpy(imageFiles[imageCount], entry.name(), 11);

        imageCount++;

        Serial.print("Found image: ");
        Serial.println(entry.name());
      }
    }

    entry.close();
  }

  dir.close();

  Serial.print("Total images in folder: ");
  Serial.println(imageCount);
}

void getCurrentImagePath(char* out, int maxLen) {
  snprintf(
      out,
      maxLen,
      "/%s/%s",
      folders[currentFolderIndex],
      imageFiles[currentImageIndex]);
}

void nextImage() {
  if (imageCount == 0) {
    return;
  }

  currentImageIndex = (currentImageIndex + 1) % imageCount;
}

void nextFolder() {
  if (folderCount == 0) {
    return;
  }

  currentFolderIndex = (currentFolderIndex + 1) % folderCount;

  scanImages();
}