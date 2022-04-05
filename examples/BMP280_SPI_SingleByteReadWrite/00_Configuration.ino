void BMP280_CONFIG_AND_CALIB() {
  pinMode(CS_PIN, OUTPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE3)); //BMP280 only works in SPIMODE0 and SPIMODE3.

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0xF3);
  SPI.transfer(0b00001000);
  digitalWrite(CS_PIN, HIGH);

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0xF4);
  SPI.transfer(0b00100111);
  digitalWrite(CS_PIN, HIGH);

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0xF5);
  SPI.transfer(0b00000000);
  digitalWrite(CS_PIN, HIGH);

  //Returning function values and saving them to variables.
  dig_T1 = calib_readTemperature_digT1();
  dig_T2 = calib_readTemperature_digT2();
  dig_T3 = calib_readTemperature_digT3();
  
  dig_P1 = calib_readPressure_digP1();
  dig_P2 = calib_readPressure_digP2();
  dig_P3 = calib_readPressure_digP3();
  dig_P4 = calib_readPressure_digP4();
  dig_P5 = calib_readPressure_digP5();
  dig_P6 = calib_readPressure_digP6();
  dig_P7 = calib_readPressure_digP7();
  dig_P8 = calib_readPressure_digP8();
  dig_P9 = calib_readPressure_digP9();
}
