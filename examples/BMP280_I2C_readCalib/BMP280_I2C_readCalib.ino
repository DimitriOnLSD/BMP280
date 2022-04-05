#include <BMP280.h>

// #define CS 49

BMP280 bmp;

void setup()
{
  Serial.begin(9600);
  // bmp.spiPin(CS);
  bmp.setupBMP280();
}

void loop() {}