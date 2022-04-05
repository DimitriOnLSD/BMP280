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
#define BMP280_REGISTER_RESET         0xE0 // 0x60 SPI read register

#define BMP280_REGISTER_STATUS_SPI    0x73
#define BMP280_REGISTER_CTRL_MEAS_SPI 0x74
#define BMP280_REGISTER_CONFIG_SPI    0x75
#define BMP280_REGISTER_CHIPID_SPI    0xD0
#define BMP280_REGISTER_VERSION_SPI   0x51
#define BMP280_REGISTER_RESET_SPI     0x60

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
  unsigned short dig_T1 = 0;//-------Calibration value from NVM temperature
  short          dig_T2 = 0;//-------Calibration value from NVM temperature
  short          dig_T3 = 0;//-------Calibration value from NVM temperature
  unsigned short dig_P1 = 0;//-------Calibration value from NVM pressure
  short          dig_P2 = 0;//-------Calibration value from NVM pressure
  short          dig_P3 = 0;//-------Calibration value from NVM pressure
  short          dig_P4 = 0;//-------Calibration value from NVM pressure
  short          dig_P5 = 0;//-------Calibration value from NVM pressure
  short          dig_P6 = 0;//-------Calibration value from NVM pressure
  short          dig_P7 = 0;//-------Calibration value from NVM pressure
  short          dig_P8 = 0;//-------Calibration value from NVM pressure
  short          dig_P9 = 0;//-------Calibration value from NVM pressure
  double         temp_fine;//--------Global variable for temperature and pressure
  long           raw_temp;//---------Global variable for raw temperature
  long           raw_press;//--------Global variable for raw pressure
  int            chip_id;//----------Save chip ID
  bool           i2c = true;//-------Setup BMP280 in either I2C or SPI
  void   printCalibValues();//-------Print calibration data to serial monitor. Useful for troubleshooting
  void   spiPin(byte cs_pin);//------This functions sets the chip select pin and enables SPI communication.
  long   getCalibrationData();//-----Get temperature and pressure calib values
  float  getAltitude();//------------Get altitude based on pressure and sea level pressure.
  float  findWaterBoilingPoint();//--Get water boiling point in ºC based on pressure.
  float  readData();//---------------Read data registers to get raw temperature and pressure
  float  getTemperature();//---------Calculate temperature.
  double getPressure();//------------Calculate pressure.

  // Define sampling for either temperature or pressure.
  enum bmp280_sampling
  {
    SAMPLING_NONE = 0x00,
    SAMPLING_1X   = 0x01,
    SAMPLING_2X   = 0x02,
    SAMPLING_4X   = 0x03,
    SAMPLING_8X   = 0x04,
    SAMPLING_16X  = 0x05
  };
  // Set power mode. Normal, forced or sleep.
  enum bmp280_mode
  {
    MODE_SLEEP  = 0x00,
    MODE_FORCED = 0x01,
    MODE_NORMAL = 0x03
  };
  // Set filter. 2X, 4X, 8X, 16X or OFF
  enum bmp280_filter
  {
    FILTER_OFF = 0x00,
    FILTER_2X  = 0x01,
    FILTER_4X  = 0x02,
    FILTER_8X  = 0x03,
    FILTER_16X = 0x04
  };
  // Set standby. 1ms, 63ms, 125ms, 250ms, 500ms, 1000ms, 2000ms or 4000ms
  enum bmp280_standby
  {
    STANDBY_1_MS    = 0x00,
    STANDBY_63_MS   = 0x01,
    STANDBY_125_MS  = 0x02,
    STANDBY_250_MS  = 0x03,
    STANDBY_500_MS  = 0x04,
    STANDBY_1000_MS = 0x05,
    STANDBY_2000_MS = 0x06,
    STANDBY_4000_MS = 0x07,
  };
  void setupBMP280(bmp280_sampling tSampling = SAMPLING_1X,
                   bmp280_sampling pSampling = SAMPLING_1X,
                   bmp280_mode     powerMode = MODE_NORMAL,
                   bmp280_filter      filter = FILTER_OFF,
                   bmp280_standby    standby = STANDBY_1_MS);
  struct ctrl_meas {
    ctrl_meas() : osrs_t(SAMPLING_NONE), osrs_p(SAMPLING_NONE), powerMode(MODE_SLEEP){}
    uint8_t osrs_t : 3;
    uint8_t osrs_p : 3;
    uint8_t powerMode : 2;
    uint8_t set() { return (osrs_t << 5) | (osrs_p << 2) | powerMode; }
  };
  struct config {
    config() : t_sb(STANDBY_1_MS), filter(FILTER_OFF), none(0), spi3w_en(0) {}
    uint8_t t_sb : 3;
    uint8_t filter : 3;
    uint8_t none : 1;
    uint8_t spi3w_en : 1;
    uint8_t set() { return (t_sb << 5) | (filter << 2) | spi3w_en; }
  };
  config configReg;//---------Object callig to structure for cpp file
  ctrl_meas measureReg;//-----Object callig to structure for cpp file
};

#endif