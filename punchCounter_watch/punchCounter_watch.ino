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

#define sensitivity_pin A0
//#define start_pause_LED_pin 4       //start led ON, pause led OFF 
#define battery_detect_pin  A1
#define reset_check_arrive 30             //timer, TimerSmallestUnit  * reset_check_arrive
#define second_check_arrive 10        //timer, TimerSmallestUnit  * second_check_arrive
#define battery_check_arrive 300         //timer,  TimerSmallestUnit * battery_check_arrive
#define tenMin_check_arrive  600      //600s = 10 mintues

//change switch  status  of  start/pause button
bool switchCheck_sp = true;

//first time to press reset/page button
bool reset_page_is_start = false;    

 //check system is or not executed all reset function
bool resetAllDone = false;

//timer count for some event check
int timer_count = 0;
int timer_reset_page_count = 0;         //count press button 3 seconds

//when power on avoids system go into reset_page_ISR() function
bool start_power_on = true;

//save punch count now and before
int punchCountBF = 0;
int punchCountNow = 0;

 //pause mode only save date to eeprom once
bool update_once = false;        

//check battery
int battery_count = 0;
int battery_percent = 0;

//check player is no used watch in 10 minutes,if no used,watch will change mode become pause mode automatically  
bool autoPause = false;
unsigned int timer_count_10Min = 0;

enum OLED_show_page{sPage1=0 , sPage2, sPage_reset} ;
uint8_t OLED_SP= sPage1 ; //show page default sets page1
int test_punch_count = 99;

void setup() {
  Serial.begin(9600);  
  punchCounter_initial();
#if slave_Mode   
  PH_watch.punchCounterWatch_initial_set(sensitivity_pin, battery_detect_pin);
  //pinMode(start_pause_LED_pin, OUTPUT);
  tp.every(TimerSmallestUnit,timerEvent);    
  interrupt_initial();
  thread_initial();
  OLED_initial();
 #endif  
}

void loop() {   
#if AT_Mode  
        punch_RL.AT_mode_function();
#else if slave_Mode    
  controll.run();
  tp.update(); 
 #endif
}

void timerEvent() {          
          if(PH_watch.get_start_pause_status()) {
            timer_count++;        
                if(timer_count == second_check_arrive) {
                      //TimerSmallestUnit *second_check_arrive = 1 second
                      PH_watch.timer_add_1_second();
                      update_punch_timer();                 
                      timer_count_10Min++;     
                      test_punch_count = test_punch_count+1 ;
                      if(test_punch_count == 9999) {
                        test_punch_count = 0;
                      }
                      if((timer_count_10Min >= tenMin_check_arrive) && autoPause) {
                           transmit_pause(&punch_RL);         //change mode become to pause mode
                           clear_auto_pasue();
                      }
                }              
          } else {    
              if(update_once) { 
                update_punch_timer();     //pause mode only save date to eeprom once
              }
              update_once = false;
          }         
          arrange_reset_page();          
          check_battery_percent();
}

void check_battery_percent() {
      battery_count++;
      if(battery_count == battery_check_arrive ){ 
          //TimerSmallestUnit *battery_check_arrive = 30 seconds                    
          battery_percent = PH_watch.get_battery_percent();
          Serial.print(" battery_percent: ");     
          Serial.println(battery_percent);     
          battery_count = 0;
      }
}

void update_punch_timer() {         
         PH_watch.savePunchCountToEEPROM();
         PH_watch.saveTimerDataToEEPROM();
 
#if show_time_serialMointor
         showTimeData();      
#endif
          timer_count = 0;  
}

void arrange_reset_page() {
          if(reset_page_is_start) {
                  timer_reset_page_count++;              
                    if(timer_reset_page_count >= reset_check_arrive) {                        
                          //TimerSmallestUnit *reset_check_arrive = 3 seconds
                            reset_data_count();
                    } 
              } else if (!reset_page_is_start && timer_reset_page_count != 0) {
                    if(!resetAllDone) {
                      //after resetAll finished ,avoids into changing page                      
                        //Serial.println("changing page");       
                         //punchCountNow++;         //ivan test        
                         punch_OLED.clear_screen();   
                         OLED_SP = !OLED_SP;               
                         timer_reset_page_count = 0;          
                    }
                    resetAllDone = false;
                    timer_reset_page_count = 0;
              }
}

void reset_data_count() {
          PH_watch.resetAll();
           Serial.println("resetAll");
           timer_reset_page_count = 0;
           resetAllDone = true;                                                          
           reset_data(&punch_RL);                        //transmit count=0
           transmit_data(&punch_RL, 0, false);          
           transmit_pause(&punch_RL);               //change mode become to pause mode
           update_once = true;
           OLED_SP = sPage_reset; 
           test_punch_count = 0;
}

void interrupt_initial() {
          digitalWrite(reset_page_btn_pin, HIGH); //turn on pullup resistors
          digitalWrite(start_pause_btn_pin, HIGH);  //turn on pullup resistors
          attachInterrupt(digitalPinToInterrupt(reset_page_btn_pin), reset_page_ISR, CHANGE);
          attachInterrupt(digitalPinToInterrupt(start_pause_btn_pin), start_pause_ISR, FALLING);          
}

void reset_page_ISR() {
         if(!start_power_on) {
              static unsigned long last_interrupt_time = 0;
              unsigned long interrupt_time = millis();     
              if (interrupt_time - last_interrupt_time > 50) {           
                  reset_page_is_start = !reset_page_is_start;                                       
              }
              last_interrupt_time = interrupt_time;
         }
        start_power_on = false;
}

void start_pause_ISR() {
            static unsigned long last_interrupt_time = 0;
            unsigned long interrupt_time = millis();
            if(interrupt_time - last_interrupt_time > 200) {                  
                if(switchCheck_sp == watch_start) {
                    //Serial.println("start");
                    //digitalWrite(start_pause_LED_pin, LOW);     //LED ON
                    PH_watch.set_start();
                    switchCheck_sp = watch_pause;       //next push start/pause button down will go into pause
                } else if(switchCheck_sp == watch_pause)  {
                   //Serial.println("pause");
                   //digitalWrite(start_pause_LED_pin, HIGH);       //LED OFF
                   transmit_pause(&punch_RL);         //change mode become to pause mode
                   update_once = true;
                }
            }
            last_interrupt_time = interrupt_time;
}

#if show_time_serialMointor
void showTimeData() {
          timerSaveFMT *TS = new timerSaveFMT;
          *TS = PH_watch.getTimeData();
          Serial.println("dd:hh:mm:ss");
          Serial.print(TS->day);
          Serial.print(": ");
          Serial.print(TS->hour);
          Serial.print(": ");
          Serial.print(TS->minute);
          Serial.print(": ");
          Serial.println(TS->second);
          delete TS;
}
#endif

void punchCounter_initial() {
#if AT_Mode
        punch_RL.punchBT_slave_initial_set(AT_mode, left_right);         
          Serial.println("AT_mode");
#else slave_Mode  
        punch_RL.punchBT_slave_initial_set(Slave_mode, left_right);       
        battery_percent = PH_watch.get_battery_percent();
        //digitalWrite(start_pause_LED_pin, HIGH);       //LED OFF
#endif  
}

void thread_initial() {
  // Configure Thread_BT_transmit
  Thread_BT_transmit->onRun(BT_transmit);
  Thread_BT_transmit->setInterval(250);
  
  // Configure Thread_BT_receive
  Thread_BT_receive->onRun(BT_receive);
  Thread_BT_receive->setInterval(250);
  
 // Configure Thread_OLED
  Thread_OLED->onRun(OLED_display);
  Thread_OLED->setInterval(250);

  // Adds all threads to the controller
  controll.add(Thread_BT_transmit);
  controll.add(Thread_BT_receive);
  controll.add(Thread_OLED);
}

void BT_transmit() {
  get_count_transmitData(&punch_RL);
}

void BT_receive() {
  bool receive_status;
  receive_status = punch_RL.Slave_mode_receive_reset();
  if(receive_status == reset_yes){
    reset_data_count();
  }
}

void get_count_transmitData(punchBT_slave *input) {
  bool show_data = false;
 int sensitivity;
  if(PH_watch.get_start_pause_status()){       
        punchCountNow  = PH_watch.getHumanPunchCount(); 
        punchCountNow = test_punch_count;    
         //Serial.print("punchCount: ");
         //Serial.println(punchCountNow);
          sensitivity = PH_watch.get_sensitivity_percent();
         //Serial.print("sensitivity: ");
         //Serial.println(sensitivity);
         
        if(punchCountNow > punchCountBF) {
            clear_auto_pasue();
        } else if (punchCountNow == punchCountBF)  {
            autoPause = true;
        }
                
        if(input->get_transmitData() == 9999) { 
             reset_data(input);
         }
        transmit_data(input, punchCountNow, show_data);        
        punchCountBF = punchCountNow;
    } else {      
      //Serial.println("puase");
    }
}

void OLED_display() {
  //int punch_count = punchCountNow;
  int punch_count = test_punch_count;
  punch_count_OLED *pco = new punch_count_OLED;
  time_save_OLED *tso = new time_save_OLED;
  timerSaveFMT *TS = new timerSaveFMT;
  *TS = PH_watch.getTimeData();
   tso->day_h = TS->day / 10;
   tso->day_l = TS->day % 10;
   tso->hour_h = TS->hour / 10;
   tso->hour_l = TS->hour % 10;
   tso->minute_h = TS->minute / 10;
   tso->minute_l =  TS->minute % 10;
   tso->second_h = TS->second / 10;
   tso->second_l = TS->second % 10;
               
  pco->THD =  punch_count / 1000;  
 //Serial.print(pco->THD);
 punch_count =punch_count % 1000;
 
  pco->HUD = punch_count / 100;
  //Serial.print(pco->HUD);
  punch_count =punch_count % 100;
  
  pco->TEN =  punch_count / 10;
  //Serial.print( pco->TEN);
 punch_count =punch_count % 10;
   
  pco->ONE = punch_count % 10;;
 //Serial.println( pco->ONE);
 
  if(OLED_SP == sPage1) {
      punch_OLED.show_watch_page1(pco, tso);  
  } else if (OLED_SP == sPage2) {
      pco->HUD = 0;
      pco->TEN = 3;
      pco->ONE = 0;
      punch_OLED.show_watch_page2(1,  module_LR, !switchCheck_sp, pco);     
      //1 bool not0/charge1,
      //2 bool left0/right1 watch, 
      //3 bool pause0/start1 mode, 
      //4 punch count sensitivity(0~100%)
  } else {
      punch_OLED.clear_screen();
      punch_OLED.show_watch_reset();
      delay(1000);
      punch_OLED.clear_screen();
      OLED_SP = sPage1;      
  }
  delete pco;
  delete tso;
  delete TS;
}

void reset_data(punchBT_slave *input) {
    input->set_transmitData(0);  
    punchCountNow = 0;
}

void transmit_data(punchBT_slave *input, int count, bool show_data) {
      input->set_transmitData(count); 
      input->set_punch_pause(punch);  
      input->Slave_mode_transmit(show_data);    
}

void transmit_pause(punchBT_slave *input) {
     PH_watch.set_pause();                      //work likes pause mode     
     input->set_punch_pause(pause);
     input->Slave_mode_transmit(false);          
     //digitalWrite(start_pause_LED_pin, HIGH);       //LED OFF
     switchCheck_sp = watch_start;       //next push start/pause button down will go into start  
}

void clear_auto_pasue() {
  autoPause = false;
  timer_count_10Min = 0;
}

void  OLED_initial() {
  punch_OLED.punchOLED_initial();
  delay(10);
  punch_OLED.clear_screen();
  punch_OLED.showPicture128x64_lab();
  delay(2000);  
  punch_OLED.clear_screen();
  punch_OLED.showPicture128x64_fist();
  delay(2000);  
  punch_OLED.clear_screen();  
}
