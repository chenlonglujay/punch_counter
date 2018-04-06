//--------------------------------------
//Seven-segment control test 
//use arduino Mega 2560
//8 Seven-segment displays
//0000->9999->donE->PAUE
//1sec refresh
//--------------------------------------
#include <Segment7_2p3inch_PH.h>
#include <Timer.h>
#define TimerSmallestUnit 100 //ms

Segment7_2p3inch_PH s7;
Timer t1;
int timer_counter = 0;
int S7L=0,S7R=0;
uint8_t S7LW =0,S7RW =0;

//74HC595
const byte dataPin = 2;
const byte latchPin = 3;
const byte clockPin = 4;

//2SK611,control by gate
//high為OFF low為ON
//gate sets 1,Seven-segment ON
//gate sets 0,Seven-segment OFF
uint8_t ctrl_com_pin[com_num];

void setup() {
  Serial.begin(9600);

  for (int i = 49; i > 41 ; i--) {
    ctrl_com_pin[abs(i - 49)] = i;
  }
  s7.setPin_7seg(dataPin, latchPin, clockPin);
  s7.setPin_com(ctrl_com_pin);
   t1.every(TimerSmallestUnit,timerEvent);
}

void loop() {
    t1.update();
    s7.divide_and_output(S7R, S7L, S7RW, S7LW);          
}

void timerEvent() {
  timer_counter ++;
  if (timer_counter == 10) { 
    //1 sec   
   dataL();
   dataR();
    timer_counter = 0;
  }  
}

void dataL() {
  
  if (S7L == 9999) {
        S7LW++;
        if(S7LW > 2) {
          S7LW = 0;
           S7L = 0;
         } 
    } else {
       S7L = S7L +1111;
   }    
}

void dataR() {
  
  if (S7R == 9999) {
        S7RW++;
        if(S7RW > 2) {
          S7RW = 0;
           S7R = 0;
         } 
    } else {
       S7R = S7R +1111;
   }    
}





