float BMP280_getTemperature() {
  //We use long variables in this case because we are later shifting 12 bits to the left, meaning we need a higher than a 2 bytes variable like "short". This way, we can shift without problems.
  long received_byte_temp = 0, received_byte_temp_2 = 0, received_byte_temp_3 = 0;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_TEMPDATA_A);
  delay(7);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_byte_temp = Wire.read();
  }
  received_byte_temp <<= 12;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_TEMPDATA_B);
  delay(7);
  Wire.endTransmission();

  Wire.requestFrom(BMP280_ADDRESS, 1);
  if (Wire.available()) {
    received_byte_temp_2 = Wire.read();
  }
  received_byte_temp_2 <<= 4;

  //Uncomment this area to use XLSB measures or a higher resolution
  /*
    Wire.beginTransmission(BMP280_ADDRESS);
    Wire.write(BMP280_REGISTER_TEMPDATA_C);
    delay(7);
    Wire.endTransmission();

    Wire.requestFrom(BMP280_ADDRESS, 1);
    if(Wire.available()){
    received_byte_temp_3 = Wire.read();
    }
    //received_byte_temp_3 <<= x;
  */
  signed long raw_temp = 0;
  raw_temp = (long) received_byte_temp | received_byte_temp_2;

  double var1 = (((double)raw_temp) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
  double var2 = ((((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0) * (((double)raw_temp) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
  temp_fine = double(var1 + var2);
  float temp = (var1 + var2) / 5120.0;
  return temp;
}
