//For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <ADXL345.h>

ADXL345::ADXL345(void){
	//default
	initThisClass();
	setDeviceID(SDO_HIGH);
	setGravityBit(c8g);
	setDataFormat(c8g);
	Wire.begin();
}


ADXL345::~ADXL345(void){

}

void ADXL345::initThisClass(){

	gravityOption[c2g]=512.000;
	gravityOption[c4g]=1024.000;
	gravityOption[c8g]=2048.000;
	gravityOption[c16g]=4096.000;

	ADXL345_deviceID_option[SDO_HIGH]=0x1d;
	ADXL345_deviceID_option[SDO_LOW]=0x53;

	ADXL345_dataFormat_option[c2g]=0x08; 
	ADXL345_dataFormat_option[c4g]=0x09;
	ADXL345_dataFormat_option[c8g]=0x0a;
	ADXL345_dataFormat_option[c16g]=0x0b;

	ADXL345_xyzAxisDataL_option[xDataL]=ADXL345_dataX0;
	ADXL345_xyzAxisDataH_option[xDataH]=ADXL345_dataX1;
	ADXL345_xyzAxisDataL_option[yDataL]=ADXL345_dataY0;
	ADXL345_xyzAxisDataH_option[yDataH]=ADXL345_dataY1;
	ADXL345_xyzAxisDataL_option[zDataL]=ADXL345_dataZ0;
	ADXL345_xyzAxisDataH_option[zDataH]=ADXL345_dataZ1;

	change_to_gravity[c2g]=2;
	change_to_gravity[c4g]=4;
	change_to_gravity[c8g]=8;
	change_to_gravity[c16g]=16;

}

void ADXL345::ADXL345_initial_set(chDeviceID chDEV_id,chGravity CHGY){
	setDeviceID(chDEV_id);
	setGravityBit(CHGY);
	setDataFormat(CHGY);
	enableMeasureMode();
	I2C_write(ADXL345_DATA_FORMAT, ADXL345_DATA_FORMAT_DATA);
	delay(5);
}

void ADXL345::I2C_write(uint8_t reg, uint8_t data){
 Wire.beginTransmission(ADXL345_deviceID);
 Wire.write(reg);		//register address
 Wire.write(data);	//write data to register
 Wire.endTransmission();
}

uint8_t ADXL345::I2C_read(uint8_t reg){
	Wire.beginTransmission(ADXL345_deviceID);
	Wire.write(reg);		//register address
	Wire.endTransmission();
	//Wire.beginTransmission(ADXL345_deviceID);
	Wire.requestFrom(ADXL345_deviceID, 1);	//request 1 byte from ADXL345
	while(Wire.available()){
		return Wire.read();			//receive byte
	}
	//Wire.beginTransmission(ADXL345_deviceID);
}


void ADXL345::setGravityBit(chGravity CHGY){
	gravityBit = gravityOption[CHGY];
	times_g = change_to_gravity[CHGY];
	//Serial.print("times_g: ");
	//Serial.println(times_g);
}

void ADXL345::setDeviceID(chDeviceID chDEV_id){
	ADXL345_deviceID = ADXL345_deviceID_option[chDEV_id];
}

void ADXL345::setDataFormat(chGravity CHGY){
	ADXL345_DATA_FORMAT_DATA = ADXL345_dataFormat_option[CHGY];
}

void ADXL345::enableMeasureMode(){
	I2C_write(ADXL345_POWER_CTL, ADXL345_MeasurementMode);
}

void ADXL345::enableStandbyMode(){
	I2C_write(ADXL345_POWER_CTL, ADXL345_StandbyMode);
}

int ADXL345::get_xyzAxis_org(ch_xyzDataL xyzDataL, ch_xyzDataH xyzDataH){
	uint8_t tempH=0, tempL=0;
	int axisData=0;
	tempL = I2C_read(ADXL345_xyzAxisDataL_option[xyzDataL]);
	tempH = I2C_read(ADXL345_xyzAxisDataH_option[xyzDataH]);
	axisData = (tempH << 8) + tempL;
	delay(1);
	return axisData;
}

int ADXL345::get_xyzAxis_gravity(ch_xyzDataL xyzDataL, ch_xyzDataH xyzDataH){
	float axisData=0;
	axisData = get_xyzAxis_org(xyzDataL, xyzDataH);
	if(axisData >= 0){
		//ex set +-2g get(0~511)/511
	  	axisData = axisData/(gravityBit-1);				
	}else{
		//ex set +-2g get(-512 ~ -1)/512		
		axisData = axisData/gravityBit;
	}
	axisData = axisData * times_g;
	return axisData;
}

void ADXL345::show_xAxis_org(){
	int xAxis=0;
	xAxis = get_xyzAxis_org(xDataL, xDataH);
	Serial.print("x-Axis: ");
	Serial.println(xAxis);
}

void ADXL345:: show_yAxis_org(){
	int yAxis=0;
	yAxis = get_xyzAxis_org(yDataL, yDataH);
	Serial.print("y-Axis: ");
	Serial.println(yAxis);
}

void ADXL345::show_zAxis_org(){
	int zAxis=0;
	zAxis = get_xyzAxis_org(zDataL, zDataH);
	Serial.print("z-Axis: ");
	Serial.println(zAxis);
}

void ADXL345::show_xyzAxis_org(){
	int xAxis=0,yAxis=0,zAxis=0;
	xAxis = get_xyzAxis_org(xDataL, xDataH);
	yAxis = get_xyzAxis_org(yDataL, yDataH);
	zAxis = get_xyzAxis_org(zDataL, zDataH);
	Serial.print("xAxis: ");
	Serial.print(xAxis);
	Serial.print("		");

	Serial.print("yAxis: ");
	Serial.print(yAxis);
	Serial.print("		");

	Serial.print("zAxis: ");
	Serial.print(zAxis);
	Serial.print("		\n");
}

void ADXL345::show_xyzAxis_gravity(){
	float xAxis=0,yAxis=0,zAxis=0;
	xAxis = get_xyzAxis_gravity(xDataL, xDataH);
	yAxis = get_xyzAxis_gravity(yDataL, yDataH);
	zAxis = get_xyzAxis_gravity(zDataL, zDataH);
	Serial.print("xAxis: ");
	Serial.print(xAxis);
	Serial.print(" g");
	Serial.print("		");

	Serial.print("yAxis: ");
	Serial.print(yAxis);
	Serial.print(" g");
	Serial.print("		");

	Serial.print("zAxis: ");
	Serial.print(zAxis);
	Serial.print(" g");
	Serial.print("		\n");
}

void ADXL345::show_xAxis_gravity(){
	float xAxis=0;
	xAxis = get_xyzAxis_gravity(xDataL, xDataH);
	Serial.print("xAxis: ");
	Serial.println(xAxis);
}

void ADXL345::show_yAxis_gravity(){
	float yAxis=0;
	yAxis = get_xyzAxis_gravity(yDataL, yDataH);
	Serial.print("yAxis: ");
	Serial.println(yAxis);
}

void ADXL345::show_zAxis_gravity(){
	float zAxis=0;
	zAxis = get_xyzAxis_gravity(zDataL, zDataH);
	Serial.print("zAxis: ");
	Serial.println(zAxis);
}
