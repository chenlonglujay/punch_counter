//------------------------
//Arduino use I2C to communicate with the ADXL345.
//about SD0(ALT address) pin:
//SDO HIGH
//ADXL345 DeviceID 0X1D
//writeReg 0x3a,readReg 0x3b,deviceID(7 bits)+Write/Read bit
//SDO LOW
//ADXL345 DeviceID OX53
//writeReg 0xa6,readReg 0xa7
//example: 0x1d(0001 1101)->take 7bits=0011 1010(0x3a)  
//Ignore the read and write address as the Wire library takes care of this for us.
//We only use the base address of the device(0x53 or 0x1d)because the read/write
//bit is handled by the Wire library.

//For more details,please refer to ADXL345 datasheet. 
//------------------------
#ifndef ADXL345_h
#define ADXL345_h
#include <Wire.h>

#define ADXL345_dataX0 0x32
#define ADXL345_dataX1 0x33
#define ADXL345_dataY0 0x34
#define ADXL345_dataY1 0x35
#define ADXL345_dataZ0 0x36
#define ADXL345_dataZ1 0x37

#define ADXL345_POWER_CTL 0x2d //reg 0x2d set data 0x08 or 0x00
#define ADXL345_MeasurementMode 0x08
#define ADXL345_StandbyMode 0x00

#define ADXL345_DATA_FORMAT 0X31 //reg 0x31 set data as below
//bit 2 set 0 sign extension
//+-2g 10 bits set 0x08 
//+-4g 11 bits set 0x09
//+-8g 12 bits set 0x0a 
//+-16g 13 bits set 0x0b
//example +-8g (-2048~2047)

typedef enum chooseGravity {c2g=0, c4g, c8g, c16g} chGravity;
typedef enum chooseDeviceID{SDO_HIGH=0, SDO_LOW} chDeviceID;
typedef enum choose_xyzAxisDataH{xDataH=0, yDataH, zDataH} ch_xyzDataH;
typedef enum choose_xyzAxisDataL{xDataL=0, yDataL, zDataL} ch_xyzDataL;
class ADXL345
{
private:
	float gravityOption[4];
	float gravityBit;
	byte times_g;
	byte change_to_gravity[4];
	byte ADXL345_DATA_FORMAT_DATA;
	int ADXL345_deviceID;
	uint8_t ADXL345_deviceID_option[2];
	uint8_t ADXL345_dataFormat_option[4];
	uint8_t ADXL345_xyzAxisDataH_option[3];
	uint8_t ADXL345_xyzAxisDataL_option[3];
	void initThisClass();
	void I2C_write(byte reg, byte data);
	uint8_t I2C_read(byte reg);
	void setGravityBit(chGravity CHGY);
	void setDeviceID(chDeviceID chDEV_id);
	void setDataFormat(chGravity CHGY);
public:
	ADXL345();
	~ADXL345();
	chDeviceID getDeviceID;
	chGravity getGravity;
	void ADXL345_initial_set(chDeviceID chDEV_id, chGravity CHGY);
	void enableMeasureMode();
	void enableStandbyMode();
	int get_xyzAxis_org(ch_xyzDataL xyzDataL, ch_xyzDataH xyzDataH);
	int get_xyzAxis_gravity(ch_xyzDataL xyzDataL, ch_xyzDataH xyzDataH);
	//if you wanna show data on the SerialMonitor,please write Serial.begin(9600)
	//in main setup()
	void show_xAxis_org();
	void show_xAxis_gravity();
	void show_yAxis_org();
	void show_yAxis_gravity();
	void show_zAxis_org();
	void show_zAxis_gravity();
  	void show_xyzAxis_org();
	void show_xyzAxis_gravity();
};

#endif
	

