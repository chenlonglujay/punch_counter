/****************************************************************
* OLED database
*(1)0-9 8x16
*(2)a-z	8x16
*(3)A-Z	8x16
*(4)special symbols 8x16 
*(5)picture 8x16
*(6)picture 16x16
*(7)Chinese 16x16
*****************************************************************/

#include <avr/pgmspace.h>
#define PROGMEM

const char PROGMEM NUM8x16[] ={
//0-9
0x00,0xF0,0xF8,0x08,0x68,0xF8,0xF0,0x00,
0x00,0x07,0x0F,0x0B,0x08,0x0F,0x07,0x00,//0,

0x00,0x20,0x20,0x30,0xF8,0xF8,0x00,0x00,
0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,//1,

0x00,0x30,0x38,0x08,0x88,0xF8,0x70,0x00,
0x00,0x0C,0x0E,0x0B,0x09,0x08,0x08,0x00,//2,

0x00,0x30,0x38,0x88,0x88,0xF8,0x70,0x00,
0x00,0x06,0x0E,0x08,0x08,0x0F,0x07,0x00,//3,

0x00,0x00,0xF8,0xF8,0x00,0xE0,0xE0,0x00,
0x00,0x03,0x03,0x02,0x02,0x0F,0x0F,0x02,//4,

0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00,
0x00,0x08,0x08,0x08,0x0C,0x07,0x03,0x00,//5,

0x00,0xC0,0xE0,0x78,0x58,0xC8,0x80,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,//6,

0x00,0x08,0x08,0x88,0xE8,0x78,0x18,0x00,
0x00,0x00,0x0E,0x0F,0x01,0x00,0x00,0x00,//7,

0x00,0x70,0xF8,0xC8,0x88,0xF8,0x70,0x00,
0x00,0x07,0x0F,0x08,0x09,0x0F,0x07,0x00,//8,

0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00,
0x00,0x00,0x09,0x0D,0x0F,0x03,0x01,0x00,//9,	

};


const char PROGMEM ENC8x16[] ={
//A-Z 
0x00,0xE0,0xF0,0x18,0x18,0xF0,0xE0,0x00,
0x00,0x0F,0x0F,0x01,0x01,0x0F,0x0F,0x00,//A,

0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00,
0x00,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00,//B,

0x00,0xF0,0xF8,0x08,0x08,0x38,0x30,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0E,0x06,0x00,//C,

0x00,0xF8,0xF8,0x08,0x18,0xF0,0xE0,0x00,
0x00,0x0F,0x0F,0x08,0x0C,0x07,0x03,0x00,//D,

0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00,
0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,//E,

0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00,
0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,//F,

0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x00,
0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,//F,

0x00,0xF0,0xF8,0x08,0x08,0x38,0x30,0x00,
0x00,0x07,0x0F,0x08,0x09,0x0F,0x0F,0x00,//G,

0x00,0xF8,0xF8,0x80,0x80,0xF8,0xF8,0x00,
0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,//H,

0x00,0x00,0x08,0xF8,0xF8,0x08,0x00,0x00,
0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00,//I,

0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x00,
0x00,0x06,0x0E,0x08,0x08,0x0F,0x07,0x00,//J,

0x00,0xF8,0xF8,0x80,0xE0,0x78,0x18,0x00,
0x00,0x0F,0x0F,0x00,0x03,0x0F,0x0C,0x00,//K,

0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,
0x00,0x0F,0x0F,0x08,0x08,0x08,0x08,0x00,//L,

0x00,0xF8,0xF8,0x20,0xC0,0x20,0xF8,0xF8,
0x00,0x0F,0x0F,0x00,0x01,0x00,0x0F,0x0F,//M,

0x00,0xF8,0xF8,0x60,0xC0,0x80,0xF8,0xF8,
0x00,0x0F,0x0F,0x00,0x00,0x01,0x0F,0x0F,//N,

0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,//O,

0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00,
0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,//P,

0x00,0xF0,0xF8,0x08,0x08,0xF8,0xF0,0x00,
0x00,0x07,0x0F,0x08,0x18,0x3F,0x27,0x00,//Q,

0x00,0xF8,0xF8,0x88,0x88,0xF8,0x70,0x00,
0x00,0x0F,0x0F,0x00,0x01,0x0F,0x0E,0x00,//R,

0x00,0x30,0x78,0xC8,0x88,0x18,0x10,0x00,
0x00,0x04,0x0C,0x08,0x09,0x0F,0x06,0x00,//S,

0x00,0x08,0x08,0xF8,0xF8,0x08,0x08,0x00,
0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,//T,

0x00,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,//U,

0x00,0xF8,0xF8,0x00,0x00,0xF8,0xF8,0x00,
0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00,//V,

0x00,0xF8,0xF8,0x00,0xC0,0x00,0xF8,0xF8,
0x00,0x01,0x0F,0x0E,0x01,0x0E,0x0F,0x01,//W,

0x00,0x18,0x38,0xE0,0xC0,0x38,0x18,0x00,
0x00,0x0E,0x0F,0x00,0x01,0x0F,0x0E,0x00,//X,

0x00,0x78,0xF8,0x80,0x80,0xF8,0x78,0x00,
0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,//Y,

0x00,0x08,0x08,0x88,0xC8,0x78,0x38,0x00,
0x00,0x0E,0x0F,0x09,0x08,0x08,0x08,0x00,//Z,

};



const char PROGMEM ENS8x16[] ={
//a-z 
0x00,0x00,0x20,0x20,0x20,0xE0,0xC0,0x00,
0x00,0x06,0x0F,0x09,0x09,0x0F,0x0F,0x00,//a,

0x00,0xF8,0xF8,0x20,0x20,0xE0,0xC0,0x00,
0x00,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00,//b,

0x00,0xC0,0xE0,0x20,0x20,0x60,0x40,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0C,0x04,0x00,//c,

0x00,0xC0,0xE0,0x20,0x20,0xF8,0xF8,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0F,0x0F,0x00,//d,

0x00,0xC0,0xE0,0x20,0x20,0xE0,0xC0,0x00,
0x00,0x07,0x0F,0x09,0x09,0x09,0x01,0x00,//e,

0x00,0x80,0xF0,0xF8,0x88,0x88,0x88,0x00,
0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,//f,

0x00,0xC0,0xE0,0x20,0x20,0xE0,0xE0,0x00,
0x00,0xC7,0xCF,0xC8,0xC8,0xFF,0x3F,0x00,//g,

0x00,0xF8,0xF8,0x20,0x20,0xE0,0xC0,0x00,
0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,//h,

0x00,0x20,0x20,0xEC,0xEC,0x00,0x00,0x00,
0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00,//i,

0x00,0x00,0x20,0x20,0xEC,0xEC,0x00,0x00,
0x00,0xC0,0xC0,0xC0,0xFF,0x3F,0x00,0x00,//j,

0x00,0xF8,0xF8,0x00,0x80,0xE0,0x60,0x00,
0x00,0x0F,0x0F,0x01,0x03,0x0E,0x0C,0x00,//k,

0x00,0x08,0x08,0xF8,0xF8,0x00,0x00,0x00,
0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00,//l,

0x00,0xE0,0xE0,0x20,0xE0,0x20,0xE0,0xC0,
0x00,0x0F,0x0F,0x00,0x07,0x00,0x0F,0x0F,//m,

0x00,0xE0,0xE0,0x20,0x20,0xE0,0xC0,0x00,
0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00,//n,

0x00,0xC0,0xE0,0x20,0x20,0xE0,0xC0,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,//o,

0x00,0xE0,0xE0,0x20,0x20,0xE0,0xC0,0x00,
0x00,0xFF,0xFF,0x08,0x08,0x0F,0x07,0x00,//p,

0x00,0xC0,0xE0,0x20,0x20,0xE0,0xE0,0x00,
0x00,0x07,0x0F,0x08,0x08,0xFF,0xFF,0x00,//q,

0x00,0xE0,0xE0,0x80,0x40,0x60,0x60,0x00,
0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,//r,

0x00,0xC0,0xE0,0x20,0x20,0x20,0x20,0x00,
0x00,0x08,0x09,0x09,0x09,0x0F,0x06,0x00,//s,

0x00,0x20,0xF8,0xF8,0x20,0x20,0x20,0x00,
0x00,0x00,0x07,0x0F,0x08,0x08,0x08,0x00,//t,

0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,
0x00,0x07,0x0F,0x08,0x08,0x0F,0x0F,0x00,//u,

0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,
0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00,//v,

0x00,0xE0,0xE0,0x00,0xC0,0x00,0xE0,0xE0,
0x00,0x03,0x0F,0x0C,0x03,0x0C,0x0F,0x03,//w,

0x00,0x60,0xE0,0x80,0x80,0xE0,0x60,0x00,
0x00,0x0C,0x0E,0x03,0x03,0x0E,0x0C,0x00,//x,

0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,
0xC0,0xC7,0xCF,0xE8,0x38,0x1F,0x07,0x00,//y,

0x00,0x20,0x20,0x20,0xA0,0xE0,0x60,0x00,
0x00,0x0C,0x0E,0x0B,0x09,0x08,0x08,0x00,//z,

};


const char PROGMEM SSYM8x16[] ={
//Special symbols	
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x00,//. dot

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x2C,0x3C,0x1C,0x00,0x00,//, comma

0x00,0x00,0x00,0x60,0x60,0x60,0x00,0x00,
0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x00,//: colon

0x00,0x30,0x38,0x88,0xC8,0x78,0x30,0x00,
0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00,//? question mark

0x00,0x00,0x70,0xF8,0xF8,0x70,0x00,0x00,
0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00,//! exclamation mark

0x00,0x00,0x00,0x60,0x60,0x60,0x00,0x00,
0x00,0x00,0x00,0x2C,0x3C,0x1C,0x00,0x00,//; semicolon

0x00,0xB0,0xF8,0x48,0x78,0x30,0x00,0x00,
0x00,0x07,0x0F,0x08,0x09,0x07,0x0F,0x09,//&

0x18,0x3C,0x24,0xBC,0xD8,0x60,0x30,0x00,
0x00,0x06,0x03,0x0D,0x1E,0x12,0x1E,0x0C,//% percent

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,//_ underscore

0x00,0x80,0xC0,0x60,0x30,0x18,0x08,0x00,
0x00,0x00,0x01,0x03,0x06,0x0C,0x08,0x00,//< is less than

0x00,0x08,0x18,0x30,0x60,0xC0,0x80,0x00,
0x00,0x08,0x0C,0x06,0x03,0x01,0x00,0x00,//> is more than

0x00,0x80,0x80,0xE0,0xE0,0x80,0x80,0x00,
0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00,//+ plus

0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//- hyphen

0x00,0x80,0xA0,0xE0,0xC0,0xE0,0xA0,0x80,
0x00,0x00,0x02,0x03,0x01,0x03,0x02,0x00,//* asterisk

0x00,0x00,0x00,0x80,0xE0,0x78,0x18,0x00,
0x00,0x18,0x1E,0x07,0x01,0x00,0x00,0x00,// / slash

0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,
0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,//= equal

0x00,0x00,0xC0,0xF0,0x38,0x08,0x00,0x00,
0x00,0x00,0x07,0x1F,0x38,0x20,0x00,0x00,//( open parenthesis

0x00,0x00,0xC0,0xF0,0x38,0x08,0x00,0x00,
0x00,0x00,0x07,0x1F,0x38,0x20,0x00,0x00,//) close parenthesis

0xF0,0xF8,0x08,0x88,0xC8,0x48,0xF8,0xF0,
0x07,0x0F,0x08,0x09,0x0B,0x0A,0x0B,0x0B,//@ at

0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,//| pipe

0x00,0x38,0x38,0x00,0x00,0x38,0x38,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" single quotation marks

0x00,0x00,0x00,0x38,0x38,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//' quote 

0x30,0x18,0x08,0x18,0x30,0x20,0x30,0x18,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~ tilde

0x00,0x30,0x78,0xCE,0x8E,0x18,0x10,0x00,
0x00,0x04,0x0C,0x38,0x39,0x0F,0x06,0x00,//$ dollar

0x00,0x18,0x78,0xE0,0x80,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x07,0x1E,0x18,0x00,// \ backslash

0x00,0x20,0xF8,0xF8,0x20,0xF8,0xF8,0x20,
0x00,0x02,0x0F,0x0F,0x02,0x0F,0x0F,0x02,//# hash

0x00,0x00,0x80,0xF0,0x78,0x08,0x00,0x00,
0x00,0x01,0x03,0x1E,0x3C,0x20,0x00,0x00,//{ open curly brackets

0x00,0x00,0x08,0x78,0xF0,0x80,0x00,0x00,
0x00,0x00,0x20,0x3C,0x1E,0x03,0x01,0x00,//} close curly brackets

0x00,0x00,0xF8,0xF8,0x08,0x08,0x00,0x00,
0x00,0x00,0xFF,0xFF,0xC0,0xC0,0x00,0x00,//[ open bracket

0x00,0x00,0x08,0x08,0xF8,0xF8,0x00,0x00,
0x00,0x00,0xC0,0xC0,0xFF,0xFF,0x00,0x00,//] close_bracket

};
	

const char PROGMEM picture8x16[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//clear OLED

0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//fill OLED
	
0x00,0xC0,0xE0,0xB0,0x98,0x0C,0x00,0x00,
0x00,0x31,0x19,0x0D,0x07,0x03,0x00,0x00,//lighting

0xC0,0xF8,0x0E,0xF6,0x76,0x0E,0xF8,0xE0,
0x07,0x3F,0x70,0x63,0xE3,0x73,0x3F,0x07,//timer

};

const char PROGMEM picture16x16[] = { 

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//clear

0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//fill

0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xBC,0x8E,0x83,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x81,0x41,0x71,0x3D,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,//lighting

0x00,0xF0,0xF8,0x1C,0x0E,0x06,0xFE,0xFE,0x86,0x86,0x96,0xAE,0x1C,0xF8,0xF0,0x00,
0x00,0x07,0x0F,0x1C,0x38,0x30,0x61,0x61,0x61,0x61,0x31,0x31,0x18,0x1F,0x07,0x00,//timer

0xF0,0xFC,0xFE,0x0E,0x07,0x07,0x0F,0x1F,0x37,0x0E,0xFC,0xF8,0xF0,0x00,0x00,0x00,
0x01,0x07,0x0F,0x0E,0x1C,0x1C,0x1C,0x1C,0x0C,0x0E,0x1F,0x3F,0x7D,0xF8,0xF0,0xE0,//sensitivity

};

const char PROGMEM CN16x16[] ={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//空白 0
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//填滿	1
0x00,0x00,0xFC,0x3C,0xEC,0xEC,0xEC,0xEC,0xEC,0x3E,0xFE,0x8C,0xEE,0x7E,0x0C,0x00,
0x0C,0x6F,0x3B,0x38,0x7B,0x46,0x5E,0x7B,0x7F,0x1E,0x77,0x3F,0x38,0x30,0x3C,0x00,//感 2
0x00,0xAC,0xAE,0xAE,0xAC,0xAC,0xFC,0xFC,0xBE,0xFC,0x90,0xD0,0xFE,0xFE,0x10,0x00,
0x00,0x7E,0x7E,0x32,0x7E,0x3E,0x33,0x7A,0x6E,0x7F,0x03,0x67,0x7F,0x7F,0x00,0x00,//謝 3 
0x00,0x02,0x02,0xF2,0xF2,0x02,0x02,0xFE,0xFE,0x82,0x82,0x82,0x82,0x82,0x82,0x00,
0x00,0x40,0x40,0x7F,0x7F,0x40,0x40,0x7F,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x00,//正 4	
0x00,0x40,0x58,0xDA,0xDE,0xDC,0x70,0x7C,0x5E,0xD8,0xDC,0xDC,0xD6,0x5A,0x58,0x40,
0x00,0x13,0x17,0x15,0x17,0x55,0x55,0x7F,0x7F,0x15,0x17,0x1E,0x15,0x17,0x13,0x12,//拳 5
0x00,0xA8,0xA8,0xAE,0xAE,0xA8,0xA8,0x20,0x20,0x20,0xFE,0xFE,0x20,0x20,0x20,0x20,
0x00,0x3E,0x3E,0x12,0x12,0x12,0x3E,0x00,0x00,0x00,0x3F,0x3F,0x00,0x00,0x00,0x00,//計 6
0x18,0x18,0xBE,0xAE,0xFE,0xFE,0xAC,0xBE,0xBE,0xE8,0xF8,0x1F,0x1A,0xF8,0x78,0x18,
0x0C,0x4C,0x7F,0x7F,0x37,0x37,0x7F,0x6F,0x4F,0x6C,0x37,0x3F,0x3F,0x77,0x60,0x00,//數 7
0xC0,0xDE,0xDE,0xD2,0xD2,0xDE,0xDE,0xE0,0xBE,0xDE,0xF2,0xF2,0xDE,0xDE,0xC0,0x00,
0x04,0x7C,0x7E,0x26,0x27,0x7D,0x7D,0x00,0x7C,0x7D,0x27,0x26,0x7E,0x7E,0x04,0x00,//器 8
0x00,0xF4,0x54,0x54,0xFE,0xFE,0x54,0x54,0xF4,0xFC,0x7C,0x36,0xF6,0xF2,0x32,0x30,
0x00,0x01,0x7D,0x7D,0x57,0x57,0x55,0x55,0x55,0x57,0x54,0x54,0x7F,0x7D,0x00,0x00,//暫 9
0x80,0xE0,0xF8,0xFE,0x06,0x74,0x74,0x54,0x54,0x56,0x56,0x54,0x74,0x74,0x04,0x04,
0x00,0x00,0x7F,0x7F,0x07,0x07,0x05,0x65,0x65,0x7D,0x3D,0x05,0x05,0x05,0x07,0x07,//停 10		
0xFC,0xFC,0x04,0xFC,0xFC,0x00,0xFE,0xDE,0x56,0xD6,0xD6,0x56,0xD2,0xDE,0x5E,0x40,
0x3F,0x3F,0x08,0xDF,0xFF,0x7C,0x5F,0x5A,0x5B,0x5B,0x7E,0x7E,0x5B,0x5B,0x5F,0x40,//喔 11
0x00,0xFE,0xFE,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xFE,0xFE,0x00,
0x7E,0x7E,0x60,0x60,0x60,0x60,0x60,0x7F,0x7F,0x60,0x60,0x60,0x60,0x60,0xFE,0xFE,//出 12
0x00,0xFE,0x06,0x06,0xFE,0xFE,0x00,0xFF,0x83,0x83,0xFF,0xFF,0x83,0xFF,0xFF,0x00,
0x00,0x3F,0x18,0x18,0x3F,0x3F,0x00,0xFF,0xC1,0xC1,0xC1,0xC1,0xC1,0xC1,0x79,0x18,//吧 13
0xAC,0xBC,0xAF,0xAE,0xBC,0xEC,0x74,0x54,0x54,0x54,0x7E,0x54,0x54,0x54,0x54,0x40,
0x7E,0x7E,0x26,0x26,0x7E,0x4C,0x7F,0x3F,0x15,0x15,0x55,0x55,0x55,0x7F,0x3F,0x00,//請 14
0x00,0x0C,0x8C,0xCC,0xEC,0xB4,0x9C,0x8F,0x8F,0x94,0xF4,0xEC,0xCC,0x8C,0x0C,0x00,
0x00,0x40,0x60,0x70,0x38,0x1F,0x07,0x00,0x00,0x7F,0x7F,0x40,0x40,0x79,0x79,0x00,//充 15
0x38,0x7A,0x0A,0x5A,0x5A,0x5A,0x0A,0xFE,0xFE,0x0A,0x5A,0x5A,0x5A,0x0A,0x7A,0x38,
0x00,0x1F,0x1F,0x15,0x15,0x15,0x15,0x7F,0xFF,0xD5,0xD5,0xD5,0xDF,0xDF,0x60,0x30,//電 16
0xF8,0xF8,0x18,0x1F,0x1F,0xF8,0xF8,0xC0,0xE0,0xBC,0x9E,0x1A,0x18,0x18,0xF8,0xF8,
0xFF,0xFF,0x21,0x21,0x21,0x7F,0x7F,0x00,0x00,0x01,0xC7,0xCE,0xCC,0xC0,0x7F,0x1F,//的 17
0xFC,0xFC,0x04,0xFC,0xFC,0x18,0xFF,0xFF,0x98,0xF8,0xFB,0x1F,0x1C,0xF8,0x78,0x18,
0x3F,0x3F,0x08,0x3F,0xDF,0xC3,0x7F,0x7F,0x41,0x43,0x7F,0x78,0x7F,0x7F,0x40,0x40,//啦 18
0x18,0x18,0x18,0x18,0xF8,0xFF,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x30,0x38,0xDC,0xCF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC0,0xC0,//左 19
0x18,0x18,0x18,0x98,0xD8,0xF8,0xF8,0x9F,0x9F,0x98,0x98,0x98,0x98,0x98,0x18,0x18,
0x0C,0x06,0x03,0x01,0xFF,0xFF,0x61,0x61,0x61,0x61,0x61,0x61,0x7F,0xFF,0x00,0x00,//右 20
0x00,0x7F,0x7F,0x6B,0xEB,0xEB,0xEB,0x7F,0x7F,0x6B,0xEB,0xEB,0x7F,0x7F,0x00,0x00,
0x40,0x48,0x6F,0x6F,0x3D,0xFF,0xCF,0xFB,0x7B,0x1D,0x3D,0x3F,0x6E,0x6C,0xD8,0x40,//累 21
0xFE,0xFE,0x86,0xFE,0xFE,0x0C,0x6C,0x6C,0x6C,0x7F,0x7F,0x6C,0xEC,0x6C,0x6C,0x00,
0x7F,0x7F,0x30,0x7F,0x7F,0x02,0x03,0x0F,0x3B,0xF3,0xC3,0xC3,0xFF,0x7F,0x03,0x03,//時 22
0x00,0xFF,0x2D,0x2D,0xAD,0xAD,0xBF,0x80,0x80,0xBF,0xAD,0xAD,0x2D,0x2D,0xFF,0x00,
0x00,0xFF,0x00,0x00,0x3F,0x3F,0x35,0x35,0x35,0x35,0x35,0xFF,0xC0,0xC0,0xFF,0x00,//間 23
0x18,0xDA,0xDA,0x5A,0x5A,0x5A,0x5A,0xFE,0xFE,0x5A,0x5A,0x5A,0xDB,0xDB,0x1A,0x18,
0xC0,0xD7,0xD7,0xD5,0xD5,0xD5,0xD5,0xFF,0xFF,0xD5,0xD5,0xD5,0xD7,0xD7,0xD0,0xC0,//重 24
0x00,0x2E,0xAE,0xAA,0xAA,0xAE,0xAE,0xFA,0xFA,0xAE,0xAE,0xAA,0xAA,0xAA,0x2E,0x00,
0x80,0xC0,0xFF,0xFF,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFF,0xFF,0xC0,0xC0,//置 25
0x00,0xF8,0xF8,0x18,0x18,0x18,0x18,0xFF,0xFF,0x18,0x18,0x18,0x18,0xF8,0xF8,0x00,
0x00,0x07,0x07,0x06,0x06,0x06,0x06,0xFF,0xFF,0x06,0x06,0x06,0x06,0x07,0x07,0x00,//中 26
0x3C,0xBD,0x85,0xAD,0xAD,0x2D,0x85,0xFF,0xFF,0x85,0x2D,0xAD,0xAD,0x85,0xBD,0x3C,
0xCC,0xEF,0xEF,0xFE,0xDF,0xFC,0xEF,0xFE,0xFF,0xEF,0xFC,0xDF,0xFE,0xEF,0xEF,0xC0,//靈 27
0x90,0x9C,0xFE,0xB7,0x74,0xD4,0x94,0xF4,0x44,0xFC,0xBF,0x0F,0xC8,0xF8,0x08,0x08,
0x01,0x1F,0x1F,0x1B,0x5F,0x5D,0x7F,0x7F,0x59,0x61,0x37,0x1E,0x3F,0x63,0x40,0x00,//敏 28
0x00,0x00,0xFE,0xFE,0x36,0x36,0xFE,0xFF,0xB7,0xB6,0xB6,0xFE,0xFE,0x36,0x36,0x36,
0x40,0x78,0x7F,0x47,0x42,0x66,0x6E,0x7A,0x32,0x32,0x3A,0x6E,0x66,0x42,0x40,0x40,//度 29
0x00,0x18,0xD8,0xFF,0xFF,0xDC,0xF4,0xF4,0x5F,0x54,0x54,0x5F,0x5F,0xF4,0x04,0x00,
0x04,0x07,0x01,0xFF,0xFF,0xCD,0x4D,0x6F,0x6D,0x3D,0x1F,0x1F,0x3D,0x6D,0xCC,0x0C,//模 30
0x00,0xD8,0xD8,0xD8,0xD8,0xD8,0xD8,0xD8,0xD8,0xFF,0xFF,0x1A,0x1E,0x1C,0x18,0x00,
0x00,0x20,0x20,0x30,0x3F,0x3F,0x10,0x18,0x18,0x07,0x3F,0x78,0xC0,0xF8,0x38,0x00,//式 31
0xA0,0xB0,0xBC,0xEE,0xAE,0xBC,0x9C,0xB4,0xB4,0xB4,0xFE,0xFE,0xB4,0xB4,0xB4,0x00,
0x41,0x5F,0x5D,0x7F,0x79,0x3F,0x39,0xFD,0xFE,0x43,0x67,0x7E,0x3D,0x6F,0xC7,0x00,//錶 32
};

