//--------------------------------------
//74HC595
//circuit please refer driver_circuits_segment7.jpg and 74HC595.jpg
//for punch counter project
//master display punch count
//diplay as below:
/*
c0        c1        c2          c3
dis[7]    dis[6]    dis[5]      dis[4]  ----->L
c4        c5        c6          c7
dis[3]    dis[2]    dis[1]      dis[0]  ----->R
*/
//--------------------------------------

#ifndef SEGMENT7_2P3INCH_PH_H
#define SEGMENT7_2P3INCH_PH_H

#include <avr/pgmspace.h> 
#include <inttypes.h>
#define com_num 8

const uint8_t PROGMEM SEG7[] = {
  		//abcdefg dp
        B00000011,  //0,O
        B10011111,  //1
        B00100101,  //2
        B00001101,  //3
  		B10011001,  //4,y
        B01001001,  //5,S
        B01000001,  //6
        B00011111,  //7
  		B00000001,  //8
        B00011001,  //9
        B10000101,   //A, d
        B11000101,   //B, o
        B11010101,   //C, n
        B01100001,   //D, E
        B00110001,   //E, P
        B00010001,   //F, A
        B10000011,   //10, U
        B01000011,   //11,G
        B11100011,   //12,L
        B11110101,  //13,r
        B11100001,  //14,t
        B11111111,   //15, all drak
};		

enum {
    snum_0_word_O =0x00,
    snum_1,
    snum_2,
    snum_3,
    snum_4_word_y,
    snum_5_word_S,
    snum_6,
    snum_7, 
    snum_8,
    snum_9,
    sword_d,
    sword_o,
    sword_n,
    sword_E,
    sword_P,
    sword_A,
    sword_U,
    sword_G,
    sword_L,
    sword_r,
    sword_t,
    sall_dark
    };
typedef enum show{
     seg_num = 0
    ,seg_done
    ,seg_pause
    ,seg_goal
    ,seg_goal_xxxn  //number
    ,seg_goal_xxxd  //dark
    ,seg_goal_xxnx
    ,seg_goal_xxdx
    ,seg_goal_xnxx
    ,seg_goal_xdxx
    ,seg_goal_nxxx
    ,seg_goal_dxxx
    ,seg_reset
    ,seg_all_7seg_dark
    }seg_show;
class Segment7_2p3inch_PH
{
private:
	uint8_t dataPin;		//74HC595 SER/DS
	uint8_t latchPin;		//74HC595 RCLK/ST_CP 
	uint8_t clockPin;		//74HC595 SRCLK/SH_CP	   
    uint8_t control_pin[com_num];
public:  
  	Segment7_2p3inch_PH();    		
  	~Segment7_2p3inch_PH();     	 
  	void setPin_7seg(uint8_t SER,uint8_t RCLK, uint8_t SRCLK);  
  	void setPin_com(uint8_t *com_pin);		
 	void divide_and_output(int input_value_L, int input_value_R, seg_show word_L, seg_show word_R);
};

#endif


