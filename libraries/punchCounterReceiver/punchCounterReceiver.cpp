//For Arduino 1.0 and earlier 
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else     
#include "WProgram.h"
#endif 

#include <punchCounterReceiver.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
Segment7_2p3inch_PH s7;
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
punchCounterReceiver::punchCounterReceiver(void) {
    //default
    seg_switch_goal_now_punch = 0;
    volume_knob = A0;
    for (int i=49;i>41;i--) {
        ctrl_com_pin[abs(i - 49)] = i;
    }
    dataPin =5;
    latchPin = 6;
    clockPin = 7;
    s7.setPin_7seg(dataPin, latchPin, clockPin);
    punch_total_goal = default_R_L_total;   
    set_goal_L = default_L_MAX;
    set_goal_R = default_R_MAX;
    set_red_status_goal();
    red_ST_counter = 0;
    left_arm_number = 0;
    right_arm_number = 0;
    change_digits_counter = 0;
    set_green_status_play_next();
    digits_drak_light_ST = DTS_light;
    digits_dl = 0;
    digits_sw = 0;
    digits_number = 0;
    goal_mode = false;
    SGST = setting_goal;
    set_goal_ST_switch = false;
    SPD_L = start_mode;
    SPD_R = start_mode;
    transmit_reset_flag = false;
    transmit_goal_flag = false;
    play_mp3_cancel_voice = false;
    play_mp3_ridicule_voice = false;
    start_check_stop_5secs = false;
    punchL_BF = 0;
    punchR_BF = 0;
    ridicule_voice_counter = 0;
    checkL_5secs = false;
    checkR_5secs = false;
    play_once = false;
}

punchCounterReceiver::~punchCounterReceiver(void) {

}

void punchCounterReceiver::initial_punchCounterReceiver(uint8_t volume_Knob, uint8_t segcom_Low, uint8_t segcom_High
                                                   , uint8_t data_pin, uint8_t latch_pin, uint8_t clock_pin) {
    volume_knob = volume_Knob;
    int i = 0;
    for (i=segcom_High;i>segcom_Low;i--) {
        ctrl_com_pin[abs(i - segcom_High)] = i;
    }

    dataPin = data_pin;
    latchPin = latch_pin;
    clockPin = clock_pin;
    s7.setPin_7seg(dataPin, latchPin, clockPin);
    s7.setPin_com(ctrl_com_pin);
    read_all_data_from_EEPROM();
    set_R_L_goal();
    show_punch_total_goal_on7SEG(0); //initial show goal
    pinMode(volume_Knob,INPUT);
    goal_mode = false;
    SGST = setting_goal;
    set_goal_ST_switch = false;
    SPD_L = start_mode;
    SPD_R = start_mode;
    transmit_reset_flag = false;
    transmit_goal_flag = false;
    play_mp3_cancel_voice = false;
    play_mp3_ridicule_voice = false;
    punchL_BF = 0;
    punchR_BF = 0;
    start_check_stop_5secs = false;
    ridicule_voice_counter = 0;
    checkL_5secs = false;
    checkR_5secs = false;
    play_once = false;
}


void punchCounterReceiver::initial_punchCounterMp3() {
    mySoftwareSerial.begin(9600);
    Serial.println();
    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(5);  //Set volume value (0~30).
  myDFPlayer.volumeUp(); //Volume Up
  myDFPlayer.volumeDown(); //Volume Down

//----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
    mp3_pause();
}


void punchCounterReceiver::set_R_L_goal() {
    if (punch_total_goal <= 0) {
        punch_total_goal = default_R_L_total;
        set_goal_L = default_L_MAX; 
        set_goal_R = default_R_MAX; 
    } else {
        int goal = punch_total_goal/2;
        set_goal_L = goal;
        set_goal_R = goal; 
    }
}

void punchCounterReceiver::show_punch_data_on7SEG(seg_show word_L, seg_show word_R) {
    s7.divide_and_output(left_arm_number, right_arm_number, word_L, word_R);
}

void punchCounterReceiver::show_punch_data_count_down_on7SEG(seg_show word_L, seg_show word_R) {
    set_R_L_goal();
    s7.divide_and_output(left_arm_number_count_down, right_arm_number_count_down, word_L, word_R);
}

void punchCounterReceiver::show_punch_total_goal_on7SEG(int goal_value) {
    s7.divide_and_output(0, goal_value , seg_goal, seg_goal);
    /*
    GOAL(L)
    xxxx(R) total number
    */
}

uint8_t punchCounterReceiver::get_volume_level() {
    //level 0 - 30(max)
    float volume = analogRead(volume_knob);
    float cul = volume / 1023 * 30;
    //Serial.println(F("volume:"));
    //Serial.println(volume);
    uint8_t level = cul;
    //Serial.println(F("volume level:"));
    //Serial.println(cul);
    return level;

}



void punchCounterReceiver::show_mp3player_message(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}


void punchCounterReceiver::set_mp3_volume(uint8_t value){
    if(value > 30)
        value = 30;
    myDFPlayer.volume(value);  //Set volume value (0~30).
}

void punchCounterReceiver::mp3_play_selected(int number) {
    myDFPlayer.play(number); 
}

void punchCounterReceiver::mp3_play_previous() {
    myDFPlayer.previous(); 
}

void punchCounterReceiver::mp3_play_next() {
    myDFPlayer.next(); 
}

void punchCounterReceiver::mp3_start() {
    myDFPlayer.start();  //start the mp3 from the pause
    mp3_status = mp3_ST_start;
}

void punchCounterReceiver::mp3_pause() {
    myDFPlayer.pause();  //pause the mp3
    mp3_status = mp3_ST_pause;
}

mp3_ST punchCounterReceiver::get_mp3_status() {
    return  mp3_status;
}

red_ST punchCounterReceiver::get_red_button_ST() {
    return red_button_ST;
}

void punchCounterReceiver::change_red_status() {
    
     if (red_ST_counter > 2) {
        red_ST_counter = 0;
        red_button_ST = red_goal_mode;
    } 

     if (red_ST_counter == 0) { 
        red_button_ST = red_goal_mode;
     } else if (red_ST_counter == 1) {
        red_button_ST = red_inc_mode;
     } else if (red_ST_counter == 2) {
        red_button_ST = red_countdown_mode;
     } 
     red_ST_counter++;
}

void punchCounterReceiver::red_button_reset() {
    red_button_ST = red_goal_mode;
    red_ST_counter = 0; 
    left_arm_number = 0;
    right_arm_number = 0;
    left_arm_number_count_down = 0;
    right_arm_number_count_down = 0;
    punch_total_goal = default_R_L_total;
    user_set_start_pause_done_status_L(pause_mode);
    user_set_start_pause_done_status_R(pause_mode);
    set_goal_L = 0;
    set_goal_R = 0;
    mp3_start();
    mp3_play_selected(mp3_reset);
}
 
void punchCounterReceiver::red_button_reset_check() {
    red_button_ST = red_reset_check_mode;
}
 
void punchCounterReceiver::show_reset_check_on7SEG() {
    s7.divide_and_output(0, 0,  seg_reset, seg_reset);   
}

void punchCounterReceiver::set_left_arm_number_inc(int value) {
    left_arm_number = value;
}

void punchCounterReceiver::set_right_arm_number_inc(int value) {
    right_arm_number = value;
}               


void punchCounterReceiver::set_left_arm_number_countdown(int value) {
    int goal = set_goal_L;
    left_arm_number_count_down = abs(goal - value);
}

void punchCounterReceiver::set_right_arm_number_countdown(int value) {
    int goal = set_goal_R;
    right_arm_number_count_down = abs(goal - value);
}        
                 

void punchCounterReceiver::set_red_status_reset() {
    user_set_transmit_reset_flag();
    //Serial.println("set_red_status_reset"); 
    red_button_ST = red_reset_mode;
}
                                                 
void punchCounterReceiver::set_red_status_goal() {
    red_button_ST = red_goal_mode;
    red_ST_counter = 0;
}

void punchCounterReceiver::set_green_status_goal() {
     green_button_ST = green_set_goal_mode;
} 

void punchCounterReceiver::set_green_status_play_next() {
     green_button_ST = green_play_next_mode;
}     

green_ST punchCounterReceiver::get_green_button_ST() {  
    return green_button_ST;   
}

void punchCounterReceiver::change_set_goal_digits() {
    //change_digits_counter++;

    if(change_digits_counter > 4) {
        change_digits_counter = 0;
    }    
    if(change_digits_counter == 0) {
        digits_button_ST = digits_units_mode;
    } else if ( change_digits_counter == 1) {
        digits_button_ST = digits_tens_mode;
    } else if ( change_digits_counter == 2) {
        digits_button_ST = digits_hundreds_mode;
    } else if (change_digits_counter ==3) {
        digits_button_ST = digits_thousands_mode;
    }

    change_digits_counter++;
}

void punchCounterReceiver::set_green_cancel_reset(){
    green_button_ST = green_cancel_reset_mode;
}

digits_ST punchCounterReceiver::get_digits_ST() {
    return digits_button_ST;
}

void punchCounterReceiver::digits_drak_light_change() {
    digits_dl = !digits_dl;
    if(digits_dl) {
        digits_drak_light_ST = DTS_dark;
    } else {
        digits_drak_light_ST = DTS_light;
    }
}

DTS_DL_ST punchCounterReceiver::get_digits_dark_light_status() {
    return digits_drak_light_ST; 
}

void punchCounterReceiver::set_digits_sw(bool sw) {
    if(sw) {
        digits_sw = 1;
    } else {
        digits_sw = 0;
    }
}

bool punchCounterReceiver::get_digits_sw() {
    bool sw = digits_sw;
    return sw;
}

void punchCounterReceiver::set_digits(){
        uint8_t dl = 0;
     if(get_digits_ST() == digits_units_mode) {
        if(get_digits_dark_light_status() == DTS_light) {
            dl = seg_goal;
        } else {
            dl = seg_goal_xxxd;
        }
        //Serial.println(punch_total_goal);
        //Serial.println(dl);
        s7.divide_and_output(0, punch_total_goal, seg_goal, dl);
    } else if(get_digits_ST() == digits_tens_mode) {
        if(get_digits_dark_light_status() == DTS_light) {
            dl = seg_goal;
        } else {
            dl = seg_goal_xxdx;
        }
        s7.divide_and_output(0, punch_total_goal, seg_goal, dl);
    } else if(get_digits_ST() == digits_hundreds_mode) {
        if(get_digits_dark_light_status() == DTS_light) {
            dl = seg_goal;
        } else {
            dl = seg_goal_xdxx;
        }
        s7.divide_and_output(0, punch_total_goal, seg_goal, dl);   
    } else if(get_digits_ST() == digits_thousands_mode) {
        if(get_digits_dark_light_status() == DTS_light) {
            dl = seg_goal;
        } else {
            dl = seg_goal_dxxx;
        }
        s7.divide_and_output(0, punch_total_goal, seg_goal, dl);   
    }
        //Serial.println(punch_total_goal);
}

void punchCounterReceiver::inc_dec_digits_num() {
    int cul = 0, cul1 = 0;
    if(get_digits_ST() == digits_units_mode) {
       cul = punch_total_goal % 10;
       cul1 = cul + 1;  
        if(cul1 == 10) {
            cul1 = 0;
        } 
       punch_total_goal = punch_total_goal - cul + cul1;
    } else if(get_digits_ST() == digits_tens_mode) {
        cul = punch_total_goal % 100;
        cul = cul / 10;
        cul1 = cul + 1;
        if(cul1 == 10) {
            cul1 = 0;
        }
       punch_total_goal = punch_total_goal - cul*10 + cul1*10; 
    } else if(get_digits_ST() == digits_hundreds_mode) {
        cul = punch_total_goal % 1000;
        cul = cul / 100;
        cul1 = cul + 1;
        if(cul1 == 10) {
            cul1 = 0;
        }
       punch_total_goal = punch_total_goal - cul*100 + cul1*100; 
    } else if(get_digits_ST() == digits_thousands_mode) {
        cul = punch_total_goal / 1000;
        cul1 = cul + 1;
        if(cul1 == 10) {
            cul1 = 0;
        }
       punch_total_goal = punch_total_goal - cul*1000 + cul1*1000; 
    }


}


void punchCounterReceiver::set_goal_mode(bool true_false) {
    goal_mode = true_false;
}

bool punchCounterReceiver::get_goal_mode() {
    return goal_mode;
}

void punchCounterReceiver::user_set_goal_ST() {
    if (!set_goal_ST_switch) {
        set_goal_ST_switch = !set_goal_ST_switch;
        SGST = setting_goal_cancel;
        save_punch_total_goal_set_before(); 
    } else {
        set_goal_ST_switch = !set_goal_ST_switch;
        SGST = setting_goal; 
    }
}

set_goal_ST punchCounterReceiver::user_get_goal_ST() {
    if (SGST == setting_goal)
        return setting_goal;
    else if (SGST == setting_goal_cancel)
        return setting_goal_cancel;
}

int punchCounterReceiver::user_get_punch_total_goal() {
    return punch_total_goal;
}

void punchCounterReceiver::save_punch_total_goal_set_before() {
    punch_total_goal_setBF = punch_total_goal;  
}

void punchCounterReceiver::cancel_setting_punch_total_goal() {
    punch_total_goal = punch_total_goal_setBF;
}


void punchCounterReceiver::save_all_data_to_EEPROM() {
    uint8_t tempH, tempL;
    int temp;
    temp = punch_total_goal;
    tempL = temp & 0x00ff;
    EEPROM.write(punch_goalL, tempL);
    tempH = temp >> 8;
    EEPROM.write(punch_goalH, tempH);
    //Serial.print(F("write punch total goal:"));
    //Serial.println(temp);
    
    temp = left_arm_number;
    tempL = temp & 0x00ff;
    EEPROM.write(punchL_nowL, tempL);
    tempH = temp >> 8;
    EEPROM.write(punchL_nowH, tempH);
    //Serial.print(F("write left arm number: "));
    //Serial.println(temp);
    //Serial.print(F("write left tempL: "));
    //Serial.println(tempL);
    //Serial.print("write left tempH: ");
    //Serial.println(tempH);
    
    temp = right_arm_number;
    tempL = temp & 0x00ff;
    EEPROM.write(punchR_nowL, tempL);
    tempH = temp >> 8;
    EEPROM.write(punchR_nowH, tempH);
    //Serial.print(F("write right arm number: "));
    //Serial.println(temp);
}

void punchCounterReceiver::read_all_data_from_EEPROM() {
    uint8_t tempL,tempH;
    int count_down;
    tempL = EEPROM.read(punch_goalL);
    tempH = EEPROM.read(punch_goalH);
    punch_total_goal = (tempH << 8) + tempL;
    count_down = punch_total_goal / 2;
    //Serial.print(F("read punch total goal: "));
    //Serial.println(punch_total_goal);
    
    tempL = EEPROM.read(punchL_nowL);
    tempH = EEPROM.read(punchL_nowH);
    left_arm_number = (tempH << 8) + tempL;
    left_arm_number_count_down = count_down - left_arm_number;
    Serial.print(F("read left_arm_number: "));
    Serial.println(left_arm_number);
    
    tempL = EEPROM.read(punchR_nowL);
    tempH = EEPROM.read(punchR_nowH);
    right_arm_number = (tempH << 8) + tempL;
    right_arm_number_count_down = count_down - right_arm_number;
    //Serial.print(F("read right_arm_number: "));
    //Serial.println(right_arm_number);
}


void punchCounterReceiver::user_set_start_pause_done_status_L(start_pause_done_ST val) {
    SPD_L = val;
}  

void punchCounterReceiver::user_set_start_pause_done_status_R(start_pause_done_ST val) {
    SPD_R = val;
}

start_pause_done_ST punchCounterReceiver::user_get_start_pause_done_status_L() {
    return SPD_L;
}

start_pause_done_ST punchCounterReceiver::user_get_start_pause_done_status_R() {
    return SPD_R;
}

void punchCounterReceiver::user_setting_goal_ok() {
    mp3_play_selected(mp3_setok);
    user_set_transmit_goal_flag();
    set_green_status_play_next();
    set_goal_mode(0);     
    set_digits_sw(0);  //turn off sw of digits drak or light     
    user_set_goal_ST();
    red_button_ST = red_goal_mode;
    red_ST_counter = 0; 
    left_arm_number = 0;
    right_arm_number = 0;
    left_arm_number_count_down = 0;
    right_arm_number_count_down = 0;
    user_set_start_pause_done_status_L(pause_mode);
    user_set_start_pause_done_status_R(pause_mode);
    set_R_L_goal();
}

void punchCounterReceiver::user_setting_goal_now() {
    set_green_status_goal();   
    set_goal_mode(1);        
    set_digits_sw(1);  //turn on sw of digits drak or light     
    user_set_goal_ST();
}

void punchCounterReceiver::user_setting_goal_cancel() {
    mp3_play_selected(mp3_set_cancel);
    set_green_status_play_next();
    set_goal_mode(0);     
    set_digits_sw(0);  //turn off sw of digits drak or light     
    user_set_goal_ST();
    cancel_setting_punch_total_goal();
}

void punchCounterReceiver::user_set_transmit_reset_flag() {
    transmit_reset_flag = !transmit_reset_flag;
}

bool punchCounterReceiver::user_get_transmit_reset_flag() {
    return transmit_reset_flag;
}

void punchCounterReceiver::user_set_transmit_goal_flag() {
    //Serial.println("user_set_transmit_goal_flag");
    transmit_goal_flag = !transmit_goal_flag;
}

bool punchCounterReceiver::user_get_transmit_goal_flag() {
    return transmit_goal_flag;
}

int punchCounterReceiver::user_get_goal_L() {
    return set_goal_L;
}

int punchCounterReceiver::user_get_goal_R() {
    return set_goal_R;
}


void punchCounterReceiver::play_cancel_voice() {
    if(play_mp3_cancel_voice) {
       mp3_play_selected(mp3_set_cancel);
       set_play_cancel_voice(0);
    } 
}    

void punchCounterReceiver::set_play_cancel_voice(bool value) {
    play_mp3_cancel_voice = value;
}

void punchCounterReceiver::play_ridicule_voice() {

    if(get_play_once()){
        ridicule_voice_counter++;
        if(ridicule_voice_counter == 6) {
            ridicule_voice_counter = 0;
        }
        mp3_play_selected(ridicule_voice_counter);
    }
}

void punchCounterReceiver::set_play_ridicule_voice(bool value) {
    play_mp3_ridicule_voice = value;
}

bool punchCounterReceiver::get_play_ridicule_voice() {
    return play_mp3_ridicule_voice;
}

void punchCounterReceiver::set_5secs_check_flag(bool value) {
    start_check_stop_5secs = value;
}

bool punchCounterReceiver::get_5secs_check_flag() {
    return start_check_stop_5secs;
}

void punchCounterReceiver::check_start_stop_punch(int numL, int numR) {
    bool LC = get_check_5secs_L();
    bool RC = get_check_5secs_R();
    if(LC && RC) {
        //Serial.println(F("both check"));
        //both are in start mode
       if(numL==punchL_BF && numR==punchR_BF) {
           set_5secs_check_flag(1);  //timer start to calculate 
       } else {
           set_5secs_check_flag(0);  //timer stop to calculate
           set_play_ridicule_voice(0); 
       } 
    } else if (LC && !RC) {
        //LC start,RC pause
       //Serial.println(F("L check"));
       if(numL==punchL_BF) {
           set_5secs_check_flag(1);  //timer start to calculate 
        } else if (numL>punchL_BF) {
           set_5secs_check_flag(0);  //timer stop to calculate
           set_play_ridicule_voice(0); 
        }
    } else if (!RC && LC) {
        //RC start,LC pause
        //Serial.println(F("R check"));
       if(numR==punchR_BF) {
           set_5secs_check_flag(1);  //timer start to calculate 
        } else if (numR>punchR_BF) {
           set_5secs_check_flag(0);  //timer stop to calculate
           set_play_ridicule_voice(0); 
        }
    } else {
           //Serial.println(F("no check"));
           set_5secs_check_flag(0);  //timer stop to calculate
           set_play_ridicule_voice(0); 
    }
    punchL_BF = numL;
    punchR_BF = numR;
}

void punchCounterReceiver::set_check_5secs_L(bool value) {
    checkL_5secs = value;
}

void punchCounterReceiver::set_check_5secs_R(bool value) {
    checkR_5secs = value;
}

bool punchCounterReceiver::get_check_5secs_L() {
    return checkL_5secs;
}

bool punchCounterReceiver::get_check_5secs_R() {
    return checkR_5secs;
}

void punchCounterReceiver::set_play_once(bool value) {
    play_once = value;
}

bool punchCounterReceiver::get_play_once() {
    return play_once;
}
