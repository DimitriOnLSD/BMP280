#include <BMP280.h>

// #define CS 49

BMP280 bmp;

void setup()
{
  Serial.begin(9600);
  Serial.println("BOSCH");
  Serial.println("--Sensor BMP280--");
  Serial.println("Digital Pressure Sensor");
  Serial.println("");
  // bmp.spiPin(CS); 
  bmp.setupBMP280(BMP280::SAMPLING_1X,   // 1X, 2X, 4X, 8X, 16X or NONE
                  BMP280::SAMPLING_1X,   // 1X, 2X, 4X, 8X, 16X or NONE
                  BMP280::MODE_NORMAL,   // NORMAL, SLEEP or FORCED
                  BMP280::FILTER_OFF,    // 2X, 4X, 8X, 16X or OFF
                  BMP280::STANDBY_1_MS); // 1ms, 63ms, 125ms, 250ms, 500ms, 1000ms, 2000ms or 4000ms
  delay(2000);
}

void loop()
{
  bmp.readData();
  Serial.print("Approximate Temperature: ");
  Serial.print(bmp.getTemperature());
  Serial.println(" ºC");
  Serial.println("");
  delay(500);
  Serial.print("Pressure: ");
  Serial.print(bmp.getPressure());
  Serial.println(" hPa");
  Serial.println("");
  delay(500);
  Serial.print("Approximate Altitude: ");
  Serial.print(bmp.getAltitude());
  Serial.println(" meters");
  Serial.println("");
  delay(500);
  Serial.print("At this altitude, water boils @ ");
  Serial.print(bmp.getWaterBoilingPoint());
  Serial.println(" ºC");
  Serial.println("");
  delay(500);  
}