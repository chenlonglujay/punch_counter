#include "watch_set.h"

void setup() {
  Serial.begin(9600);  
#if slave_Mode   
  timer_count = 0;
  timer_reset_page_count = 0;
  PH_watch.punchCounterWatch_initial_set(sensitivity_pin, battery_detect_pin);
  punchCounter_initial();
  tp.every(TimerSmallestUnit, timerEvent_start_pause);    
  tc.every(TimerSmallestUnit, timerEvent_reset_page);  
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
  tc.update(); 
 #endif
}

void timerEvent_start_pause() {          
          if(PH_watch.get_start_pause_status()) {
            timer_count++;        
                if(timer_count == second_check_arrive) {
                      //TimerSmallestUnit *second_check_arrive = 1 second
                      PH_watch.timer_add_1_second();
                       timer_count = 0;  
#if show_time_serialMointor
         showTimeData();      
#endif                             
 #if  auto_pause_switch                        
                      timer_count_10Min++;                                           
                      if((timer_count_10Min >= tenMin_check_arrive) && autoPause) {
                           transmit_pause(&punch_RL);         //change mode become to pause mode
                           clear_auto_pasue();
                      }
 #endif
                }              
          }                   
}

void timerEvent_reset_page(){
    arrange_reset_page();  
    timerEvent_detect_punch();
}

void update_punch_timer() {         
         PH_watch.savePunchCountToEEPROM();
         PH_watch.saveTimerDataToEEPROM();         
}

void timerEvent_detect_punch(){
  if(!PH_watch.get_punch_detect()) {
    timer_detcet_punch_count ++;
      if( timer_detcet_punch_count == detcet_punch_goal ) {
      //Serial.println("set detect 1");
        PH_watch.set_punch_detect(1);
        timer_detcet_punch_count = 0;
      }
  }
}

void arrange_reset_page() {  
          wh_page pp = PH_watch.get_page_count();
          now_page = pp;
          if(now_page == before_page && digitalRead(reset_page_btn_pin) == 0) {
            //still press button
                  timer_reset_page_count++;              
                    if(timer_reset_page_count >= reset_check_arrive) {                        
                          //TimerSmallestUnit *reset_check_arrive = 3 seconds
                            PH_watch.set_which_page(page_reset);
                    } 
          } else if(now_page == before_page && digitalRead(reset_page_btn_pin) ==1) {
               timer_reset_page_count = 0;            
          }
          before_page = now_page;      
}

void reset_data_count() {
         //Serial.println("resetAll");
          PH_watch.resetAll();       
           timer_reset_page_count = 0;                                                    
           reset_data(&punch_RL);                        //transmit count=0
           transmit_data(&punch_RL, 0, false);          
           transmit_pause(&punch_RL);               //change mode become to pause mode
           punchGoal = punch_goal_default;
           PH_watch.savePunchGoalToEEPROM(punchGoal);  
           update_once = true;
           PH_watch.set_transmit_BT_reset(transmit_0_not);
           punchCountNow = 0;
}

void interrupt_initial() {
          digitalWrite(reset_page_btn_pin, HIGH); //turn on pullup resistors
          digitalWrite(start_pause_btn_pin, HIGH);  //turn on pullup resistors
          attachInterrupt(digitalPinToInterrupt(reset_page_btn_pin), reset_page_ISR, FALLING);   
          attachInterrupt(digitalPinToInterrupt(start_pause_btn_pin), start_pause_ISR, FALLING);          
}

void reset_page_ISR() {
         if(!start_power_on) {
              static unsigned long last_interrupt_time = 0;
              unsigned long interrupt_time = millis();     
              if (interrupt_time - last_interrupt_time > 200) {                 
                   PH_watch.add_page();                              
              }
              last_interrupt_time = interrupt_time;
         }
        start_power_on = false;
}

void start_pause_ISR() {
            static unsigned long last_interrupt_time = 0;
            unsigned long interrupt_time = millis();
            if(interrupt_time - last_interrupt_time > 200) {           
                if( PH_watch.check_goal_done(punchGoal , punchCountNow) == goal_not_yet) {       
                          if(switchCheck_sp == watch_start) {
                              //Serial.println("start");            
                              PH_watch.set_start();
                              switchCheck_sp = watch_pause;       //next push start/pause button down will go into pause
                          } else if(switchCheck_sp == watch_pause)  {
                             //Serial.println("pause");     
                             transmit_pause(&punch_RL);         //change mode become to pause mode
                             update_once = true;
                          }
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
#else slave_Mode  
        punch_RL.punchBT_slave_initial_set(Slave_mode, left_right);      
        punchCountNow  = PH_watch.get_punchCounter();       
        punchCountBF = punchCountNow;
 #if    use_test_add_punch_count
          punchCountNow = punchCountNow + test_add_punch_count;
 #endif        
          punchGoal = PH_watch.get_punchGoal(); 
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
  dw_status dws;
  dws = punch_RL.Slave_mode_receive_goal_or_reset();
  switch (dws) {
    case do_reset:
        //Serial.println(F("do_reset"));
        PH_watch.set_which_page(page_reset);
        reset_data_count();
   break;   
   case get_goal:   
    //Serial.println(F("get goal"));
        punchGoal = punch_RL.get_goal_value();    //from BT receives data of goal
        if(punchGoal == 0 || punchGoal >punch_goal_default ) {
          punchGoal = punch_goal_default;
        }
        PH_watch.savePunchGoalToEEPROM(punchGoal);     
        //Serial.println(F("------------------------savePunchGoalToEEPROMl-----------------"));
        //Serial.println(punchGoal);
    break;                                                                                                                                                                                                                      
   case  nothing: 
           //Serial.println(F("nothing"));
             if(PH_watch.get_start_pause_status()) {
                update_punch_timer();
            } else {    
              if(update_once) { 
                update_punch_timer();     //pause mode only save punch counter and time data to eeprom once
              }
              update_once = false;
          }             
    break;
  }  
}

void get_count_transmitData(punchBT_slave *input) {
  bool show_data = false;
          
  if(PH_watch.get_start_pause_status()){       
        if(PH_watch.get_punch_pause_switch_do_once_transmit()) {          
            for(int i =0; i <20; i++){
                transmit_data(input, punchCountBF, show_data);
            }
            PH_watch.set_punch_pause_switch_do_once_transmit();
        }
        punchCountNow  = PH_watch.getHumanPunchCount();     
 #if    use_test_add_punch_count
          punchCountNow = punchCountNow + test_add_punch_count;
 #endif                 
        if(punchCountNow > punchCountBF) {
#if  auto_pause_switch                     
            clear_auto_pasue();
#endif           
            transmit_data(input, punchCountNow, show_data);   
            punchCountBF = punchCountNow;         
        } else if (punchCountNow == punchCountBF)  {           
 #if  auto_pause_switch             
            autoPause = true;
 #endif
        }
        if( PH_watch.check_goal_done(punchGoal , punchCountNow) == goal_done){
                    //change mode become to pause mode
                     PH_watch.set_pause();                      //work likes pause mode     
                      input->set_punch_pause(pause);             
                      switchCheck_sp = watch_start;       //next push start/pause button down will go into start  
        }                     
    } 
    
    //if reset,slave transmit l0000. or r0000.
   if(PH_watch.get_page_count() == page_reset) {
      if(PH_watch.get_transmit_BT_reset() == transmit_0_not) {
          reset_data(input);
          PH_watch.set_transmit_BT_reset(transmit_0_done);
      }
   }
}

void OLED_display() {
  punch_count_digit *pcd = new punch_count_digit;
  punch_count_digit *power = new punch_count_digit;
  punch_count_digit *goal = new punch_count_digit;
  punch_count_digit *goal_sub_now = new punch_count_digit;
  punch_count_digit *sensitivity = new punch_count_digit;
  time_save_OLED *tso = new time_save_OLED;  
  timerSaveFMT *TS = new timerSaveFMT;

  //if change page need to clear OLED display
  if(PH_watch.change_page_check()){
    punch_OLED.clear_screen();
  } else {  
        if(PH_watch.get_page_count() == page1) {
            time_div_cul(tso, TS);
            pcd_div_cul(pcd);   
            punch_OLED.show_watch_page1(pcd, tso);  
        }else if(PH_watch.get_page_count() == page2) {          
            power_div_cul(power);
            sensitivity_div_cul(sensitivity);
            punch_OLED.show_watch_page2(power,  module_LR, !switchCheck_sp, sensitivity);     
            //1 power(0~100%)
            //2 bool left0/right1 watch, 
            //3 bool pause0/start1 mode, 
            //4 punch count sensitivity(0~100%)     
        } else if (PH_watch.get_page_count() == page3) {
            goal_countdown_div_cul(goal, goal_sub_now );
            pcd_div_cul(pcd);   
           punch_OLED.show_watch_page3(goal, goal_sub_now );   
        } else  if (PH_watch.get_page_count() == page_reset ){
            reset_data_count();
            punch_OLED.clear_screen();
            punch_OLED.show_watch_reset();
            delay(1000);
            punch_OLED.clear_screen();
            PH_watch.set_which_page(page1);
        }
  }
  delete sensitivity;
  delete power;
  delete goal_sub_now ;
  delete goal;
  delete pcd;
  delete tso;
  delete TS;
}

void time_div_cul(time_save_OLED *time_show_val, timerSaveFMT *time_input) {
   *time_input= PH_watch.getTimeData();   
   time_show_val->day_h = time_input->day / 10;
   time_show_val->day_l = time_input->day % 10;
   time_show_val->hour_h = time_input->hour / 10;
   time_show_val->hour_l = time_input->hour % 10;
   time_show_val->minute_h = time_input->minute / 10;
   time_show_val->minute_l =  time_input->minute % 10;
   time_show_val->second_h = time_input->second / 10;
   time_show_val->second_l = time_input->second % 10;
}

void power_div_cul(punch_count_digit *bp) {
  uint8_t power_battery =  PH_watch.get_battery_percent();
  //power_battery = 30; //test
   bp->THD = 0;
   bp->HUD = power_battery  /100;
   power_battery  = power_battery  % 100;
   bp->TEN =  power_battery   /10;
   bp->ONE = power_battery  % 10;     
}

void goal_countdown_div_cul(punch_count_digit * goal ,punch_count_digit  *goal_sub_now ) {
    //int goal_test = 1897;
    int goal_temp = punchGoal;
   //int now_test = 97;
    int goal_sub_now_temp = goal_temp - punchCountNow ;
    goal->THD = goal_temp /1000;
    goal_temp = goal_temp % 1000;
    goal->HUD = goal_temp / 100;
    goal_temp = goal_temp % 100;
    goal->TEN = goal_temp / 10;
    goal->ONE = goal_temp % 10; 
    
    goal_sub_now->THD = goal_sub_now_temp /1000;
    goal_sub_now_temp = goal_sub_now_temp % 1000;
    goal_sub_now->HUD = goal_sub_now_temp / 100;
    goal_sub_now_temp = goal_sub_now_temp  % 100;
    goal_sub_now->TEN = goal_sub_now_temp  / 10;
    goal_sub_now->ONE = goal_sub_now_temp  % 10; 

}

void pcd_div_cul(punch_count_digit  *pcd_val) { 
  int pouch_count_temp = punchCountNow; 
  pcd_val->THD =  pouch_count_temp / 1000;  
  pouch_count_temp = pouch_count_temp % 1000; 
  pcd_val->HUD = pouch_count_temp / 100;
  pouch_count_temp = pouch_count_temp% 100;  
  pcd_val->TEN =  pouch_count_temp/ 10;
  pcd_val->ONE = pouch_count_temp % 10;   
}

void  sensitivity_div_cul(punch_count_digit  *sen){
   uint8_t sen_temp = PH_watch.get_sensitivity_percent();  
   sen->THD = 0;
   sen->HUD = sen_temp / 100;
   sen_temp = sen_temp %100;
   sen->TEN = sen_temp /10;
   sen->ONE = sen_temp %10;  
}

void reset_data(punchBT_slave *input) {
    input->set_transmitData(0);  
    punchCountNow = 0;
    punchCountBF = 0;
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
  delay(1500);  
  punch_OLED.clear_screen();
  punch_OLED.showPicture128x64_fist();
  delay(1500);  
  punch_OLED.clear_screen();  
}
