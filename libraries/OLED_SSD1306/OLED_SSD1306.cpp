
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Wire.h"
#include "OLED_SSD1306.h"


OLED_SSD1306::OLED_SSD1306(){

}

OLED_SSD1306::~OLED_SSD1306(){

}

void OLED_SSD1306::writeCommand(unsigned char ins)
{
  Wire.beginTransmission(0x78 >> 1);//0x78 >> 1
  Wire.write(0x00);//0x00
  Wire.write(ins);
  Wire.endTransmission();
}

void OLED_SSD1306::writeData(unsigned char dat)
{
  Wire.beginTransmission(0x78 >> 1);//0x78 >> 1
  Wire.write(0x40);//0x40
  Wire.write(dat);
  Wire.endTransmission();
}

//set origin position
void OLED_SSD1306::IIC_setPos(unsigned char x, unsigned char y)
{
  writeCommand(0xb0+y);
  writeCommand(((x&0xf0)>>4)|0x10);//|0x10
  writeCommand((x&0x0f)|0x01);//|0x01
}


//(0x00, 0x00) clear screen
//(0xFF, 0xFF) fill screen
void OLED_SSD1306::fill_screen(unsigned char dat1,unsigned char dat2)
{
   unsigned char x,y;

     writeCommand(0x02);    /*set lower column address*/       
     writeCommand(0x10);    /*set higher column address*/
     writeCommand(0xB0);    /*set page address*/
     for(y=0;y<8;y++)
        {
           writeCommand(0xB0+y);    /*set page address*/
           writeCommand(0x02);    /*set lower column address*/       
           writeCommand(0x10);    /*set higher column address*/
           for(x=0;x<64;x++)
             {
              writeData(dat1);
              writeData(dat2);
             }
       }
}


void OLED_SSD1306::display8x16(unsigned char x, unsigned char y, unsigned char N, unsigned char *dp){
	
	unsigned char wm=0;
	unsigned int adder=16*N;
	unsigned char data;
	IIC_setPos(x , y);
	for(wm = 0;wm < 8;wm++)
	{	
		data = pgm_read_byte_near(dp + adder);
		writeData(data);
		adder += 1;
	}
	IIC_setPos(x,y + 1);
	for(wm = 0;wm < 8;wm++)
	{
		data = pgm_read_byte_near(dp + adder);
		writeData(data);
		adder += 1;
	}
	
}


void OLED_SSD1306::display16x16(unsigned char x, unsigned char y, unsigned char N, unsigned char *dp)
{
  unsigned char wm=0;
  unsigned int adder=32*N;
  unsigned char data;
  IIC_setPos(x , y); 
  for(wm = 0;wm < 16;wm++)
  {
    data = pgm_read_byte_near(dp + adder);
	writeData(data);
    adder += 1;
  }
  IIC_setPos(x,y + 1); 
  for(wm = 0;wm < 16;wm++)
  {
    data = pgm_read_byte_near(dp + adder);
	writeData(data);
    adder += 1;
  }
}



/*
display picture size 128x64
read x direction ....>
     column1.......column128
row0			0		....>			0
row1			0		....>			0
row2			0		....>			0
row3			0		....>			0
row4			0		....>			0
row5			0		....>			0
row6			0		....>			0
row7			0		....>			0
above all read finished(8 rows),read y direction v
.	
rowN			0		....>			0
.
v
above all read finished(8 rows),read y direction v
.
.
v
row63
*/
void OLED_SSD1306::display128x64(unsigned char *dp) 
{
	unsigned int adder = 0;
	unsigned char data;
	uint8_t x = 0;
	uint8_t y = 0;	
	IIC_setPos(0, 0);
	for(y = 0; y < 8 ; y++){
		IIC_setPos(0, y);
		for(x = 0; x < 128; x++){			
			data = pgm_read_byte_near(dp + adder);
			writeData(data);
			adder += 1;
		}	
	}
}

void OLED_SSD1306::OLED_display_OFF(){
  writeCommand(0xAE);//display OFF
}

void OLED_SSD1306::OLED_display_ON(){
  writeCommand(0xAF);//display ON
}

void OLED_SSD1306::initial()
{
  Wire.begin();
  writeCommand(0xAE);//display off
  
  writeCommand(0x00);//set lower column address
  writeCommand(0x10);//set higher column address
  
  writeCommand(0x40);//set display start line
  
  writeCommand(0xB0);//set page address
  
  writeCommand(0x81);//set Contrast Control for BANK0
  writeCommand(0xCF);//0~255
  
  writeCommand(0xA1);//set segment remap
  
  writeCommand(0xA6);//normal / reverse
  
  writeCommand(0xA8);//multiplex ratio
  writeCommand(0x3F);//duty = 1/64
  
  writeCommand(0xC8);//Com scan direction
  
  writeCommand(0xD3);//set display offset
  writeCommand(0x00);
  
  writeCommand(0xD5);//set osc division
  writeCommand(0x80);
  
  writeCommand(0xD9);//set pre-charge period
  writeCommand(0xF1);
  
  writeCommand(0xDA);//set COM pins
  writeCommand(0x12);
  
  writeCommand(0xDB);//set vcomh
  writeCommand(0x40);
  
  writeCommand(0x8D);//set charge pump enable
  writeCommand(0x14);
  
  writeCommand(0xAF);//display ON
#if 0 //SH1106
  WriteCommand(0xAE);    /*display off*/

  WriteCommand(0x02);    /*set lower column address*/
  WriteCommand(0x10);    /*set higher column address*/

  WriteCommand(0x40);    /*set display start line*/

  WriteCommand(0xB0);    /*set page address*/

  WriteCommand(0x81);    /*contract control*/
  WriteCommand(0x80);    /*128*/

  WriteCommand(0xA1);    /*set segment remap*/

  WriteCommand(0xA6);    /*normal / reverse*/

  WriteCommand(0xA8);    /*multiplex ratio*/
  WriteCommand(0x3F);    /*duty = 1/32*/

  WriteCommand(0xad);    /*set charge pump enable*/
  WriteCommand(0x8b);     /*    0x8a    外供VCC   */

  WriteCommand(0x30);    /*0X30---0X33  set VPP   9V 电压可以调整亮度!!!!*/

  WriteCommand(0xC8);    /*Com scan direction*/

  WriteCommand(0xD3);    /*set display offset*/
  WriteCommand(0x00);   /*   0x20  */

  WriteCommand(0xD5);    /*set osc division*/
  WriteCommand(0x80);    

  WriteCommand(0xD9);    /*set pre-charge period*/
  WriteCommand(0x1f);    /*0x22*/

  WriteCommand(0xDA);    /*set COM pins*/
  WriteCommand(0x12);//0x02 -- duanhang xianshi,0x12 -- lianxuhang xianshi!!!!!!!!!

  WriteCommand(0xdb);    /*set vcomh*/
  WriteCommand(0x40);     


  WriteCommand(0xAF);    /*display ON*/
#endif
}
