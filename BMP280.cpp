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
    // Reset the device
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_RESET);
    Wire.write(0xB6);
    Wire.endTransmission();
    delay(1); // Wait for power-on-reset to finish
    // Read chip ID
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_CHIPID);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS, 1);
    if (Wire.available())
      chip_id = Wire.read();
    // Set both temperature and pressure oversampling and function mode
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_CTRL_MEAS);
    Wire.write(measureReg.set());
    Wire.endTransmission();
    // Set filter, standby time and 3-wire SPI
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_CONFIG);
    Wire.write(configReg.set());
    Wire.endTransmission();
  }
  else if (!i2c)
  {
    Serial.println("-SPI Communication-");
    Serial.println("");
    SPI.begin();
    // BMP280 only works in SPIMODE0 and SPIMODE3 and has a SPI clock input frequency of 10 MHz
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3));
    // Reset the device
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_RESET_SPI);
    SPI.transfer(0xB6);
    digitalWrite(cs_pin, HIGH);
    delay(1); // Wait for power-on-reset to finish
    // Read chip ID
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_CHIPID_SPI);
    chip_id = SPI.transfer(0);
    digitalWrite(cs_pin, HIGH);
    // Set both temperature and pressure oversampling and function mode
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_CTRL_MEAS_SPI);
    SPI.transfer(measureReg.set());
    digitalWrite(cs_pin, HIGH);
    // Set filter, standby time and 3-wire SPI
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_CONFIG_SPI);
    SPI.transfer(configReg.set());
    digitalWrite(cs_pin, HIGH);
  }
  else
  {
    Serial.println("Error! No interface selected.");
  }
  switch (chip_id)
  {
  case 88:
    Serial.print("Device ");
    Serial.print(chip_id);
    Serial.println(" recognized!");
    getCalibrationData();
    printCalibValues();
    break;
  default:
    Serial.println("Error! Device not recognized. Check address.");
    Serial.print("Device ");
    Serial.print(chip_id);
    Serial.println(" unrecognized!");
    break;
  }
}

void BMP280::spiPin(byte cs_pin)
{
  this->cs_pin = cs_pin;
  pinMode(cs_pin, OUTPUT);
  i2c = false;
}

float BMP280::readData()
{
  long data[6];
  if (i2c)
  {
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_PRESSDATA_7);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS, 6);
    if (Wire.available())
    {
      data[0] = Wire.read(); // Pressure MSB
      data[1] = Wire.read(); // Pressure LSB
      data[2] = Wire.read(); // Pressure XLSB
      data[3] = Wire.read(); // Temperature MSB
      data[4] = Wire.read(); // Temperature LSB
      data[5] = Wire.read(); // Temperature XLSB
    }
  }
  else if (!i2c)
  {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_PRESSDATA_7);
    data[0] = SPI.transfer(0); // Pressure MSB     0xF7
    data[1] = SPI.transfer(0); // Pressure LSB     0xF8
    data[2] = SPI.transfer(0); // Pressure XLSB    0xF9
    data[3] = SPI.transfer(0); // Temperature MSB  0xFA
    data[4] = SPI.transfer(0); // Temperature LSB  0xFB
    data[5] = SPI.transfer(0); // Temperature XLSB 0xFC
    digitalWrite(cs_pin, HIGH);
  }
  else
  {
    Serial.println("Error reading data. Check BMP280::readData().");
  }
  raw_press = ((data[0] << 16) | (data[1] << 8) | data[2]) >> 4; // We shift 4 bits to the right to get a 20 bit array.
  raw_temp = ((data[3] << 16) | (data[4] << 8) | data[5]) >> 4;  // We shift 4 bits to the right to get a 20 bit array.
}

float BMP280::getTemperature()
{
  double var1 = (((double)raw_temp) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
  double var2 = ((((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0) * (((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
  temp_fine = double(var1 + var2);
  float temp = (var1 + var2) / 5120.0;
  return temp;
}

double BMP280::getPressure()
{
  double var3 = ((double)temp_fine / 2.0) - 64000.0;
  double var4 = var3 * var3 * ((double)dig_P6) / 32768.0;
  var4 = var4 + var3 * ((double)dig_P5) * 2.0;
  var4 = (var4 / 4.0) + (((double)dig_P4) * 65536.0);
  var3 = (((double)dig_P3) * var3 * var3 / 524288.0 + ((double)dig_P2) * var3) / 524288.0;
  var3 = (1.0 + var3 / 32768.0) * ((double)dig_P1);
  double pressure = 1048576.0 - (double)raw_press;
  pressure = (pressure - (var4 / 4096.0)) * 6250.0 / var3;
  var3 = ((double)dig_P9) * pressure * pressure / 2147483648.0;
  var4 = pressure * ((double)dig_P8) / 32768.0;
  pressure = pressure + (var3 + var4 + ((double)dig_P7)) / 16.0;
  // Since we are using hPa scale [Pascal], we will multiply our "pressure" variable by 0.01;
  pressure *= 0.01;
  return pressure;
}

float BMP280::getAltitude()
{
  float altitude;
  float seaLevelhPa = 1013.25;
  double pressure = getPressure();
  altitude = 44300 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));
  return altitude;
}

float BMP280::findWaterBoilingPoint()
{
  double pressure = getPressure();
  float waterBoilingPoint = (234.175 * log(pressure / 6.1078)) / (17.08085 - log(pressure / 6.1078));
  return waterBoilingPoint;
}

long BMP280::getCalibrationData()
{
  long data[24]; // Hold calib values
  if (i2c)
  {
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_DIGT1_LSB);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS, 24);
    if (Wire.available())
    {
      data[0] = Wire.read();  // 0x88 T1 LSB
      data[1] = Wire.read();  // 0x89 T1 MSB
      data[2] = Wire.read();  // 0x8A T2 LSB
      data[3] = Wire.read();  // 0x8B T2 MSB
      data[4] = Wire.read();  // 0x8C T3 LSB
      data[5] = Wire.read();  // 0x8D T3 MSB
      data[6] = Wire.read();  // 0x8E P1 LSB
      data[7] = Wire.read();  // 0x8F P1 MSB
      data[8] = Wire.read();  // 0x90 P2 LSB
      data[9] = Wire.read();  // 0x91 P2 MSB
      data[10] = Wire.read(); // 0x92 P3 LSB
      data[11] = Wire.read(); // 0x93 P3 MSB
      data[12] = Wire.read(); // 0x94 P4 LSB
      data[13] = Wire.read(); // 0x95 P4 MSB
      data[14] = Wire.read(); // 0x96 P5 LSB
      data[15] = Wire.read(); // 0x97 P5 MSB
      data[16] = Wire.read(); // 0x98 P6 LSB
      data[17] = Wire.read(); // 0x99 P6 MSB
      data[18] = Wire.read(); // 0x9A P7 LSB
      data[19] = Wire.read(); // 0x9B P7 MSB
      data[20] = Wire.read(); // 0x9C P8 LSB
      data[21] = Wire.read(); // 0x9D P8 MSB
      data[22] = Wire.read(); // 0x9E P9 LSB
      data[23] = Wire.read(); // 0x9F P9 MSB
    }
  }
  else if (!i2c)
  {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_DIGT1_LSB);
    data[0] = SPI.transfer(0);  // 0x88 T1 LSB
    data[1] = SPI.transfer(0);  // 0x89 T1 MSB
    data[2] = SPI.transfer(0);  // 0x8A T2 LSB
    data[3] = SPI.transfer(0);  // 0x8B T2 MSB
    data[4] = SPI.transfer(0);  // 0x8C T3 LSB
    data[5] = SPI.transfer(0);  // 0x8D T3 MSB
    data[6] = SPI.transfer(0);  // 0x8E P1 LSB
    data[7] = SPI.transfer(0);  // 0x8F P1 MSB
    data[8] = SPI.transfer(0);  // 0x90 P2 LSB
    data[9] = SPI.transfer(0);  // 0x91 P2 MSB
    data[10] = SPI.transfer(0); // 0x92 P3 LSB
    data[11] = SPI.transfer(0); // 0x93 P3 MSB
    data[12] = SPI.transfer(0); // 0x94 P4 LSB
    data[13] = SPI.transfer(0); // 0x95 P4 MSB
    data[14] = SPI.transfer(0); // 0x96 P5 LSB
    data[15] = SPI.transfer(0); // 0x97 P5 MSB
    data[16] = SPI.transfer(0); // 0x98 P6 LSB
    data[17] = SPI.transfer(0); // 0x99 P6 MSB
    data[18] = SPI.transfer(0); // 0x9A P7 LSB
    data[19] = SPI.transfer(0); // 0x9B P7 MSB
    data[20] = SPI.transfer(0); // 0x9C P8 LSB
    data[21] = SPI.transfer(0); // 0x9D P8 MSB
    data[22] = SPI.transfer(0); // 0x9E P9 LSB
    data[23] = SPI.transfer(0); // 0x9F P9 MSB
    digitalWrite(cs_pin, HIGH);
  }
  else
  {
    Serial.println("Error reading NVM calib values. Check BMP280::getCalibrationData().");
  }
  dig_T1 = (long)(data[1] << 8) | data[0];
  dig_T2 = (long)(data[3] << 8) | data[2];
  dig_T3 = (long)(data[5] << 8) | data[4];
  dig_P1 = (long)(data[7] << 8) | data[6];
  dig_P2 = (long)(data[9] << 8) | data[8];
  dig_P3 = (long)(data[11] << 8) | data[10];
  dig_P4 = (long)(data[13] << 8) | data[12];
  dig_P5 = (long)(data[15] << 8) | data[14];
  dig_P6 = (long)(data[17] << 8) | data[16];
  dig_P7 = (long)(data[19] << 8) | data[18];
  dig_P8 = (long)(data[21] << 8) | data[20];
  dig_P9 = (long)(data[23] << 8) | data[22];
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

// IR_filter
// data_filtered = (data_filtered_old * (filter_coefficient - 1) + raw_temp) / filter_coefficient;