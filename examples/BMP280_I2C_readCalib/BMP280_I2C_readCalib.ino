#include <BMP280_I2C.h>

BMP280 bmp;

void setup() {
  bmp.config();
  Serial.begin(9600);

  //TEMPERATURE
  Serial.println("Calibration values from device's NVM");
  Serial.println("");
  Serial.print("DIGIT T1: ");
  Serial.println(bmp.digT1());
  Serial.print("DIGIT T2: ");
  Serial.println(bmp.digT2());
  Serial.print("DIGIT T3: ");
  Serial.println(bmp.digT3());
  //PRESSURE
  Serial.print("DIGIT P1: ");
  Serial.println(bmp.digP1());
  Serial.print("DIGIT P2: ");
  Serial.println(bmp.digP2());
  Serial.print("DIGIT P3: ");
  Serial.println(bmp.digP3());
  Serial.print("DIGIT P4: ");
  Serial.println(bmp.digP4());
  Serial.print("DIGIT P5: ");
  Serial.println(bmp.digP5());
  Serial.print("DIGIT P6: ");
  Serial.println(bmp.digP6());
  Serial.print("DIGIT P7: ");
  Serial.println(bmp.digP7());
  Serial.print("DIGIT P8: ");
  Serial.println(bmp.digP8());
  Serial.print("DIGIT P9: ");
  Serial.println(bmp.digP9());
}

void loop() {
  
}