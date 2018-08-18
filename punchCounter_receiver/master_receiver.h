#ifndef MASTER_RECEIVER_h
#define MASTER_RECEIVER_h
//Master:Arduino Mega 2560

//----------------------------------------------------
//BT:
//blue tooth(HC05) module
//hardware serial
//RX1 19 TX1 18
//RX2 17 TX2 16
//RX3 15 TX3 14
#include <punchBT_master.h>
#define AT_Mode 0
#define Master_Mode 1
char val_right;
char val_left;
//data format 
//left arm lxxxx. ex:r1000.
//right arm rxxxx. ex:r0888.
//!!First,you need use AT_Mode to pair BT echo other(master and slave),please refer to BT_pair document!!
punchBT_master punch_BT_R,punch_BT_L;
bool already_resetL = false;
bool already_resetR = false;
//----------------------------------------------------


//----------------------------------------------------
//seven segment
#include <Segment7_2p3inch_PH.h>
#define SEG7_test_mode 1
//74HC595
#define data_Pin 5
#define latch_Pin 6
#define clock_Pin 7
//49(c7)-41(c0)
#define Segcom_Low 49
#define Segcom_High 41
bool seg_change = 0;
int S7L_vaule = 0, S7R_vaule = 0;
uint8_t S7L_states = 0, S7R_states =0;
//#define  RL_total_goal 1000
//----------------------------------------------------

//----------------------------------------------------
//Timer
#include <Timer.h>
#define TimerSmallestUnit 100 //ms
Timer t1;
int red_timer_counter = 0;
#define reset_check_arrive 30            //timer, TimerSmallestUnit  * reset_check_arrive
int  green_timer_counter = 0;
#define setGoal_arrive 30            //timer, TimerSmallestUnit  * setGoal_arrive
int  sw_timer_counter = 0;
#define set_sw_arrive 5            //timer, TimerSmallestUnit  * set_sw_arrive
//----------------------------------------------------

//----------------------------------------------------
//mp3 player
#define volume_knob_pin A0
#define mp3_button_pin 2  //on/off mp3 player 
#define mp3_test_mode  0           //test every 5 secons  will play mp3 once
bool mp3_button_status = 0; 
bool now_play_next = 0;
//----------------------------------------------------

//----------------------------------------------------
#include <Thread.h>
#include <ThreadController.h>
// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//Thread BT receiver 
Thread* Thread_BT_receive = new Thread();
/*
//Thread BT transmit
Thread* Thread_BT_transmit = new Thread();
*/
//Thread seven segment
Thread* Thread_SEG7 = new Thread();
Thread* Thread_mp3 = new Thread();
//----------------------------------------------------

//----------------------------------------------------
//button
#define red_button_pin 19 

/*    
red:
press red button once switch punch total goal/now RL punch count increase/ now RL punch countdown
press 3 seconds  show as below
rESt 
y n
press red button once again will clean total and now count data
press green  button will cancel this reset
*/
#define green_button_pin 18
/*    
green
press green button once will play voice mp3(1->last loop)
press 3 seconds  show as below(!!must be set, can not cancel!!)
GOAL
xxxx
press green button that can increase number(0-9 loop)
press red  button that can choose thousands,hundreds,tens,units digit
press red button 3 seconds again will finish this setting
press green button 3 seconds again will cancel this setting
*/

//----------------------------------------------------
bool check_red_button_reset = 0;    //use to prevent to press red button already 3 seconds and let the  button release will go into reset
bool check_green_button_set = 0;    //use to prevent to press green button already 3 seconds and let the  button release will go into set

bool start_power_on1 = 1;  //red button,prevent power on will go into interrupt ISR 
bool start_power_on2 = 1;  //green button,prevent power on will go into interrupt ISR
bool start_power_on3 = 1;  //mp3player power button prevent power on will go into interrupt ISR
#endif
