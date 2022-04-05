/*  This is a library for the BMP280 Digital Pressure Sensor
 *
 *  This sensor uses either I2C or SPI communication, but not simultaneously.
 * 
 *  Author: Paulo Sousa
 * 
 *  Instituro Politécnico de Leiria, Escola Superior Tecnologia e Gestão, Curso Eletrónica e Redes de Telecomunicação
 */
#ifndef BMP280_H
#define BMP280_H

#include <Arduino.h>

//Configure Addresses
#define BMP280_ADDRESS                0x76 // Can also be 0x77. Check with I2C scanner first.
#define BMP280_REGISTER_STATUS        0xF3 // 0x73 SPI read register
#define BMP280_REGISTER_CTRL_MEAS     0xF4 // 0x74 SPI read register
#define BMP280_REGISTER_CONFIG        0xF5 // 0x75 SPI read register
#define BMP280_REGISTER_CHIPID        0xD0 // 0x50 SPI read register
#define BMP280_REGISTER_VERSION       0xD1 // 0x51 SPI read register
#define BMP280_REGISTER_CAL26         0xE1 // 0x61 SPI read register

#define BMP280_REGISTER_STATUS_SPI    0x73
#define BMP280_REGISTER_CTRL_MEAS_SPI 0x74
#define BMP280_REGISTER_CONFIG_SPI    0x75
#define BMP280_REGISTER_CHIPID_SPI    0x50
#define BMP280_REGISTER_VERSION_SPI   0x51
#define BMP280_REGISTER_CAL26_SPI     0x61

//Temperature Read & Calibration Addresses
#define BMP280_REGISTER_TEMPDATA_A    0xFA
#define BMP280_REGISTER_TEMPDATA_B    0xFB
#define BMP280_REGISTER_TEMPDATA_C    0xFC
#define BMP280_REGISTER_DIGT1_LSB     0x88
#define BMP280_REGISTER_DIGT1_MSB     0x89
#define BMP280_REGISTER_DIGT2_LSB     0x8A
#define BMP280_REGISTER_DIGT2_MSB     0x8B
#define BMP280_REGISTER_DIGT3_LSB     0x8C
#define BMP280_REGISTER_DIGT3_MSB     0x8D

//Pressure Read & Calibration Addresses
#define BMP280_REGISTER_PRESSDATA_7   0xF7
#define BMP280_REGISTER_PRESSDATA_8   0xF8
#define BMP280_REGISTER_PRESSDATA_9   0xF9
#define BMP280_REGISTER_DIGP1_LSB     0x8E
#define BMP280_REGISTER_DIGP1_MSB     0x8F
#define BMP280_REGISTER_DIGP2_LSB     0x90
#define BMP280_REGISTER_DIGP2_MSB     0x91
#define BMP280_REGISTER_DIGP3_LSB     0x92
#define BMP280_REGISTER_DIGP3_MSB     0x93
#define BMP280_REGISTER_DIGP4_LSB     0x94
#define BMP280_REGISTER_DIGP4_MSB     0x95
#define BMP280_REGISTER_DIGP5_LSB     0x96
#define BMP280_REGISTER_DIGP5_MSB     0x97
#define BMP280_REGISTER_DIGP6_LSB     0x98
#define BMP280_REGISTER_DIGP6_MSB     0x99
#define BMP280_REGISTER_DIGP7_LSB     0x9A
#define BMP280_REGISTER_DIGP7_MSB     0x9B
#define BMP280_REGISTER_DIGP8_LSB     0x9C
#define BMP280_REGISTER_DIGP8_MSB     0x9D
#define BMP280_REGISTER_DIGP9_LSB     0x9E
#define BMP280_REGISTER_DIGP9_MSB     0x9F

class BMP280
{
private:
  byte cs_pin;

public:
  /* This functions sets the chip select pin and enables SPI communication.
   * If function not declared, I2C will take over. Both cannot work simultaneously.
   */
  void spiPin(byte cs_pin);

  // Setup BMP280 in either I2C or SPI
  void config();

  // Get temperature and pressure calib values
  long getCalibrationData();

  // Read data registers to get raw temperature and pressure
  float readData();

  // Calculate temperature.
  float getTemperature();

  // Calculate pressure.
  double getPressure();

  // Get altitude based on pressure and sea level pressure.
  float getAltitude();

  // Get water boiling point in ºC based on pressure.
  float findWaterBoilingPoint();

  // Print calibration data to serial monitor. Useful for troubleshooting
  void printCalibValues();
};

#endif