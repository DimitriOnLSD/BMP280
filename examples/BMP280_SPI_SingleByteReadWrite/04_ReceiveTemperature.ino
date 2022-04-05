float BMP280_getTemperature(){
  long received_byte_temp = 0, received_byte_temp_2 = 0, received_byte_temp_3 = 0;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_TEMPDATA_A);
  received_byte_temp = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_byte_temp <<= 12;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_TEMPDATA_B);
  received_byte_temp_2 = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_byte_temp_2 <<= 4;

  /*
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_TEMPDATA_C);
  received_byte_temp_3 = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  */
  
  signed long raw_temp = (long) received_byte_temp | received_byte_temp_2;

  double var1 = (((double)raw_temp) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
  double var2 = ((((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0) * (((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
  temp_fine = double(var1 + var2);
  float temp = (var1 + var2) / 5120.0;
  return temp;
}