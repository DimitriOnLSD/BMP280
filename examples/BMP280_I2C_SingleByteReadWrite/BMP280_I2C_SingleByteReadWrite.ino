//Include I2C libray
#include <Wire.h>

//Configure Addresses
#define BMP280_ADDRESS                0x76
#define BMP280_REGISTER_CONFIG        0xF3
#define BMP280_REGISTER_CTRL_MEAS     0xF4

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

//Declared variables for math equations. Both T1 and P1 are short variables because they can reach higher values than 2 bytes and are never negative.
volatile unsigned short dig_T1 = 0;
volatile int dig_T2 = 0;
volatile int dig_T3 = 0;

volatile unsigned short dig_P1 = 0;
volatile int dig_P2 = 0;
volatile int dig_P3 = 0;
volatile int dig_P4 = 0;
volatile int dig_P5 = 0;
volatile int dig_P6 = 0;
volatile int dig_P7 = 0;
volatile int dig_P8 = 0;
volatile int dig_P9 = 0;

//This variable travels between 2 functions so we have to declare it outside loop and setup
double temp_fine = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("BOSCH");
  Serial.println("--Sensor BMP280--");
  Serial.println("Digital Pressure Sensor");
  Serial.println("");
  Serial.println("-I2C Communication-");
  Serial.println("");

  //Configure measurement and power mode. Read sensor calibration values for later equations
  BMP280_CONFIG_AND_CALIB();

  //Uncomment to read calibration values
  //BMP280_READ_CALIB();
  delay(2000);
}

void loop() {
  //Receive temperature calculated value in degrees Celsius
  float temperature = BMP280_getTemperature();
  //Receive pressure calculated value in Pascal unit [hPa]
  double pressure   = BMP280_getPressure();

  //Print values to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" ºC");
  Serial.println("");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.println("");

  //Wait 1 second
  delay(1000);
}
