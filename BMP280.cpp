/*  This is a library for the BMP280 Digital Pressure Sensor
 *
 *  This sensor uses either I2C or SPI communication, but not simultaneously.
 *
 *  Author: Paulo Sousa
 *
 *  Instituto Politécnico de Leiria, Escola Superior Tecnologia e Gestão, Curso Eletrónica e Redes de Telecomunicação
 */

#include <Arduino.h>
#include <BMP280.h>
#include <Wire.h>
#include <SPI.h>

void BMP280::setupBMP280(bmp280_sampling tSampling,
                         bmp280_sampling pSampling,
                         bmp280_mode powerMode,
                         bmp280_filter filter,
                         bmp280_standby standby)
{
  measureReg.osrs_t = tSampling;
  measureReg.osrs_p = pSampling;
  measureReg.powerMode = powerMode;

  configReg.filter = filter;
  configReg.t_sb = standby;

  if (i2c)
  {
    Serial.println("-I2C Communication-");
    Serial.println("");
    Wire.begin();
    writeByte(BMP280_ADDRESS, BMP280_RESET, 0xB6);                 // Reset the device
    delay(5);                                                      // Wait for power-on-reset to finish
    chip_id = readByte8(BMP280_ADDRESS, BMP280_CHIPID);            // Read chip ID
    writeByte(BMP280_ADDRESS, BMP280_CTRL_MEAS, measureReg.set()); // Set both temperature and pressure oversampling and function mode
    writeByte(BMP280_ADDRESS, BMP280_CONFIG, configReg.set());     // Set filter, standby time and 3-wire SPI
  }
  else if (!i2c)
  {
    Serial.println("-SPI Communication-");
    Serial.println("");
    SPI.begin();
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3)); // BMP280 only works in SPIMODE0 and SPIMODE3 and has a SPI clock input frequency of 10 MHz
    writeByte_SPI(cs_pin, BMP280_RESET_SPI, 0xB6);                    // Reset the device
    delay(5);                                                         // Wait for power-on-reset to finish
    chip_id = readByte8_SPI(cs_pin, BMP280_CHIPID);                   // Read chip ID
    writeByte_SPI(cs_pin, BMP280_CTRL_MEAS_SPI, measureReg.set());    // Set both temperature and pressure oversampling and function mode
    writeByte_SPI(cs_pin, BMP280_CONFIG_SPI, configReg.set());        // Set filter, standby time and 3-wire SPI
  }
  else { Serial.println("Error! No interface selected."); }
  switch (chip_id)
  {
  case 88:
    Serial.print("Device ");
    Serial.print("0x");
    Serial.print(chip_id, HEX);
    Serial.println(" recognized!");
    getCalibrationData();
    printCalibValues();
    break;
  default:
    Serial.println("Error! Device not recognized. Check address.");
    Serial.print("Returned: ");
    Serial.print("0x");
    Serial.println(chip_id, HEX);
    break;
  }
}

void BMP280::spiPin(byte cs_pin)
{
  this->cs_pin = cs_pin;
  pinMode(cs_pin, OUTPUT);
  i2c = false;
}

int32_t BMP280::readData()
{
  uint32_t data[6] = {0, 0, 0, 0, 0, 0};
  if (i2c)       { readBytes32(BMP280_ADDRESS, BMP280_PRESSDATA_7, 6, &data[0]); }
  else if (!i2c) { readBytes32_SPI(cs_pin, BMP280_PRESSDATA_7, 6, &data[0]); }
  else           { Serial.println("Error reading data. Check BMP280::readData()."); }
  raw_press = ((data[0] << 16) | (data[1] << 8) | data[2]) >> 4; // We shift 4 bits to the right to get a 20 bit array.
  raw_temp =  ((data[3] << 16) | (data[4] << 8) | data[5]) >> 4;  // We shift 4 bits to the right to get a 20 bit array.
}

float BMP280::getTemperature()
{
  float var1 = (((float)raw_temp) / 16384.0 - ((float)dig_T1) / 1024.0) * ((float)dig_T2);
  float var2 = ((((float)raw_temp) / 131072.0 - ((float)dig_T1) / 8192.0) * (((float)raw_temp) / 131072.0 - ((float)dig_T1) / 8192.0)) * ((float)dig_T3);
  temp_fine = float(var1 + var2);
  float temp = (var1 + var2) / 5120.0;
  return temp;
}
float BMP280::getPressure()
{
  float var3 = ((float)temp_fine / 2.0) - 64000.0;
  float var4 = var3 * var3 * ((float)dig_P6) / 32768.0;
  var4 = var4 + var3 * ((float)dig_P5) * 2.0;
  var4 = (var4 / 4.0) + (((float)dig_P4) * 65536.0);
  var3 = (((float)dig_P3) * var3 * var3 / 524288.0 + ((float)dig_P2) * var3) / 524288.0;
  var3 = (1.0 + var3 / 32768.0) * ((float)dig_P1);
  float pressure = 1048576.0 - (float)raw_press;
  pressure = (pressure - (var4 / 4096.0)) * 6250.0 / var3;
  var3 = ((float)dig_P9) * pressure * pressure / 2147483648.0;
  var4 = pressure * ((float)dig_P8) / 32768.0;
  pressure = pressure + (var3 + var4 + ((float)dig_P7)) / 16.0;
  // Since we are using hPa scale [Pascal], we will multiply our "pressure" variable by 0.01;
  pressure *= 0.01;
  return pressure;
}
float BMP280::getAltitude()
{
  float altitude;
  float seaLevelhPa = 1013.25;
  float pressure = getPressure();
  altitude = 44300 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));
  return altitude;
}
float BMP280::getWaterBoilingPoint()
{
  float pressure = getPressure();
  float waterBoilingPoint = (234.175 * log(pressure / 6.1078)) / (17.08085 - log(pressure / 6.1078));
  return waterBoilingPoint;
}

int32_t BMP280::getCalibrationData()
{
  uint8_t data[24]; // Hold calib values
  if (i2c)       { readBytes8(BMP280_ADDRESS, BMP280_DIGT1_LSB, 24, &data[0]); } 
  else if (!i2c) { readBytes8_SPI(cs_pin, BMP280_DIGT1_LSB, 24, &data[0]); }
  else           { Serial.println("Error reading NVM calib values. Check BMP280::getCalibrationData()."); }
  dig_T1 = (data[1] << 8) | data[0];
  dig_T2 = (data[3] << 8) | data[2];
  dig_T3 = (data[5] << 8) | data[4];
  dig_P1 = (data[7] << 8) | data[6];
  dig_P2 = (data[9] << 8) | data[8];
  dig_P3 = (data[11] << 8) | data[10];
  dig_P4 = (data[13] << 8) | data[12];
  dig_P5 = (data[15] << 8) | data[14];
  dig_P6 = (data[17] << 8) | data[16];
  dig_P7 = (data[19] << 8) | data[18];
  dig_P8 = (data[21] << 8) | data[20];
  dig_P9 = (data[23] << 8) | data[22];
}

void BMP280::printCalibValues()
{
  // TEMPERATURE
  Serial.println("Calibration values from device's NVM");
  Serial.println("");
  Serial.print("DIGIT T1: ");
  Serial.println(dig_T1);
  Serial.print("DIGIT T2: ");
  Serial.println(dig_T2);
  Serial.print("DIGIT T3: ");
  Serial.println(dig_T3);
  Serial.println("");
  // PRESSURE
  Serial.print("DIGIT P1: ");
  Serial.println(dig_P1);
  Serial.print("DIGIT P2: ");
  Serial.println(dig_P2);
  Serial.print("DIGIT P3: ");
  Serial.println(dig_P3);
  Serial.print("DIGIT P4: ");
  Serial.println(dig_P4);
  Serial.print("DIGIT P5: ");
  Serial.println(dig_P5);
  Serial.print("DIGIT P6: ");
  Serial.println(dig_P6);
  Serial.print("DIGIT P7: ");
  Serial.println(dig_P7);
  Serial.print("DIGIT P8: ");
  Serial.println(dig_P8);
  Serial.print("DIGIT P9: ");
  Serial.println(dig_P9);
  Serial.println("");
}

// I2C and SPI read/write functions
void BMP280::writeByte(uint8_t Address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.write(data);
  Wire.endTransmission();
}
uint8_t BMP280::readByte8(uint8_t Address, uint8_t subAddress)
{
  uint8_t data;
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(Address, 1);
  data = Wire.read();
  return data;
}
void BMP280::readBytes8(uint8_t Address, uint8_t subAddress, uint8_t count, uint8_t *mdata)
{
  uint8_t i = 0;
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(Address, count);
  while (Wire.available()) { mdata[i++] = Wire.read(); }
}
void BMP280::readBytes32(uint8_t Address, uint8_t subAddress, uint8_t count, uint32_t *mdata)
{
  uint8_t i = 0;
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(Address, count);
  while (Wire.available()) { mdata[i++] = Wire.read(); }
}
void BMP280::writeByte_SPI(byte cs_pin, uint8_t subAddress, uint8_t data)
{
  digitalWrite(cs_pin, LOW);
  SPI.transfer(subAddress);
  SPI.transfer(data);
  digitalWrite(cs_pin, HIGH);
}
uint8_t BMP280::readByte8_SPI(byte cs_pin, uint8_t subAddress)
{
  uint8_t data;
  digitalWrite(cs_pin, LOW);
  SPI.transfer(subAddress);
  data = SPI.transfer(0);
  digitalWrite(cs_pin, HIGH);
  return data;
}
void BMP280::readBytes8_SPI(byte cs_pin, uint8_t subAddress, uint8_t count, uint8_t *mdata)
{
  uint8_t i = 0;
  digitalWrite(cs_pin, LOW);
  SPI.transfer(subAddress);
  for (i >= 0; i <= count; i++) { mdata[i] = SPI.transfer(0); }
  digitalWrite(cs_pin, HIGH);
}
void BMP280::readBytes32_SPI(byte cs_pin, uint8_t subAddress, uint8_t count, uint32_t *mdata)
{
  uint8_t i = 0;
  digitalWrite(cs_pin, LOW);
  SPI.transfer(subAddress);
  for (i >= 0; i <= count; i++) { mdata[i] = SPI.transfer(0); }
  digitalWrite(cs_pin, HIGH);
}