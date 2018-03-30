/***************************************************************************
* OLED SSD1306 DISPLAY
//HardWare I2C
***************************************************************************/

#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <avr/pgmspace.h>

class OLED_SSD1306
{
 private:
  void writeCommand(unsigned char ins);
  void writeData(unsigned char dat);
  void IIC_setPos(unsigned char x, unsigned char y);
 public:
  OLED_SSD1306();
  ~OLED_SSD1306();
  void fill_screen(unsigned char dat1,unsigned char dat2);
  void initial();
  void display8x16(unsigned char x, unsigned char y, unsigned char N, unsigned char *dp);
  void display16x16(unsigned char x, unsigned char y, unsigned char N, unsigned char *dp);
  void display128x64(unsigned char *dp);
	void OLED_display_ON();
	void OLED_display_OFF();
};
#endif
