int calib_readPressure_digP1() {
  unsigned short received_digP1_msb = 0, received_digP1_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP1_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP1_msb = Wire.read();
  }

  received_digP1_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP1_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP1_lsb = Wire.read();
  }

  unsigned short dig_P1 = (unsigned short) received_digP1_msb | received_digP1_lsb;
  return dig_P1;
}

int calib_readPressure_digP2() {
  short received_digP2_msb = 0, received_digP2_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP2_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP2_msb = Wire.read();
  }

  received_digP2_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP2_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP2_lsb = Wire.read();
  }

  short dig_P2 = (short) received_digP2_msb | received_digP2_lsb;
  return dig_P2;
}

int calib_readPressure_digP3() {
  short received_digP3_msb = 0, received_digP3_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP3_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP3_msb = Wire.read();
  }

  received_digP3_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP3_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP3_lsb = Wire.read();
  }

  short dig_P3 = (short) received_digP3_msb | received_digP3_lsb;
  return dig_P3;
}

int calib_readPressure_digP4() {
  short received_digP4_msb = 0, received_digP4_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP4_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP4_msb = Wire.read();
  }

  received_digP4_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP4_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP4_lsb = Wire.read();
  }

  short dig_P4 = (short) received_digP4_msb | received_digP4_lsb;
  return dig_P4;
}

int calib_readPressure_digP5() {
  short received_digP5_msb = 0, received_digP5_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP5_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP5_msb = Wire.read();
  }

  received_digP5_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP5_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP5_lsb = Wire.read();
  }

  short dig_P5 = (short) received_digP5_msb | received_digP5_lsb;
  return dig_P5;
}

int calib_readPressure_digP6() {
  short received_digP6_msb = 0, received_digP6_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP6_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP6_msb = Wire.read();
  }

  received_digP6_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP6_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP6_lsb = Wire.read();
  }

  short dig_P6 = (short) received_digP6_msb | received_digP6_lsb;
  return dig_P6;
}

int calib_readPressure_digP7() {
  short received_digP7_msb = 0, received_digP7_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP7_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP7_msb = Wire.read();
  }

  received_digP7_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP7_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP7_lsb = Wire.read();
  }

  short dig_P7 = (short) received_digP7_msb | received_digP7_lsb;
  return dig_P7;
}

int calib_readPressure_digP8() {
  short received_digP8_msb = 0, received_digP8_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP8_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP8_msb = Wire.read();
  }

  received_digP8_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP8_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP8_lsb = Wire.read();
  }

  short dig_P8 = (short) received_digP8_msb | received_digP8_lsb;
  return dig_P8;
}

int calib_readPressure_digP9() {
  short received_digP9_msb = 0, received_digP9_lsb = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP9_MSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP9_msb = Wire.read();
  }

  received_digP9_msb <<= 8;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_DIGP9_LSB);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_digP9_lsb = Wire.read();
  }

  short dig_P9 = (short) received_digP9_msb | received_digP9_lsb;
  return dig_P9;
}
