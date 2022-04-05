# BMP280
## Digital Pressure Sensor Library for Arduino Platform with I2C/SPI interfaces

The library included in the repository was tested with GY-91 board containing two sensors: **BMP280** and **MPU9250**.

### Changelog

<Version 0.3.0> 

`05/04/2022`

* Enums and structures to setup different values
* Overall changes to _.cpp_ and _.h_ files
* Better organisation in _.ino_ examples
* Variables in header file
* SPI pressure bug fix
* Added commentary

<Version 0.2.1> 

`01/04/2022`

* Created SPI communication example using multiple byte read/write in data arrays
* Added examples for SPI communication
* Added examples for calibration data
* Tutorial document

<Version 0.2.0> 

`31/03/2022`

* Created library
* Created I2C communication example using multiple byte read/write in data arrays
* Water boiling point calculation
* Approximate altitude calculation
* Printable calibration values for troubleshooting
* Added examples for I2C communication
* Added examples for calibration data

<Version 0.1.1> 

`17/03/2022`

* Added single byte read/write SPI communication example
* Organized code and some commenting
* Cleaned functions
* Formatted document

<Version 0.1.0> 

`16/03/2022`

* 1 example single byte read/write I2C communication
* Arduino code with tabs
* Temperature reading
* Pressure reading

### Examples

> I2C/SPI digital interface communication mutiple (burst) read/write data

> I2C/SPI digital interface communication single read/write data

> I2C/SPI calibration read only

**Author:** _Paulo Sousa_
