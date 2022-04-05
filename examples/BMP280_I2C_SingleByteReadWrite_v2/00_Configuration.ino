void BMP280_CONFIG_AND_CALIB() {
  //The 0xF3 Address is the “status” register that contains two bits which indicate the status of the device.
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_CONFIG);
  Wire.write(0x03); //0b00001000 indicates we want to measure R&T from this device.
  Wire.endTransmission();
  //The 0xF4 Address is the “ctrl_meas” register that sets the data acquisition options of the device.
  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_CTRL_MEAS);
  Wire.write(0x27); //0b00100111, here we are setting a temp resolution of 16 bits, a press resolution of 16 bits and we are selecting a "normal" power mode for the sensor.
  Wire.endTransmission();

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