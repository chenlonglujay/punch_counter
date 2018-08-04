//--------------------------------------
//Seven-segment control test 
//use arduino Mega 2560
//8 Seven-segment displays

//function1:
//0000->1111->...->9999->donE->PAUE->all dark->rESt
//1sec refresh

//function2:
//done(L)
//PAUE(R)

//function3:
//PAUE(L)
//done(R)

//function4:
//rESt(L)
//yxnx(R)

//function5: blink
//GOAL(L)
//4321(R)
//1sec refresh
//GOAL(L)
//432d(R) dark

//function6: blink
//GOAL(L)
//3210(R)
//1sec refresh
//GOAL(L)
//32d0(R) dark

//function7: blink
//GOAL(L)
//8765(R)
//1sec refresh
//GOAL(L)
//8d65(R) dark

//function8: blink
//GOAL(L)
//4567(R)
//1sec refresh
//GOAL(L)
//d567(R) dark

//function9: 
//GOAL(L)
//9998(R)
//--------------------------------------
#include <Segment7_2p3inch_PH.h>
#include <Timer.h>
#define TimerSmallestUnit 100 //ms
uint8_t function_set = 1; //test function 1~9

Segment7_2p3inch_PH s7;
Timer t1;
int timer_counter = 0;
int S7L=0,S7R=0;
uint8_t S7LW =0,S7RW =0;

//74HC595
const byte dataPin = 5;
const byte latchPin = 6;
const byte clockPin = 7;

//2SK611,control by gate
//high為OFF low為ON
//gate sets 1,Seven-segment ON
//gate sets 0,Seven-segment OFF
uint8_t ctrl_com_pin[com_num];
bool seg_change = 0;

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
 switch (function_set)
 {
    case 1:
      s7.divide_and_output(S7L, S7R, S7LW, S7RW);   
     break;  
     case 2:
      s7.divide_and_output(0, 0,  seg_done, seg_pause);   
     break;  
     case 3:
      s7.divide_and_output(0, 0,  seg_pause, seg_done);   
     break;  
    case 4:
      s7.divide_and_output(0, 0,  seg_reset, seg_reset);   
     break;  
    case 5:
      s7.divide_and_output(0, 4321, seg_goal, S7RW);   
     break;  
    case 6:
      s7.divide_and_output(0, 3210, seg_goal, S7RW);   
     break;  
    case 7:
      s7.divide_and_output(0, 8765, seg_goal, S7RW);   
     break;
    case 8:
      s7.divide_and_output(0, 4567, seg_goal, S7RW);   
     break;
     case 9:
      s7.divide_and_output(0, 9998, seg_goal, seg_goal);   
     break;
  }
}

void timerEvent() {
  timer_counter ++;
  if (timer_counter == 10) { 
    //1 sec   
     switch (function_set)
      {
        case 1:
         function1_L();
         function1_R();
        break;
        case 5:
          function5();
        break;
           case 6:
          function6();
        break;
           case 7:
          function7();
        break;
           case 8:
          function8();
        break;
      }
 
    timer_counter = 0;
  }  
}

void function1_L() {
  //0000->1111->...->9999->donE->PAUE->all dark->rESt
  if (S7L == 9999) {
        S7LW++;
        if(S7LW>2 && S7LW<seg_all_7seg_dark) {
          S7LW = seg_all_7seg_dark;   //all dark
        } else if(S7LW  == (seg_all_7seg_dark+1)) {
          S7LW = seg_reset;   //reset
        } else  if(S7LW  == seg_all_7seg_dark) {
          S7LW = 0;
          S7L = 0;
         }
    } else {
       S7L = S7L +1111;
   }    
}

void function1_R() {
    //0000->1111->...->9999->donE->PAUE->all dark->rESt
  if (S7R == 9999) {
        S7RW++;
        if(S7RW>seg_pause && S7RW<seg_all_7seg_dark) {
          S7RW = seg_all_7seg_dark;   //all dark
         } else if(S7RW == (seg_all_7seg_dark+1)) {
          S7RW = seg_reset;   //reset
         }else if(S7RW  == seg_all_7seg_dark) {
          S7RW = 0;
          S7R= 0;
         }
    } else {
       S7R = S7R +1111;
   }    
}

void function5() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_num;
     } else {
      S7RW = seg_goal_xxxd;
     }  
}

void function6() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_num;
     } else {
      S7RW = seg_goal_xxdx;
     }  
}

void function7() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_num;
     } else {
      S7RW = seg_goal_xdxx;
     }  
}

void function8() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_num;
     } else {
      S7RW = seg_goal_dxxx;
     }  
}


