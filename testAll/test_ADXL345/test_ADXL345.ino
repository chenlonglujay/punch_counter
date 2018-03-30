//--------------------------------------
//程式名稱:test_AXDL345
//配合硬體:
//Arduino uno or pro mini
//AXDL345
//備註：
//AXDL345 supply voltage 2.0V to 3.3v
//--------------------------------------
#include <Wire.h>

//A4(SDA) A5(SCL)
#define SDO_PIN  1

//I2C setting
#if SDO_PIN
//SDO PIN High
#define ADXL345_deviceID 0x1d
//#define ADXL345_writeReg 0x3a
//#define ADXL345_readReg 0x3b
#else
//SDO PIN LOW
#define ADXL345_deviceID 0x53
//#define ADXL345_writeReg 0xa6
//#define ADXL345_readReg  0xa7
#endif
//Ignore the read and write address as the Wire library takes care of this for us(catch).
//we only use the base address of the device(0x53 0x1d) because the read/write bit is handled by the Wire library.

#define ADXL345_dataX0    0x32
#define ADXL345_dataX1    0x33
#define ADXL345_dataY0    0x34
#define ADXL345_dataY1    0x35
#define ADXL345_dataZ0    0x36
#define ADXL345_dataZ1    0x37

#define  ADXL345_POWER_CTL 0x2d   //reg 0x2d set data 0x08 or 0x00
#define ADXL345_MeasurementMode 0x08
#define ADXL345_StandbyMode 0x00

#define ADXL345_Data_Format  0x31 //reg 0x31 set data 0x0A +-8g(12 bits)
#define ADXL345_Data_Format_data  0x0a
#define  gravityBit  2048.000
#define  times_g 8
//get data from AXDL345
int punchCounter = 0;
float saveBF_temp = 0, saveNOW_temp = 0;
int saveCounter = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  ADXL345_initial();
  delay(100);
  Serial.println("start");
}

void loop() {
  readAndShowPunchCounter();
#if 0
  readAndShowXYZ();
#endif
}

void ADXL345_initial() {
  I2C_write(ADXL345_POWER_CTL , ADXL345_MeasurementMode);
  I2C_write(ADXL345_Data_Format , ADXL345_Data_Format_data);
}

void I2C_write(byte  reg , byte data) {
  Wire.beginTransmission(ADXL345_deviceID);
  Wire.write(reg);     //register address
  Wire.write(data);   //write data to register
  Wire.endTransmission();
}

byte I2C_read(byte  reg) {
  Wire.beginTransmission(ADXL345_deviceID);
  Wire.write(reg);     //register address
  Wire.endTransmission();
 // Wire.beginTransmission(ADXL345_deviceID);
  Wire.requestFrom(ADXL345_deviceID, 1);       //request 1 byte from ADXL345
  while (Wire.available()) {
    return Wire.read();                           //receive a byte
  }
 // Wire.beginTransmission(ADXL345_deviceID);
}

void readAndShowPunchCounter() {
  byte tempL, tempH;
  saveBF_temp = saveNOW_temp;
  tempL = I2C_read(ADXL345_dataY0);
  tempH = I2C_read(ADXL345_dataY1);
  saveNOW_temp = ((tempH << 8) + tempL);
  //Serial.print("saveNOW_temp: ");
  //Serial.println(saveNOW_temp);
  if ( abs(saveNOW_temp - saveBF_temp) >= 1000) {
    saveCounter++;

      if (saveCounter == 1) {
        Serial.println("-------------------------- ");
        Serial.print("saveBF_temp: ");
        Serial.print(saveBF_temp);
        Serial.print("                   ");
        Serial.print("saveNOW_temp: ");
        Serial.println(saveNOW_temp);
        Serial.println("-------------------------- ");
        Serial.println("                             ");
      }
      if (saveCounter == 2) {
        //move situation is fist punch and get back that will get data has big changed.
        //freeze situation data only has small changed.
        saveCounter = 0;
        punchCounter++;
        Serial.println("-------------------------- ");
        Serial.print("saveBF_temp: ");
        Serial.print(saveBF_temp);
        Serial.print("                   ");
        Serial.print("saveNOW_temp: ");
        Serial.println(saveNOW_temp);
        Serial.print("punch counter: ");
        Serial.println(punchCounter);
        Serial.println("-------------------------- ");
        Serial.println("                             ");
      }
    }
    delay(5);
  }


  void readAndShowXYZ() {
    float AccX = 0, AccY = 0, AccZ = 0;
    byte tempH = 0, tempL = 0;

    tempL = I2C_read(ADXL345_dataX0);
    tempH = I2C_read(ADXL345_dataX1);
    AccX = ((tempH << 8) + tempL) / gravityBit; //changes data to gravity unit
    Serial.print("AccX: ");
    Serial.print(AccX * times_g);
    Serial.print(" g");
    Serial.print("             ");

    tempL = I2C_read(ADXL345_dataY0);
    tempH = I2C_read(ADXL345_dataY1);
    AccY = ((tempH << 8) + tempL) / gravityBit; //changes data to gravity unit
    Serial.print("AccY: ");
    Serial.print(AccY * times_g);
    Serial.print(" g");
    Serial.print("             ");

    tempL = I2C_read(ADXL345_dataZ0);
    tempH = I2C_read(ADXL345_dataZ1);
    AccZ = ((tempH << 8) + tempL) / gravityBit; //changes data to gravity unit
    Serial.print("AccZ: ");
    Serial.print(AccZ * times_g);
    Serial.print(" g");
    Serial.print("            \n");
  }




