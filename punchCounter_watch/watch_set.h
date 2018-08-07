#ifndef WATCH_SET_H
#define WATCH_SET_H

#include <punchCounterWatch.h>
#include <Thread.h>
#include <Timer.h>
#include <ThreadController.h>
#include <punchBT_slave.h>
#include <punchOLED.h>
#include "OLED_database_watch.c"

enum watch_left_right{wLeft = 0, wRight} ;
#define module_LR wLeft      //if this module is right module, please change this parameter become wRight
#if module_LR
  #define punch_RL punch_R                
  #define left_right right              
#else if  
  #define punch_RL punch_L                
  #define left_right left               
#endif
#define show_time_serialMointor 0

punchBT_slave punch_RL;
punchCounterWatch PH_watch;
Timer tp;
punchOLED punch_OLED;

// ThreadController that will controll all threads
ThreadController controll = ThreadController();
//Thread BT receiver 
Thread* Thread_BT_receive = new Thread();
//Thread BT transmit
Thread* Thread_BT_transmit = new Thread();
//Thread I2C OLED
Thread* Thread_OLED = new Thread();

#define TimerSmallestUnit 100
#define start_pause_btn_pin 2      
#define reset_page_btn_pin 3      //reset need press button 3 seconds

#define AT_Mode 0       //first,you need use AT_Mode to pair BT,please refer to BT_pair document
#define slave_Mode 1   //second, use slave_Mode to recrive data from master or transmit data to master

#define sensitivity_pin A1
#define battery_detect_pin  A0
#define reset_check_arrive 30             //timer, TimerSmallestUnit  * reset_check_arrive
#define second_check_arrive 10        //timer, TimerSmallestUnit  * second_check_arrive
#define tenMin_check_arrive  600      //600s = 10 mintues


//change switch  status  of  start/pause button
bool switchCheck_sp = true;

//first time to press reset/page button
bool reset_page_is_start = false;    

//timer count for some event check
int timer_count = 0;
int timer_reset_page_count = 0;         //count press button 3 seconds

//when power on avoids system go into reset_page_ISR() function
bool start_power_on = true;

//save punch count now and before
int punchCountBF = 0;
int punchCountNow = 0;

int punchGoal = punch_goal_default ;

 //pause mode only save punch counter and time data to eeprom once
bool update_once = false;        

//check is playerno used watch in 10 minutes?if no used,watch will change mode become pause mode automatically  
//this function works decide by auto_pause_switch
#define auto_pause_switch false
bool autoPause = false;
unsigned int timer_count_10Min = 0;

uint8_t before_page;
uint8_t now_page;
#endif
