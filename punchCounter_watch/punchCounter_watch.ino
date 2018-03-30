#include <punchCounterWatch.h>
#include <Timer.h>
#define TimerSmallestUnit 100
#define start_pause_btn_pin 2      
#define reset_page_btn_pin 3      //reset need push down 3 second

punchCounterWatch PH_watch;
Timer tp;
int timer_count = 0;
bool switchCheck_sp = true;
bool reset_page_is_start = false;
bool resetAllDone = false;
int timer_reset_page_count = 0;
bool start_power_on = true;
int punchCountBF = 0;
int punchCountNow = 0;
bool update_once = false;

void setup() {
  Serial.begin(9600);
  PH_watch.punchCounterWatch_initial_set(A0);
  tp.every(TimerSmallestUnit,timerEvent);  
  interrupt_initial();
}

void loop() {   
     tp.update();  
     if(PH_watch.get_start_pause_status()){  
      //Serial.println("loop start ");
        punchCountNow  = PH_watch.getHumanPunchCount();
        if(punchCountNow > punchCountBF) {
              Serial.print("punchCount: ");
              Serial.println(punchCountNow);
        }
         punchCountBF = punchCountNow;
    } else {
        //Serial.println("loop pause ");
    }
}

void timerEvent() {          
          if(PH_watch.get_start_pause_status()) {
            timer_count++;        
                if(timer_count == 10) {
                      //100ms *10 = 1 second
                      PH_watch.timer_add_1_second();
                      update_punch_timer();
                }              
          } else {    
              if(update_once) { 
                update_punch_timer();     //pause mode only save date to eeprom once
              }
              update_once = false;
          }
          arrange_reset_page();         
}

void update_punch_timer() {         
         PH_watch.savePunchCountToEEPROM();
         PH_watch.saveTimerDataToEEPROM();
          showTimeData();      
          timer_count = 0;  
}

void arrange_reset_page() {
          if(reset_page_is_start) {
                  timer_reset_page_count++;              
                    if(timer_reset_page_count >= 30) {                        
                          //100ms *30 = 3 seconds
                            PH_watch.resetAll();
                            Serial.println("resetAll");
                            timer_reset_page_count = 0;
                            resetAllDone = true;
                            PH_watch.set_pause();           //work like pause mode
                            update_once = true;
                            switchCheck_sp = watch_start;       //next push start/pause button down will go into start
                    } 
              } else if (!reset_page_is_start && timer_reset_page_count != 0) {
                    if(!resetAllDone) {
                      //after resetAll finished ,avoids into changing page                      
                        Serial.println("changing page");                   
                         timer_reset_page_count = 0;          
                    }
                    resetAllDone = false;
                    timer_reset_page_count = 0;
              }
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
                    Serial.println("start");
                    PH_watch.set_start();
                } else if(switchCheck_sp == watch_pause)  {
                   Serial.println("pause");
                   PH_watch.set_pause();
                   update_once = true;
                }
                switchCheck_sp = !switchCheck_sp;
            }
            last_interrupt_time = interrupt_time;
}

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



