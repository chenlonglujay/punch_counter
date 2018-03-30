#include <Wire.h>
#include <punchOLED.h>
punchOLED punch_OLED;
#include "OLED_database_watch.c"
void setup(){
  punch_OLED.punchOLED_initial();
  delay(10);
}

void loop(){
  punch_count_OLED *pco = new punch_count_OLED;
  time_save_OLED *tso = new time_save_OLED;
  punch_OLED.clear_screen();
  punch_OLED.showPicture128x64_lab();
  delay(2000);  

   punch_OLED.clear_screen();
  punch_OLED.showPicture128x64_fist();
  delay(2000);  
  pco->THD = 0;
  pco->HUD = 9;
  pco->TEN = 8;
  pco->ONE = 7;

  tso->day_h = 0;
  tso->day_l = 1;
  tso->hour_h = 2;
  tso->hour_l = 3;
  tso->minute_h = 4;
  tso->minute_l = 5;
  tso->second_h = 0;
  tso->second_l = 6;
  
  //show page1
  punch_OLED.clear_screen();
   punch_OLED.show_watch_page1(pco, tso);
   delay(2000);
   
   //show page2
   punch_OLED.clear_screen();
   pco->HUD = 0;
   pco->TEN = 6;
   pco->ONE = 5;
   punch_OLED.show_watch_page2(1,0,1,pco);      //bool not0/charge1,bool left0/right1 watch, bool pause0/start1 mode, punch count sensitivity(0~100%)
   delay(2000); 

 //show page3 reset
  punch_OLED.clear_screen();
  punch_OLED.show_watch_reset();
   delay(2000); 
   
  delete pco;
  delete tso;
}
