// For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Segment7_2p3inch_PH.h>
Segment7_2p3inch_PH::Segment7_2p3inch_PH(){

}

Segment7_2p3inch_PH::~Segment7_2p3inch_PH(){

}

void Segment7_2p3inch_PH::setPin_7seg(uint8_t SER, uint8_t  RCLK, uint8_t SRCLK){
	dataPin = SER;
	latchPin = RCLK;
 	clockPin = SRCLK;
  	pinMode(latchPin, OUTPUT);
 	pinMode(clockPin, OUTPUT);
  	pinMode(dataPin, OUTPUT);
}

void Segment7_2p3inch_PH::setPin_com(uint8_t *com_pin){	
	
    for (int i = 0; i < com_num; i++) {
            control_pin[i] = com_pin[i];
    		pinMode(control_pin[i], OUTPUT);
  	}  
}

void Segment7_2p3inch_PH::divide_and_output(int input_value_L, int input_value_R, seg_show word_L, seg_show word_R){
  uint8_t dis[com_num];
  uint8_t value; 
  
  if(word_R == seg_num){
    for (int i = 0; i < com_num/2; i++) {
        dis[i] = input_value_R % 10;
        input_value_R = input_value_R / 10;
    }
  } else if (word_R == seg_done) {
        //donE = done
        dis[3] = sword_d;
        dis[2] = sword_o;
        dis[1] = sword_n;
        dis[0] = sword_E;
  } else if (word_R == seg_pause) { 
        //PAUE = pause
        dis[3] = sword_P;
        dis[2] = sword_A;
        dis[1] = sword_U;
        dis[0] = sword_E;
  } else if (word_R == seg_goal_xxxn){
        //units digit
        dis[3] = snum_0_word_O;
        dis[2] = snum_0_word_O;
        dis[1] = snum_0_word_O;
        uint8_t num = input_value_R%1000;
        dis[0] = num;
  } else if (word_R == seg_goal_xxxd){
        //units digit dark
        dis[3] = snum_0_word_O;
        dis[2] = snum_0_word_O;
        dis[1] = snum_0_word_O;
        dis[0] = sall_dark;
  } else if (word_R == seg_goal_xxnx){
        //tens digit
        dis[3] = snum_0_word_O;
        dis[2] = snum_0_word_O;
        uint8_t num = input_value_R/10;
        dis[1] = num;
        dis[0] = snum_0_word_O;
  } else if (word_R == seg_goal_xxdx){
        //tens digit dark
        dis[3] = snum_0_word_O;
        dis[2] = snum_0_word_O;
        dis[1] = sall_dark;
        dis[0] = snum_0_word_O;
  } else if (word_R == seg_goal_xnxx){
        // hundreds digit
        dis[3] = snum_0_word_O;
        uint8_t num = input_value_R/100;
        dis[2] = num;
        dis[1] = snum_0_word_O;
        dis[0] = snum_0_word_O;
  } else if (word_R == seg_goal_xdxx){
        // hundreds digit dark
        dis[3] = snum_0_word_O;
        dis[2] = sall_dark;
        dis[1] = snum_0_word_O;
        dis[0] = snum_0_word_O;
  } else if (word_R == seg_goal_nxxx){
        // thousands digit
        uint8_t num = input_value_R/1000;
        dis[3] = num;
        dis[2] = snum_0_word_O;
        dis[1] = snum_0_word_O;
        dis[0] = snum_0_word_O;
  } else if (word_R == seg_goal_dxxx){
        // thousands digit dark
        dis[3] = sall_dark;
        dis[2] = snum_0_word_O;
        dis[1] = snum_0_word_O;
        dis[0] = snum_0_word_O;
  } else if (word_R == seg_reset){
        dis[3] = snum_4_word_y;
        dis[2] = sall_dark;
        dis[1] = sword_n;
        dis[0] = sall_dark;
  } else if (word_R == seg_all_7seg_dark) {
        dis[3] = sall_dark;
        dis[2] = sall_dark;
        dis[1] = sall_dark;
        dis[0] = sall_dark;
  } 

  if(word_L == seg_num){
    for (int i = 4; i < com_num; i++) {
        dis[i] = input_value_L % 10;
        input_value_L = input_value_L / 10;
    }
  } else if (word_L == seg_done) {
        //donE = done
        dis[7] = sword_d;
        dis[6] = sword_o;
        dis[5] = sword_n;
        dis[4] = sword_E;
  } else if (word_L == seg_pause) { 
        //PAUE = pause
        dis[7] = sword_P;
        dis[6] = sword_A;
        dis[5] = sword_U;
        dis[4] = sword_E;
  } else if (word_L == seg_goal) {
        dis[7] = sword_G;
        dis[6] = snum_0_word_O;
        dis[5] = sword_A;
        dis[4] = sword_L;
  } else if (word_L == seg_reset){
        //reset = rESt
        dis[7] = sword_r;
        dis[6] = sword_E;
        dis[5] = snum_5_word_S;
        dis[4] = sword_t;
  } else if (word_L == seg_all_7seg_dark) {
        dis[7] = sall_dark;
        dis[6] = sall_dark;
        dis[5] = sall_dark;
        dis[4] = sall_dark;
  } 


    
    //display
  for (int i = 0; i < com_num; i++) {
    digitalWrite(latchPin, LOW);
    value = pgm_read_byte_near(SEG7 + dis[i]);    //read data from Flash that give SRAM to use 
    //shiftOut(dataPin, clockPin, LSBFIRST, SEG7[dis[i]]);    
    shiftOut(dataPin, clockPin, LSBFIRST, value);    
    for (int j = 0; j < com_num; j++) {           
      if (j == i ) {
		digitalWrite(control_pin[j], HIGH);  
      } else {       
		 digitalWrite(control_pin[j], LOW) ;
      }
    }
    digitalWrite(latchPin, HIGH);
    delay(2);
  }
  for (int j = 0; j < com_num; j++) {           //refreshing the display
    digitalWrite(control_pin[j], LOW);   
  }
}







