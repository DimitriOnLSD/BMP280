int calib_readTemperature_digT1(){
  unsigned short received_digT1_msb = 0, received_digT1_lsb = 0;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGT1_MSB);
  delay(10);
  received_digT1_msb = SPI.transfer(0);
  delay(10);
  digitalWrite(CS_PIN, HIGH);

  received_digT1_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGT1_LSB);
  delay(10);
  received_digT1_lsb = SPI.transfer(0);
  delay(10);
  digitalWrite(CS_PIN, HIGH);

  unsigned short dig_T1 = (unsigned short) received_digT1_msb | received_digT1_lsb;
  return dig_T1;
}

int calib_readTemperature_digT2(){
  short received_digT2_msb, received_digT2_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGT2_MSB);
  received_digT2_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digT2_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGT2_LSB);
  received_digT2_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_T2 = (short) received_digT2_msb | received_digT2_lsb;
  return dig_T2;
}

int calib_readTemperature_digT3(){
  short received_digT3_msb, received_digT3_lsb;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGT3_MSB);
  received_digT3_msb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_digT3_msb <<= 8;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_DIGT3_LSB);
  received_digT3_lsb = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);

  short dig_T3 = (short) received_digT3_msb | received_digT3_lsb;
  return dig_T3;
}