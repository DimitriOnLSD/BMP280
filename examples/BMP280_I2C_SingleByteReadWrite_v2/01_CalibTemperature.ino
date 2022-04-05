int calib_readTemperature_digT1() {
  unsigned short received_digT1_msb = 0, received_digT1_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT1_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digT1_msb = Wire.read();
  }

  received_digT1_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT1_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digT1_lsb = Wire.read();
  }

  unsigned short dig_T1 = (unsigned short) received_digT1_msb | received_digT1_lsb;
  return dig_T1;
}

int calib_readTemperature_digT2() {
  short received_digT2_msb = 0, received_digT2_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT2_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digT2_msb = Wire.read();
  }

  received_digT2_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT2_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digT2_lsb = Wire.read();
  }

  short dig_T2 = (short) received_digT2_msb | received_digT2_lsb;
  return dig_T2;
}

int calib_readTemperature_digT3() {
  short received_digT3_msb = 0, received_digT3_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT3_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digT3_msb = Wire.read();
  }

  received_digT3_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGT3_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digT3_lsb = Wire.read();
  }

  short dig_T3 = (short) received_digT3_msb | received_digT3_lsb;
  return dig_T3;
}
