double BMP280_getPressure() {
  long received_byte_press = 0, received_byte_press_2 = 0, received_byte_press_3 = 0;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_PRESSDATA_7);
  received_byte_press = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_byte_press <<= 12;

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_PRESSDATA_8);
  received_byte_press_2 = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  received_byte_press_2 <<= 4;

  /*
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(BMP280_REGISTER_PRESSDATA_9);
  received_byte_press_3 = SPI.transfer(0);
  digitalWrite(CS_PIN, HIGH);
  */

  signed long raw_press = (long) received_byte_press | received_byte_press_2;

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

  //Since we are using hPa scale [Pascal], we will multiply our "press" variable by 0.01;
  pressure *= 0.01;
  return pressure;
}