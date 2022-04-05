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

bool           i2c = true; // Variable to switch between SPI and I2C
unsigned short dig_T1 = 0; // Calibration value from NVM temperature
short          dig_T2 = 0; // Calibration value from NVM temperature
short          dig_T3 = 0; // Calibration value from NVM temperature
unsigned short dig_P1 = 0; // Calibration value from NVM pressure
short          dig_P2 = 0; // Calibration value from NVM pressure
short          dig_P3 = 0; // Calibration value from NVM pressure
short          dig_P4 = 0; // Calibration value from NVM pressure
short          dig_P5 = 0; // Calibration value from NVM pressure
short          dig_P6 = 0; // Calibration value from NVM pressure
short          dig_P7 = 0; // Calibration value from NVM pressure
short          dig_P8 = 0; // Calibration value from NVM pressure
short          dig_P9 = 0; // Calibration value from NVM pressure
double         temp_fine;  // Global variable for temperature and pressure
long           raw_temp;   // Global variable for raw temperature
long           raw_press;  // Global variable for raw pressure

void BMP280::spiPin(byte cs_pin)
{
  this->cs_pin = cs_pin;
  pinMode(cs_pin, OUTPUT);
  i2c = false;
}

void BMP280::config()
{
  if (i2c)
  {
    Wire.begin();
    // // The 0xF3 Address is the "status" register that contains two bits which indicate the status of the device.
    // Wire.beginTransmission(BMP280_ADDRESS);
    // Wire.write(BMP280_REGISTER_STATUS);
    // Wire.write(0b00001000); // 0b00001000 indicates we want to measure R&T from this device.
    // Wire.endTransmission();
    // The 0xF4 Address is the "ctrl_meas" register that sets the data acquisition options of the device.
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_CTRL_MEAS);
    Wire.write(0b00100111);
    Wire.endTransmission();
    // The 0xF5 Address is the "config". Writes in normal mode may be ignored
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_CONFIG);
    Wire.write(0b00000000);
    Wire.endTransmission();
  }
  else
  {
    SPI.begin();
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3)); // BMP280 only works in SPIMODE0 and SPIMODE3 and has a SPI clock input frequency of 10 MHz 

    // digitalWrite(cs_pin, LOW);
    // SPI.transfer(BMP280_REGISTER_STATUS_SPI);
    // SPI.transfer(0b00001000);
    // digitalWrite(cs_pin, HIGH);

    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_CTRL_MEAS_SPI);
    SPI.transfer(0b00100111);
    digitalWrite(cs_pin, HIGH);

    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_CONFIG_SPI);
    SPI.transfer(0b00000000);
    digitalWrite(cs_pin, HIGH);
  }
}

float BMP280::readData()
{
  long data[5];
  if (i2c)
  {
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_PRESSDATA_7);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS, 6);
    if (Wire.available())
    {
      data[0] = Wire.read(); 
      data[1] = Wire.read(); 
      data[2] = Wire.read(); 
      data[3] = Wire.read(); 
      data[4] = Wire.read(); 
      data[5] = Wire.read(); 
    }
  }
  else
  {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_PRESSDATA_7);
    data[0] = SPI.transfer(0); 
    data[1] = SPI.transfer(0); 
    data[2] = SPI.transfer(0); 
    data[3] = SPI.transfer(0); 
    data[4] = SPI.transfer(0); 
    data[5] = SPI.transfer(0); 
    digitalWrite(cs_pin, HIGH);
  }
  raw_press = ((data[0] << 16) | (data[1] << 8) | data[2]) >> 4;
  raw_temp  = ((data[3] << 16) | (data[4] << 8) | data[5]) >> 4;
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
  long data[24];
  if (i2c)
  {
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_DIGT1_LSB);
    Wire.endTransmission();
    Wire.requestFrom(BMP280_ADDRESS, 24);
    if (Wire.available())
    {
      data[0] =  Wire.read(); // 0x88 T1 LSB
      data[1] =  Wire.read(); // 0x89 T1 MSB
      data[2] =  Wire.read(); // 0x8A T2 LSB
      data[3] =  Wire.read(); // 0x8B T2 MSB
      data[4] =  Wire.read(); // 0x8C T3 LSB
      data[5] =  Wire.read(); // 0x8D T3 MSB
      data[6] =  Wire.read(); // 0x8E P1 LSB
      data[7] =  Wire.read(); // 0x8F P1 MSB
      data[8] =  Wire.read(); // 0x90 P2 LSB
      data[9] =  Wire.read(); // 0x91 P2 MSB
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
    dig_T1 = (long)(data[1]  << 8) | data[0];
    dig_T2 = (long)(data[3]  << 8) | data[2];
    dig_T3 = (long)(data[5]  << 8) | data[4];
    dig_P1 = (long)(data[7]  << 8) | data[6];
    dig_P2 = (long)(data[9]  << 8) | data[8];
    dig_P3 = (long)(data[11] << 8) | data[10];
    dig_P4 = (long)(data[13] << 8) | data[12];
    dig_P5 = (long)(data[15] << 8) | data[14];
    dig_P6 = (long)(data[17] << 8) | data[16];
    dig_P7 = (long)(data[19] << 8) | data[18];
    dig_P8 = (long)(data[21] << 8) | data[20];
    dig_P9 = (long)(data[23] << 8) | data[22];
  }
  else
  {
    digitalWrite(cs_pin, LOW);
    SPI.transfer(BMP280_REGISTER_DIGT1_LSB);
    data[0] =  SPI.transfer(0); // 0x88 T1 LSB
    data[1] =  SPI.transfer(0); // 0x89 T1 MSB
    data[2] =  SPI.transfer(0); // 0x8A T2 LSB
    data[3] =  SPI.transfer(0); // 0x8B T2 MSB
    data[4] =  SPI.transfer(0); // 0x8C T3 LSB
    data[5] =  SPI.transfer(0); // 0x8D T3 MSB
    data[6] =  SPI.transfer(0); // 0x8E P1 LSB
    data[7] =  SPI.transfer(0); // 0x8F P1 MSB
    data[8] =  SPI.transfer(0); // 0x90 P2 LSB
    data[9] =  SPI.transfer(0); // 0x91 P2 MSB
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

    dig_T1 = (long)(data[1]  << 8) | data[0]; 
    dig_T2 = (long)(data[3]  << 8) | data[2]; 
    dig_T3 = (long)(data[5]  << 8) | data[4]; 
    dig_P1 = (long)(data[7]  << 8) | data[6]; 
    dig_P2 = (long)(data[9]  << 8) | data[8]; 
    dig_P3 = (long)(data[11] << 8) | data[10];
    dig_P4 = (long)(data[13] << 8) | data[12];
    dig_P5 = (long)(data[15] << 8) | data[14];
    dig_P6 = (long)(data[17] << 8) | data[16];
    dig_P7 = (long)(data[19] << 8) | data[18];
    dig_P8 = (long)(data[21] << 8) | data[20];
    dig_P9 = (long)(data[23] << 8) | data[22];
  }
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