/*  This is a library for the BMP280 Digital Pressure Sensor
 *
 *  This sensor uses either I2C or SPI communication, but not simultaneously.
 * 
 *  Author: Paulo Sousa
 * 
 *  Instituro Politécnico de Leiria, Escola Superior Tecnologia e Gestão, Curso Eletrónica e Redes de Telecomunicação
 */
#ifndef BMP280_I2C_H
#define BMP280_I2C_H

#include <Arduino.h>

//Configure Addresses
#define BMP280_ADDRESS                0x76
#define BMP280_REGISTER_CONFIG        0xF3
#define BMP280_REGISTER_CTRL_MEAS     0xF4
#define BMP280_REGISTER_CONFIG        0xF5
#define BMP280_REGISTER_CHIPID        0xD0
#define BMP280_REGISTER_VERSION       0xD1
#define BMP280_REGISTER_CAL26         0xE1

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
public:
  // Setup BMP280 and get calibration values
  void config();

  // Get raw temperature and calculate temperature
  float getTemperature();

  // Get raw pressure and calculate pressure
  double getPressure();

  float getAltitude();

  float waterBoilingPoint();

  float seaLevelForAltitude();

  // Get temperature and pressure calib values
  unsigned short digT1();
  short          digT2();
  short          digT3(); 
  unsigned short digP1();
  short          digP2();
  short          digP3();
  short          digP4();
  short          digP5();
  short          digP6();
  short          digP7();
  short          digP8();
  short          digP9();
};

#endif