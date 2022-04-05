/*  This is a library for the BMP280 Digital Pressure Sensor
 *
 *  This sensor uses either I2C or SPI communication, but not simultaneously.
 *
 *  Author: Paulo Sousa
 *
 *  Instituro Politécnico de Leiria, Escola Superior Tecnologia e Gestão, Curso Eletrónica e Redes de Telecomunicação
 */

#include <BMP280_I2C.h>
#include <Arduino.h>
#include <Wire.h>

/* Declared variables for math equations. Both T1 and P1 are short variables because
 *  they can reach higher values than 2 bytes and are never negative.
 */

unsigned short dig_T1 = 0;
short          dig_T2 = 0;
short          dig_T3 = 0;
unsigned short dig_P1 = 0;
short          dig_P2 = 0;
short          dig_P3 = 0;
short          dig_P4 = 0;
short          dig_P5 = 0;
short          dig_P6 = 0;
short          dig_P7 = 0;
short          dig_P8 = 0;
short          dig_P9 = 0;
double         temp_fine;
unsigned short uIncomingByte[3];
short          sIncomingByte[19];
long           tIncomingByte[2];
long           pIncomingByte[2];

void BMP280::config()
{
  Wire.begin();
  // The 0xF3 Address is the "status" register that contains two bits which indicate the status of the device.
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_CONFIG);
  Wire.write(0x08); // 0b00001000 indicates we want to measure R&T from this device.
  Wire.endTransmission();
  // The 0xF4 Address is the "ctrl_meas" register that sets the data acquisition options of the device.
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_CTRL_MEAS);
  Wire.write(0b11110111);
  Wire.endTransmission();
  // The 0xF5 Address is the "config". Writes in normal mode may be ignored
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_CONFIG);
  Wire.write(0x00);
  Wire.endTransmission();
}

float BMP280::getTemperature()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT1_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[0] = Wire.read(); // LSB
    uIncomingByte[1] = Wire.read(); // MSB
  }
  unsigned short dig_T1 = (unsigned short) (uIncomingByte[1]  << 8) | uIncomingByte[0];

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT2_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    sIncomingByte[0] = Wire.read(); // LSB
    sIncomingByte[1] = Wire.read(); // MSB
  }
  short dig_T2 = (short) (sIncomingByte[1]  << 8) | sIncomingByte[0];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT3_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    sIncomingByte[2] = Wire.read(); // LSB
    sIncomingByte[3] = Wire.read(); // MSB
  }
  short dig_T3 = (short) (sIncomingByte[3]  << 8) | sIncomingByte[2];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_TEMPDATA_A);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 3);
  if (Wire.available())
  {
    tIncomingByte[0] = Wire.read(); // MSB
    tIncomingByte[1] = Wire.read(); // LSB
    tIncomingByte[2] = Wire.read(); // XLSB
  }
  // We use long variables in this case because we are later shifting 12 bits to the left, meaning we need a higher than a 2 bytes variable like "short". This way, we can shift without problems.
  signed long raw_temp = (long) ((tIncomingByte[0] << 12) | (tIncomingByte[1] << 4) | tIncomingByte[2]);
  double var1 = (((double)raw_temp) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
  double var2 = ((((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0) * (((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
  temp_fine = double(var1 + var2);
  float temp = (var1 + var2) / 5120.0;
  // For better temperature/pressure accuracy
  temp *= 0.875;
  temp_fine *= 0.875;
  return temp;
}

double BMP280::getPressure()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP1_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[2] = Wire.read(); // LSB
    uIncomingByte[3] = Wire.read(); // MSB
  }

  unsigned short dig_P1 = (unsigned short) (uIncomingByte[3]  << 8) | uIncomingByte[2];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP2_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[4] = Wire.read(); // LSB
    uIncomingByte[5] = Wire.read(); // MSB
  }

  short dig_P2 = (short) (uIncomingByte[5]  << 8) | uIncomingByte[4];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP3_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[6] = Wire.read(); // LSB
    uIncomingByte[7] = Wire.read(); // MSB
  }

  short dig_P3 = (short) (uIncomingByte[7]  << 8) | uIncomingByte[6];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP4_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[8] = Wire.read(); // LSB
    uIncomingByte[9] = Wire.read(); // MSB
  }

  short dig_P4 = (short) (uIncomingByte[9]  << 8) | uIncomingByte[8];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP5_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[10] = Wire.read(); // LSB
    uIncomingByte[11] = Wire.read(); // MSB
  }

  short dig_P5 = (short) (uIncomingByte[11]  << 8) | uIncomingByte[10];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP6_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[12] = Wire.read(); // LSB
    uIncomingByte[13] = Wire.read(); // MSB
  }

  short dig_P6 = (short) (uIncomingByte[13]  << 8) | uIncomingByte[12];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP7_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[14] = Wire.read(); // LSB
    uIncomingByte[15] = Wire.read(); // MSB
  }

  short dig_P7 = (short) (uIncomingByte[15]  << 8) | uIncomingByte[14];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP8_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[16] = Wire.read(); // LSB
    uIncomingByte[17] = Wire.read(); // MSB
  }

  short dig_P8 = (short) (uIncomingByte[17]  << 8) | uIncomingByte[16];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP9_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[18] = Wire.read(); // LSB
    uIncomingByte[19] = Wire.read(); // MSB
  }

  short dig_P9 = (short) (uIncomingByte[19]  << 8) | uIncomingByte[18];
  
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_PRESSDATA_7);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 3);
  if (Wire.available())
  {
    pIncomingByte[0] = Wire.read(); // MSB
    pIncomingByte[1] = Wire.read(); // LSB
    pIncomingByte[2] = Wire.read(); // XLSB
  }
  signed long raw_press = (long) ((pIncomingByte[0] << 12) | (pIncomingByte[1] << 4) | pIncomingByte[2]);
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

float BMP280::seaLevelForAltitude()
{
  double pressure = getPressure();
  float altitude = getAltitude();
  float atmosphere = (pressure) / pow(1.0 - (altitude / 44330.0), 5.255);
  return atmosphere;
}

float BMP280::waterBoilingPoint()
{
  double pressure = getPressure();
  float waterBoilingPoint = (234.175 * log(pressure / 6.1078)) / (17.08085 - log(pressure / 6.1078));
  return waterBoilingPoint;
}

unsigned short BMP280::digT1()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT1_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[0] = Wire.read(); // LSB
    uIncomingByte[1] = Wire.read(); // MSB
  }
  unsigned short dig_T1 = (unsigned short) (uIncomingByte[1]  << 8) | uIncomingByte[0];
  return dig_T1;
}
short BMP280::digT2()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT2_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    sIncomingByte[0] = Wire.read(); // LSB
    sIncomingByte[1] = Wire.read(); // MSB
  }
  short dig_T2 = (short) (sIncomingByte[1]  << 8) | sIncomingByte[0];
  return dig_T2;
}
short BMP280::digT3()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT3_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    sIncomingByte[2] = Wire.read(); // LSB
    sIncomingByte[3] = Wire.read(); // MSB
  }
  short dig_T3 = (short) (sIncomingByte[3]  << 8) | sIncomingByte[2];
  return dig_T3;
}
unsigned short BMP280::digP1()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP1_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[2] = Wire.read(); // LSB
    uIncomingByte[3] = Wire.read(); // MSB
  }

  unsigned short dig_P1 = (unsigned short) (uIncomingByte[3]  << 8) | uIncomingByte[2];
  return dig_P1;
}
short BMP280::digP2()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP2_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[4] = Wire.read(); // LSB
    uIncomingByte[5] = Wire.read(); // MSB
  }

  short dig_P2 = (short) (uIncomingByte[5]  << 8) | uIncomingByte[4];
  return dig_P2;
}
short BMP280::digP3()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP3_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[6] = Wire.read(); // LSB
    uIncomingByte[7] = Wire.read(); // MSB
  }

  short dig_P3 = (short) (uIncomingByte[7]  << 8) | uIncomingByte[6];
  return dig_P3;
}
short BMP280::digP4()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP4_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[8] = Wire.read(); // LSB
    uIncomingByte[9] = Wire.read(); // MSB
  }

  short dig_P4 = (short) (uIncomingByte[9]  << 8) | uIncomingByte[8];
  return dig_P4;
}
short BMP280::digP5()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP5_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[10] = Wire.read(); // LSB
    uIncomingByte[11] = Wire.read(); // MSB
  }

  short dig_P5 = (short) (uIncomingByte[11]  << 8) | uIncomingByte[10];
  return dig_P5;
}
short BMP280::digP6()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP6_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[12] = Wire.read(); // LSB
    uIncomingByte[13] = Wire.read(); // MSB
  }

  short dig_P6 = (short) (uIncomingByte[13]  << 8) | uIncomingByte[12];
  return dig_P6;
}
short BMP280::digP7()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP7_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[14] = Wire.read(); // LSB
    uIncomingByte[15] = Wire.read(); // MSB
  }

  short dig_P7 = (short) (uIncomingByte[15]  << 8) | uIncomingByte[14];
  return dig_P7;
}
short BMP280::digP8()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP8_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[16] = Wire.read(); // LSB
    uIncomingByte[17] = Wire.read(); // MSB
  }

  short dig_P8 = (short) (uIncomingByte[17]  << 8) | uIncomingByte[16];
  return dig_P8;
}
short BMP280::digP9()
{
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP9_LSB);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, 2);
  if (Wire.available())
  {
    uIncomingByte[18] = Wire.read(); // LSB
    uIncomingByte[19] = Wire.read(); // MSB
  }

  short dig_P9 = (short) (uIncomingByte[19]  << 8) | uIncomingByte[18];
  return dig_P9;
}