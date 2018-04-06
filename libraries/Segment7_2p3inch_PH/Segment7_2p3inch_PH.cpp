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

void Segment7_2p3inch_PH::divide_and_output(int input_value_R, int input_value_L, uint8_t word_R, uint8_t word_L){
  uint8_t dis[com_num];
  uint8_t value; 
  
  if(word_R == 0){
    for (int i = 0; i < com_num/2; i++) {
        dis[i] = input_value_R % 10;
        input_value_R = input_value_R / 10;
    }
  } else if (word_R == 1) {
        //donE = done
        dis[0] =0X0D;
        dis[1] =0X0C;
        dis[2] =0X0B;
        dis[3] =0X0A;
  } else if (word_R == 2) { 
        //PAUE = pause
        dis[0] =0X0D;
        dis[1] =0X10;
        dis[2] =0X0F;
        dis[3] =0X0E;
  }

  if(word_L == 0){
    for (int i = 4; i < com_num; i++) {
        dis[i] = input_value_L % 10;
        input_value_L = input_value_L / 10;
    }
  } else if (word_L == 1) {
        //donE = done
        dis[4] =0X0D;
        dis[5] =0X0C;
        dis[6] =0X0B;
        dis[7] =0X0A;
  } else if (word_L == 2) { 
        //PAUE = pause
        dis[4] =0X0D;
        dis[5] =0X10;
        dis[6] =0X0F;
        dis[7] =0X0E;
  }  

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







