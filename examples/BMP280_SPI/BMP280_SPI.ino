#include <BMP280.h>

#define CS 49

BMP280 bmp;

void setup()
{
  Serial.begin(9600);
  Serial.println("BOSCH");
  Serial.println("--Sensor BMP280--");
  Serial.println("Digital Pressure Sensor");
  Serial.println("");
  Serial.println("-SPI Communication-");
  Serial.println("");
  bmp.spiPin(CS);
  bmp.config();
  bmp.getCalibrationData();
  bmp.printCalibValues();
  delay(2000);
}

void loop()
{
  bmp.readData();
  Serial.print("Approximate Temperature: ");
  Serial.print(bmp.getTemperature());
  Serial.println(" ºC");
  Serial.println("");
  delay(1000);
  Serial.print("Pressure: ");
  Serial.print(bmp.getPressure());
  Serial.println(" hPa");
  Serial.println("");
  delay(1000);
  Serial.print("Approximate Altitude: ");
  Serial.print(bmp.getAltitude());
  Serial.println(" meters");
  Serial.println("");
  delay(1000);
  Serial.print("At this altitude, water boils @ ");
  Serial.print(bmp.findWaterBoilingPoint());
  Serial.println(" ºC");
  Serial.println("");
  delay(1000);
}