#include <Wire.h>

uint16_t dig_T1 = 0;         // Calibration value from NVM temperature.
int16_t dig_T2 = 0;          // Calibration value from NVM temperature.
int16_t dig_T3 = 0;          // Calibration value from NVM temperature.
uint16_t dig_P1 = 0;         // Calibration value from NVM pressure.
int16_t dig_P2 = 0;          // Calibration value from NVM pressure.
int16_t dig_P3 = 0;          // Calibration value from NVM pressure.
int16_t dig_P4 = 0;          // Calibration value from NVM pressure.
int16_t dig_P5 = 0;          // Calibration value from NVM pressure.
int16_t dig_P6 = 0;          // Calibration value from NVM pressure.
int16_t dig_P7 = 0;          // Calibration value from NVM pressure.
int16_t dig_P8 = 0;          // Calibration value from NVM pressure.
int16_t dig_P9 = 0;          // Calibration value from NVM pressure.
int32_t raw_press, raw_temp; // Global variable for temperature and pressure.
float temp_fine;             // Global variable for raw temperature and raw pressure.

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  writeByte(0x76, 0xE0, 0xB6);
  delay(10);
  uint8_t chip_id = readByte8(0x76, 0xD0);
  Serial.print("0x");
  Serial.println(chip_id, HEX);
  writeByte(0x76, 0xF4, 0b11111111);
  writeByte(0x76, 0xF5, 0b00000000);
  getCalibrationData();
  //  printCalib();
  delay(1000);
}

void loop()
{
  readData();
  Serial.print("Temperature: ");
  Serial.print(getTemperature());
  Serial.println(" ºC");
  Serial.print("Pressure: ");
  Serial.print(getPressure());
  Serial.println(" hPa");
  Serial.print("Altitude: ");
  Serial.print(getAltitude());
  Serial.println(" m");
  delay(250);
}

int32_t readData()
{
  uint32_t data[6] = {0, 0, 0, 0, 0, 0};
  readBytes32(0x76, 0xF7, 6, &data[0]);
  raw_press = ((data[0] << 16) | (data[1] << 8) | data[2]) >> 4;
  raw_temp = ((data[3] << 16) | (data[4] << 8) | data[5]) >> 4;
}

int32_t getCalibrationData()
{
  uint8_t data[24];
  readBytes8(0x76, 0x88, 24, &data[0]);
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

float getTemperature()
{
  float var1 = (((float)raw_temp) / 16384.0 - ((float)dig_T1) / 1024.0) * ((float)dig_T2);
  float var2 = ((((float)raw_temp) / 131072.0 - ((float)dig_T1) / 8192.0) * (((float)raw_temp) / 131072.0 - ((float)dig_T1) / 8192.0)) * ((float)dig_T3);
  temp_fine = float(var1 + var2);
  float temp = (var1 + var2) / 5120.0;
  return temp;
}

float getPressure()
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

float getAltitude()
{
  float altitude;
  float seaLevelhPa = 1013.25;
  float pressure = getPressure();
  altitude = 44300 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));
  return altitude;
}

void printCalib()
{
  Serial.println("Calibration values from device's NVM");
  Serial.println("");
  Serial.print("DIGIT T1: "); Serial.println(dig_T1);
  Serial.print("DIGIT T2: "); Serial.println(dig_T2);
  Serial.print("DIGIT T3: "); Serial.println(dig_T3);
  Serial.println("");
  Serial.print("DIGIT P1: "); Serial.println(dig_P1);
  Serial.print("DIGIT P2: "); Serial.println(dig_P2);
  Serial.print("DIGIT P3: "); Serial.println(dig_P3);
  Serial.print("DIGIT P4: "); Serial.println(dig_P4);
  Serial.print("DIGIT P5: "); Serial.println(dig_P5);
  Serial.print("DIGIT P6: "); Serial.println(dig_P6);
  Serial.print("DIGIT P7: "); Serial.println(dig_P7);
  Serial.print("DIGIT P8: "); Serial.println(dig_P8);
  Serial.print("DIGIT P9: "); Serial.println(dig_P9);
  Serial.println("");
}

// I2C Functions
void writeByte(uint8_t Address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.write(data);
  Wire.endTransmission();
}
uint8_t readByte8(uint8_t Address, uint8_t subAddress)
{
  uint8_t data;
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(Address, 1);
  data = Wire.read();
  return data;
}
void readBytes8(uint8_t Address, uint8_t subAddress, uint8_t count, uint8_t *mdata)
{
  uint8_t i = 0;
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(Address, count);
  while (Wire.available())
  {
    mdata[i++] = Wire.read();
  }
}
void readBytes32(uint8_t Address, uint8_t subAddress, uint8_t count, uint32_t *mdata)
{
  uint8_t i = 0;
  Wire.beginTransmission(Address);
  Wire.write(subAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(Address, count);
  while (Wire.available())
  {
    mdata[i++] = Wire.read();
  }
}
