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
//GOAL(L)
//0008(R)

//function5:
//GOAL(L)
//0070(R)

//function6:
//GOAL(L)
//0600(R)

//function7:
//GOAL(L)
//5000(R)

//function8:
//rESt(L)
//yxnx(R)

//function9: blink
//GOAL(L)
//0001(R)
//1sec refresh
//GOAL(L)
//000d(R) dark

//function10: blink
//GOAL(L)
//0020(R)
//1sec refresh
//GOAL(L)
//00d0(R) dark

//function11: blink
//GOAL(L)
//0300(R)
//1sec refresh
//GOAL(L)
//0d00(R) dark

//function12: blink
//GOAL(L)
//4000(R)
//1sec refresh
//GOAL(L)
//d000(R) dark

//function13: 
//GOAL(L)
//9998(R)
//--------------------------------------
#include <Segment7_2p3inch_PH.h>
#include <Timer.h>
#define TimerSmallestUnit 100 //ms
uint8_t function_set = 13; //test function

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
      s7.divide_and_output(0, 8,  seg_goal, seg_goal_xxxn);   
     break;  
     case 5:
      s7.divide_and_output(0, 70,  seg_goal, seg_goal_xxnx);   
     break;  
     case 6:
      s7.divide_and_output(0, 600,  seg_goal, seg_goal_xnxx);   
     break;  
     case 7:
      s7.divide_and_output(0, 5000,  seg_goal, seg_goal_nxxx);   
     break;  
    case 8:
      s7.divide_and_output(0, 0,  seg_reset, seg_reset);   
     break;  
    case 9:
      s7.divide_and_output(0, 1, seg_goal, S7RW);   
     break;  
    case 10:
      s7.divide_and_output(0, 20, seg_goal, S7RW);   
     break;  
    case 11:
      s7.divide_and_output(0, 300, seg_goal, S7RW);   
     break;
    case 12:
      s7.divide_and_output(0, 4000, seg_goal, S7RW);   
     break;
     case 13:
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
        case 9:
          function9();
        break;
           case 10:
          function10();
        break;
           case 11:
          function11();
        break;
           case 12:
          function12();
        break;
      }
 
    timer_counter = 0;
  }  
}

void function1_L() {
  
  if (S7L == 9999) {
        S7LW++;
        if(S7LW>2 && S7LW<13) {
          S7LW = 13;   //all dark
        } else if(S7LW  == 14) {
          S7LW = 12;   //reset
        } else  if(S7LW  == 13) {
          S7LW = 0;
          S7L = 0;
         }
    } else {
       S7L = S7L +1111;
   }    
}

void function1_R() {
  
  if (S7R == 9999) {
        S7RW++;
        if(S7RW>2 && S7RW<13) {
          S7RW = 13;   //all dark
         } else if(S7RW == 14) {
          S7RW = 12;   //reset
         }else if(S7RW  == 13) {
          S7RW = 0;
          S7R= 0;
         }
    } else {
       S7R = S7R +1111;
   }    
}

void function9() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_goal_xxxn;
     } else {
      S7RW = seg_goal_xxxd;
     }  
}

void function10() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_goal_xxnx;
     } else {
      S7RW = seg_goal_xxdx;
     }  
}

void function11() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_goal_xnxx;
     } else {
      S7RW = seg_goal_xdxx;
     }  
}

void function12() {
    seg_change = !seg_change;
     if(seg_change == 0 ){
      S7RW = seg_goal_nxxx;
     } else {
      S7RW = seg_goal_dxxx;
     }  
}


