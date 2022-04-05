#include <BMP280.h>

#define CS 49

BMP280 bmp;

void setup()
{
  Serial.begin(9600);
  bmp.spiPin(CS);
  bmp.config();
  bmp.getCalibrationData();
  bmp.printCalibValues();
}

void loop()
{
}