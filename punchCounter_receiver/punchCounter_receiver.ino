#include "master_receiver.h"


void setup() {
  Serial.begin(9600);  
  t1.every(TimerSmallestUnit,timerEvent);    
  PCR.initial_punchCounterReceiver(volume_knob_pin, Segcom_Low, Segcom_High         
                                         , data_Pin, latch_Pin, clock_Pin ) ;
  PCR.initial_punchCounterMp3();                                     
  thread_initial();
  interrupt_initial();
 
#if AT_Mode
  Serial.println("enter AT commands");
    //punch_BT_L.punchBT_master_initial_set(serial2, AT_mode,left);
  punch_BT_R.punchBT_master_initial_set(serial3, AT_mode,right);
#else if   Master_Mode
 Serial.println("Master_Mode");
 punch_BT_L.punchBT_master_initial_set(serial2, Master_mode,left);
 punch_BT_R.punchBT_master_initial_set(serial3, Master_mode,right);
#endif
}

void timerEvent() {   
  red_condition(); 
  green_condition();
 digits_sw_condition(); 
}

void red_condition() {
    //1,release,0 pressed red button 3 seconds
   if(digitalRead(red_button_pin)){
      red_timer_counter = 0;
      check_red_button_reset = 0;
   } else {
        //0:not setting goal mode
        //1:setting goal mode
        red_timer_counter++;   
        if(!PCR.get_goal_mode()) {                 
                if(red_timer_counter >= reset_check_arrive) { 
                //reset check
                  PCR.red_button_reset_check();
                  red_timer_counter = 0;
                  check_red_button_reset = 1;        
              }
        } else {           
              if(red_timer_counter >= reset_check_arrive) {                  
                     //setting goal ok
                     red_timer_counter = 0;
                     check_green_button_set = 1;    
                     PCR.user_setting_goal_ok();
               }
     }  
   }
}

void green_condition() {
      //1:release
      //0:pressed green button 3 seconds
     if(digitalRead(green_button_pin)){
      green_timer_counter = 0;
      check_green_button_set = 0;
        if(!PCR.get_goal_mode())
            PCR.set_green_status_play_next();
          else 
            //still in goal mode
           PCR.set_green_status_goal();
     } else {
        green_timer_counter++;    
        if(PCR.user_get_goal_ST() == setting_goal){
                  if(green_timer_counter >= setGoal_arrive)  {
                   //setting goal
                    green_timer_counter = 0;
                    check_green_button_set = 1;        
                    PCR.user_setting_goal_now();  
                }
        } else {
                if(green_timer_counter >= setGoal_arrive)  {
                    //setting goal cancel
                     green_timer_counter = 0;
                     check_green_button_set = 1;    
                      PCR.user_setting_goal_cancel();
                }
        }
     }  
}

void digits_sw_condition() {
 
   if(PCR.get_digits_sw()){
          //digits dark or light    
       sw_timer_counter++;
       if (sw_timer_counter >= set_sw_arrive) {
          PCR.digits_drak_light_change();       
           sw_timer_counter = 0;
       }
     }
}

void loop() {
 #if AT_Mode  
    //punch_BT_L.AT_mode_function();
    punch_BT_R.AT_mode_function();
 #else if  Master_Mode
     t1.update();
     controll.run();
 #endif
}

void thread_initial() {
  
  // Configure Thread_BT_transmit
  Thread_BT_transmit->onRun(BT_transmit);
  Thread_BT_transmit->setInterval(150);
  
  // Configure Thread_BT_receive
  Thread_BT_receive->onRun(BT_receive);
  Thread_BT_receive->setInterval(150);

 // Configure Thread_SEG7
  Thread_SEG7->onRun(SEG7_display);
  Thread_SEG7->setInterval(10);
  
   // Configure Thread_mp3
  Thread_mp3->onRun(mp3_player);
  Thread_mp3->setInterval(1000);
 
  // Adds all threads to the controller    
    controll.add(Thread_BT_transmit);
    controll.add(Thread_BT_receive);      
    controll.add(Thread_SEG7);   
    controll.add(Thread_mp3);
}

void BT_transmit() {
 /*
 #if AT_Mode  
    punch_BT_R.AT_mode_function();
    //punch_BT_L.AT_mode_function();
 #else if receive_Mode
  int reL = punch_BT_L.Master_mode_receive();
  if(reL >0 && reL <10000){
     Serial.print("receive val_left: ");
     Serial.println(reL); 
     if(reL == 5) {
         if(!already_resetL) {   
            Serial.println("master is reset send reset signal to slaveL");
            punch_BT_L.Master_mode_transmit_reset();
         }
         already_resetL = true;
     } else if(reL == 1) {
         already_resetL = false;
     }
  } else if (reL == 10000){
      Serial.println("slaveL is pause!!");
  }
  
 int reR = punch_BT_R.Master_mode_receive();
 if(reR >0 && reR <10000){
     Serial.print("receive val_right: ");
     Serial.println(reR); 
     if(reR == 5) {
       if(!already_resetR) {   
            Serial.println("master is reset send reset signal to slaveR");
            punch_BT_R.Master_mode_transmit_reset();
       }
        already_resetR = true;
     } else if(reR == 1) {
        already_resetR = false;
     }
  } else if (reR == 10000){
      Serial.println("slaveR is pause!!");
  }
 #endif
 */
  
}

void BT_receive() {
  int reL = punch_BT_L.Master_mode_receive();
  int reR = punch_BT_R.Master_mode_receive();  
  int total_half = PCR.user_get_punch_total_goal()/2;
   
   if(reL >= 0 && reL <  total_half ){
    Serial.println("reL");
    PCR.user_set_start_pause_done_status_L(start_mode);  
     PCR.set_left_arm_number_inc(reL);         
     PCR.set_left_arm_number_countdown(reL);    
   } else if(reL == 10000){
    //pause
    Serial.println("pause L");
      PCR.user_set_start_pause_done_status_L(pause_mode);      
   } else if(reL == total_half) {
    //done
     PCR.user_set_start_pause_done_status_L(done_mode);  
   }

   
    if(reR >= 0 && reR<  total_half ){
      PCR.user_set_start_pause_done_status_R(start_mode);  
      PCR.set_right_arm_number_inc(reR);          
      PCR.set_right_arm_number_countdown(reR);    
   } else if(reR == 10000){
    //pause
      PCR.user_set_start_pause_done_status_R(pause_mode);      
   } else if(reR == total_half) {
    //done
     PCR.user_set_start_pause_done_status_R(done_mode);  
   }
   
   PCR.save_all_data_to_EEPROM();
}

void SEG7_display() {
    if(PCR.get_green_button_ST() != green_set_goal_mode) {
        if(PCR.get_red_button_ST() == red_goal_mode ) {
              PCR.show_punch_total_goal_on7SEG(PCR.user_get_punch_total_goal());      
        } else if (PCR.get_red_button_ST() == red_inc_mode) {
            deal_with_display_punch_pause_done();
        }  else if (PCR.get_red_button_ST() == red_countdown_mode) {
            deal_with_display_punch_pause_done_count_down();                 
        } else if (PCR.get_red_button_ST() == red_reset_check_mode) {
         PCR.show_reset_check_on7SEG();
          PCR.set_green_cancel_reset();   
        } else if(PCR.get_red_button_ST() == red_reset_mode) {
            PCR.red_button_reset();
            PCR.show_punch_total_goal_on7SEG(PCR.user_get_punch_total_goal());      
        }
    } /*else {
       PCR.set_digits_sw(0);  //turn off sw of digits drak light
    }*/

    if(PCR.get_red_button_ST() != red_reset_check_mode) {
        if(PCR.get_green_button_ST() == green_set_goal_mode) {
            PCR. set_digits();
        }      
    }
}

void deal_with_display_punch_pause_done() {
    if(PCR.user_get_start_pause_done_status_L() == start_mode && PCR.user_get_start_pause_done_status_R() == start_mode ) {
          PCR.show_punch_data_on7SEG(seg_num, seg_num);
     } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == pause_mode ) {
          PCR.show_punch_data_on7SEG(seg_pause, seg_pause);
      } else if(PCR.user_get_start_pause_done_status_L() == start_mode && PCR.user_get_start_pause_done_status_R() == pause_mode )  {
          PCR.show_punch_data_on7SEG(seg_num, seg_pause);
      } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == start_mode )  {
          PCR.show_punch_data_on7SEG(seg_pause, seg_num);
      } else if(PCR.user_get_start_pause_done_status_L() == done_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_on7SEG(seg_done, seg_done);
      } else if(PCR.user_get_start_pause_done_status_L() == done_mode && PCR.user_get_start_pause_done_status_R() == start_mode )  {
          PCR.show_punch_data_on7SEG(seg_done, seg_num);
      } else if(PCR.user_get_start_pause_done_status_L() == start_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_on7SEG(seg_num,seg_done );
      } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_on7SEG(seg_pause, seg_done);
      } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_on7SEG(seg_pause, seg_done);
      } else if(PCR.user_get_start_pause_done_status_L() == done_mode && PCR.user_get_start_pause_done_status_R() ==  pause_mode)  {
          PCR.show_punch_data_on7SEG(seg_done, seg_pause);
      }
}

void deal_with_display_punch_pause_done_count_down() {
    if(PCR.user_get_start_pause_done_status_L() == start_mode && PCR.user_get_start_pause_done_status_R() == start_mode ) {
          PCR.show_punch_data_count_down_on7SEG(seg_num, seg_num);
     } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == pause_mode ) {
          PCR.show_punch_data_count_down_on7SEG(seg_pause, seg_pause);
      } else if(PCR.user_get_start_pause_done_status_L() == start_mode && PCR.user_get_start_pause_done_status_R() == pause_mode )  {
          PCR.show_punch_data_count_down_on7SEG(seg_num, seg_pause);
      } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == start_mode )  {
          PCR.show_punch_data_count_down_on7SEG(seg_pause, seg_num);
      } else if(PCR.user_get_start_pause_done_status_L() == done_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_count_down_on7SEG(seg_done, seg_done);
      } else if(PCR.user_get_start_pause_done_status_L() == done_mode && PCR.user_get_start_pause_done_status_R() == start_mode )  {
          PCR.show_punch_data_count_down_on7SEG(seg_done, seg_num);
      } else if(PCR.user_get_start_pause_done_status_L() == start_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_count_down_on7SEG(seg_num,seg_done);
      } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_count_down_on7SEG(seg_pause, seg_done);
      } else if(PCR.user_get_start_pause_done_status_L() == pause_mode && PCR.user_get_start_pause_done_status_R() == done_mode )  {
          PCR.show_punch_data_count_down_on7SEG(seg_pause, seg_done);
      } else if(PCR.user_get_start_pause_done_status_L() == done_mode && PCR.user_get_start_pause_done_status_R() ==  pause_mode)  {
          PCR.show_punch_data_count_down_on7SEG(seg_done, seg_pause);
      }
}

void mp3_player() { 
  //if =0 press , else no press
  #if mp3_test_mode
  static unsigned long timer = millis();  
  #endif 
  
  if(!mp3_button_status) {     
      //Serial.println(F("mp3 button press"));    
        PCR.set_mp3_volume(read_volume_knob_value());
  #if mp3_test_mode    
  //test every 5 secons  will play mp3 once
         if(millis()- timer >5000) {
                  timer = millis();
                  if(!PCR.get_mp3_status()){
                     PCR.mp3_start();
                  }
                  PCR.mp3_play_next();
         }
 #else if 
           if (now_play_next) {
                  PCR.mp3_play_next();
                 now_play_next = 0;    
             }
  #endif        
  } else {
      //Serial.println(F("mp3 button no press"));          
         if(PCR.get_mp3_status()){
                     PCR.mp3_pause();
          }
  }
}

void interrupt_initial() {
          digitalWrite(red_button_pin, HIGH); //turn on pullup resistors
          digitalWrite(green_button_pin, HIGH);  //turn on pullup resistors
          digitalWrite(mp3_button_pin, HIGH);
           //pinMode(red_button_pin,INPUT);        
          // pinMode(green_button_pin,INPUT);    
          pinMode(mp3_button_pin,INPUT);       
           get_mp3_button_value();   
          delay(100);    
          attachInterrupt(digitalPinToInterrupt(red_button_pin), switch_goal_nowData_ISR,FALLING);
          attachInterrupt(digitalPinToInterrupt(green_button_pin), switch_playTip_setGoal_ISR, FALLING);          
         attachInterrupt(digitalPinToInterrupt(mp3_button_pin), switch_mp3_on_off_ISR, CHANGE);
}

void switch_goal_nowData_ISR() {
  //red button
         if(!start_power_on1) {
              static unsigned long last_interrupt_time = 0;
              unsigned long interrupt_time = millis();     
              if (interrupt_time - last_interrupt_time > 200) {      
                  if(PCR.get_green_button_ST() != green_set_goal_mode) {
                        //1 not reset,0 already reset check
                         if(PCR.get_red_button_ST() != red_reset_check_mode && PCR.get_red_button_ST() != red_reset_mode ){
                                PCR.change_red_status();                                                   
                         } else if(PCR.get_red_button_ST() == red_reset_check_mode && check_red_button_reset == 0) {
                              //check_red_button_reset use to prevent to press red button already 3 seconds and let the  button release will go into here                                  
                               PCR.set_red_status_reset();                         
                         }  
                  }  else {
                      PCR.change_set_goal_digits();
                  }
              }
              last_interrupt_time = interrupt_time;
         }
        start_power_on1 = false;
}

void switch_playTip_setGoal_ISR() {
  //green button
         if(!start_power_on2) {
              static unsigned long last_interrupt_time = 0;
              unsigned long interrupt_time = millis();     
              if (interrupt_time - last_interrupt_time > 200) { 
                if(PCR.get_red_button_ST() != red_reset_check_mode ) {                   
                      if(PCR.get_green_button_ST() == green_set_goal_mode /*&& check_green_button_set == 0*/ && PCR.get_goal_mode() == 1 ){
                              //check_green_button_reset use to prevent to press green button already 3 seconds and let the  button release will go into here   
                              PCR.inc_dec_digits_num();
                              //Serial.println(F("inc_dec_digits_num()"));    
                      } else  if(PCR.get_green_button_ST() == green_play_next_mode){
                            now_play_next = 1;            
                             //Serial.println(F("now_play_next"));             
                      }
                } else {
                    if(PCR.get_green_button_ST() == green_cancel_reset_mode && check_red_button_reset == 0) {    
                              PCR.set_red_status_goal();  //cancel reset
                      } 
                }
              }
              last_interrupt_time = interrupt_time;
         }
        start_power_on2 = false;
}



void switch_mp3_on_off_ISR() {
         if(!start_power_on3) {
              static unsigned long last_interrupt_time = 0;
              unsigned long interrupt_time = millis();     
              if (interrupt_time - last_interrupt_time > 200) {           
                 mp3_button_status = !mp3_button_status ;          
              }
              last_interrupt_time = interrupt_time;
         }
        start_power_on3 = false;
}

uint8_t read_volume_knob_value() {
  uint8_t level = 0;
  level = PCR.get_volume_level(); //test
  level = 25;   //test
  //Serial.println(F("level: "));
  //Serial.println(level);
  return level; 
}

void get_mp3_button_value() {  
  mp3_button_status = digitalRead(mp3_button_pin);
}

