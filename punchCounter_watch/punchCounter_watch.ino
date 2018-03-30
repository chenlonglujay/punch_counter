#include <punchCounterWatch.h>
#include <Thread.h>
#include <Timer.h>
#include <ThreadController.h>
#include <punchBT_slave.h>
punchBT_slave punch_L;
punchCounterWatch PH_watch;
Timer tp;

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
#define reset_page_btn_pin 3      //reset need push down 3 seconds

#define AT_Mode 0       //first,you need use AT_Mode to pair BT,please refer to BT_pair document
#define slave_Mode 1   //second, use slave_Mode to recrive data from master or transmit data to master

#define sensitivity_pin A0
#define start_pause_LED_pin 4       //start led ON, pause led OFF 
#define battery_detect_pin  A1
#define reset_check_arrive 30             //timer, TimerSmallestUnit  * reset_check_arrive
#define second_check_arrive 10        //timer, TimerSmallestUnit  * second_check_arrive
#define battery_check_arrive 300         //timer,  TimerSmallestUnit * battery_check_arrive

int timer_count = 0;
bool switchCheck_sp = true;
bool reset_page_is_start = false;
bool resetAllDone = false;
int timer_reset_page_count = 0;
bool start_power_on = true;
int punchCountBF = 0;
int punchCountNow = 0;
bool update_once = false;
int battery_count = 0;
int battery_percent = 0;

void setup() {
  Serial.begin(9600);  
  PH_watch.punchCounterWatch_initial_set(sensitivity_pin, battery_detect_pin);
  pinMode(start_pause_LED_pin, OUTPUT);
  tp.every(TimerSmallestUnit,timerEvent);  
  punchCounter_initial();
  interrupt_initial();
  thread_initial();
}

void loop() {   
  controll.run();
  tp.update(); 
}

void timerEvent() {          
          if(PH_watch.get_start_pause_status()) {
            timer_count++;        
                if(timer_count == second_check_arrive) {
                      //TimerSmallestUnit *second_check_arrive = 1 second
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
         // showTimeData();      
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
                        Serial.println("changing page");                                
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
           reset_data(&punch_L);                        //transmit count=0
           transmit_data(&punch_L, 0, false);            
           PH_watch.set_pause();           //work likes pause mode
           transmit_puse(&punch_L);    
           update_once = true;
           switchCheck_sp = watch_start;       //next push start/pause button down will go into start  
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
                    digitalWrite(start_pause_LED_pin, LOW);     //LED ON
                    PH_watch.set_start();
                } else if(switchCheck_sp == watch_pause)  {
                   //Serial.println("pause");
                   digitalWrite(start_pause_LED_pin, HIGH);       //LED OFF
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

void punchCounter_initial() {
#if AT_Mode
        punch_L.punchBT_slave_initial_set(AT_mode, left);         
#else slave_Mode  
        punch_L.punchBT_slave_initial_set(Slave_mode, left);       //if this module need set right arm module,please type argument 2 is right
        battery_percent = PH_watch.get_battery_percent();
        digitalWrite(start_pause_LED_pin, HIGH);       //LED OFF
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
  get_count_transmitData(&puncqh_L);
}

void BT_receive() {
  bool receive_status;
  receive_status = punch_L.Slave_mode_receive_reset();
  if(receive_status == reset_yes){
    reset_data_count();
  }
}

void get_count_transmitData(punchBT_slave *input) {
  bool show_data = false;
 int sensitivity;
  if(PH_watch.get_start_pause_status()){       
        punchCountNow  = PH_watch.getHumanPunchCount();
         //Serial.print("punchCount: ");
         //Serial.println(punchCountNow);
          sensitivity = PH_watch.get_sensitivity_percent();
         //Serial.print("sensitivity: ");
         //Serial.println(sensitivity);
        //if(punchCountNow > punchCountBF) {
               if(input->get_transmitData() == 9999) { 
                       reset_data(input);
                }
                transmit_data(input, punchCountNow, show_data);
        //}
         punchCountBF = punchCountNow;
    } else {      
      //Serial.println("puase");
    }
}

void OLED_display() {
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

void transmit_puse(punchBT_slave *input) {
     input->set_punch_pause(pause);
     input->Slave_mode_transmit(false);          
     digitalWrite(start_pause_LED_pin, HIGH);       //LED OFF
}


