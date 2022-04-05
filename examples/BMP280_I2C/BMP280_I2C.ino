#include <BMP280_I2C.h>

BMP280 bmp;

void setup() {
  bmp.config();
  Serial.begin(9600);
  Serial.println("BOSCH");
  Serial.println("--Sensor BMP280--");
  Serial.println("Digital Pressure Sensor");
  Serial.println("");
  Serial.println("-I2C Communication-");
  Serial.println("");
  delay(2000);
}

void loop() {
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
  Serial.print(bmp.waterBoilingPoint());
  Serial.println(" ºC");
  Serial.println("");
  delay(1000);
  Serial.print("Pressure at sea level: ");
  Serial.print(bmp.seaLevelForAltitude());
  Serial.println(" hPa");
  Serial.println("");
  delay(1000);
}