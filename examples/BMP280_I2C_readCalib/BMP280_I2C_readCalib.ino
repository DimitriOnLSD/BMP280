#include <BMP280.h>

BMP280 bmp;

void setup()
{
  Serial.begin(9600);
  bmp.config();
  bmp.getCalibrationData();
  bmp.printCalibValues();
}

void loop()
{
}