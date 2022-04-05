int calib_readPressure_digP1() {
  unsigned short received_digP1_msb, received_digP1_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP1_MSB);
  received_digP1_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP1_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP1_LSB);
  received_digP1_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  unsigned short dig_P1 = (unsigned short) received_digP1_msb | received_digP1_lsb;
  return dig_P1;
}

int calib_readPressure_digP2() {
  short received_digP2_msb, received_digP2_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP2_MSB);
  received_digP2_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP2_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP2_LSB);
  received_digP2_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P2 = (short) received_digP2_msb | received_digP2_lsb;
  return dig_P2;
}

int calib_readPressure_digP3() {
  short received_digP3_msb, received_digP3_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP3_MSB);
  received_digP3_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP3_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP3_LSB);
  received_digP3_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P3 = (short) received_digP3_msb | received_digP3_lsb;
  return dig_P3;
}

int calib_readPressure_digP4() {
  short received_digP4_msb, received_digP4_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP4_MSB);
  received_digP4_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP4_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP4_LSB);
  received_digP4_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P4 = (short) received_digP4_msb | received_digP4_lsb;
  return dig_P4;
}

int calib_readPressure_digP5() {
  short received_digP5_msb, received_digP5_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP5_MSB);
  received_digP5_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP5_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP5_LSB);
  received_digP5_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P5 = (short) received_digP5_msb | received_digP5_lsb;
  return dig_P5;
}

int calib_readPressure_digP6() {
  short received_digP6_msb, received_digP6_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP6_MSB);
  received_digP6_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP6_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP6_LSB);
  received_digP6_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P6 = (short) received_digP6_msb | received_digP6_lsb;
  return dig_P6;
}

int calib_readPressure_digP7() {
  short received_digP7_msb, received_digP7_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP7_MSB);
  received_digP7_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP7_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP7_LSB);
  received_digP7_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P7 = (short) received_digP7_msb | received_digP7_lsb;
  return dig_P7;
}

int calib_readPressure_digP8() {
  short received_digP8_msb, received_digP8_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP8_MSB);
  received_digP8_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP8_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP8_LSB);
  received_digP8_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P8 = (short) received_digP8_msb | received_digP8_lsb;
  return dig_P8;
}

int calib_readPressure_digP9() {
  short received_digP9_msb, received_digP9_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP9_MSB);
  received_digP9_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digP9_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGP9_LSB);
  received_digP9_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_P9 = (short) received_digP9_msb | received_digP9_lsb;
  return dig_P9;
}