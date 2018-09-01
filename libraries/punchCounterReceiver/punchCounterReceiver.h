//Master:Arduino Mega 2560

/*
function:
read and show punch count data on 7 segment from user(slave) punches
left and right count total are 9999
MAX:9998 MIN:0000
0188(L) MAX:4999
0087(R) MAX:4999

about button: 
press green button once:  
    play the fisrt mp3 once,if you press again which play second one(1->last loop)
press red button once:
    if count data goal are 500 that will show on 7segment as below:
    GOAL(L)
    0500(R)    
    press again will show R and L count data now
    0088(L)
    0087(R)

press green button 3 seconds:
    sets goal number
    caution!!can not set total are 0,if you do that,system will play tips voice(0006_reset.mp3)
    GOAL
    0000(total)
    now press green button once,7 segment will increase number(0-9 loop)
    press red button once,will change digit(thousands hundreds tens units),number picked will blink
    if you set ok,press green button 3 seconds,number will blink 3times until to stop that expresses set ok  
press red button 3 seconds:
    first 7 segment will show as below
    rESt(L)
    y n (R)
    red is yes
    green is no
    if you choose yes
    all data will reset includes now punch data and goal number!!!
    and play tips voice(0007_setok.mp3)
    if you choose no
    will show data as below
    GOAL(L)
    0010(R)

about 7 segment displays:
total numbers are 8,R has 4 and L has too.
2SK611 control byte gate
gate sets 1,seven-segment ON
gate sets 0,seven-segment OFF


about BT: 
bluetooth(HC05) module 
function
1.master receives punch count data from slave
  data format:
  left arm: master receives   punch count: "lxxxx." ex:"l1000."  expresses punch count is 1000
  right arm: master receives  punch count: "rxxxx." ex:"r0887." expresses punch count is 887
2.master receives reset from slave
  data format:
  left arm: master receives "l0000." from slave ,expresses slave is reset,master needs to clear punch data
  right arm: master receives "r0000."from slave ,expresses slave is reset,master needs to clear punch data
3.master transmits reset to slave
  data format:
  left arm: master transmits "lREST." to slave,expresses master is reset, slave needs to clear punch data
  right arm: master teansmits "rREST." to slave,expresses master is reset, slave needs to clear punch data
4.master receives pause from slave
  data format:
  left arm: master receives "lPUSE." from slave ,expresses slave is pause, master needs to stop checking human punch,    
                     otherwise count increases too slow,master needs to play music or voice to remind player needs to punch quickly
  right arm: master receives "rPUSE." from slave ,expresses slave is pause, master needs to stop checking human punch,   
                      otherwise count increases too slow, master needs to play music or voice to remind player needs to punch quickly
5.master transmits goal value to slave                                                                                      
  data format:                                                                                                              
  left or right : master transmits "gxxxx." to slave,expresses master is setting goal number,this time slave will setting 
  punch_total_goal value and reset punch count,ex:"g0500." that expresses goal is 500   
!!use BT module need to pair echo other!!
First,you need use AT_mode to pair BT module echo other,please refer to BT_pair document
when you pair BT module succeed each other(master and slave),that can receive or transmit data
!!!CAUTION!!!
at AT_mode only can use one BT module to get information
if you have two BT modules needs to use AT_mode to get information
please follow as below
step1
    punchBT_slave punch_BT_R
    //punchBT_slave punch_BT_L;
    void setup(){
            Serial.begin(9600);
            punch_BT_R.punchBT_initial_set(serial1, AT_mode,left);
            //punch_BT_L.punchBT_initial_set(serial2, AT_mode,left);
    }

    void loop() {
    punch_BT_R.AT_mode_function();
        //punch_BT_L.AT_mode_function();
    }

step2
if you get and set BT modlue succeed ,then you can let another BT module to use AT_mode

        //punchBT_slave punch_BT_R
        punchBT_slave punch_BT_L;
        void setup(){
            Serial.begin(9600);
            //punch_BT_R.punchBT_initial_set(serial1, AT_mode,left);
             punch_BT_L.punchBT_initial_set(serial2, AT_mode,left);
         }

            void loop() {
              //punch_BT_R.AT_mode_function();
                    punch_BT_L.AT_mode_function();
            }
about mp3
track0001
track0002
...
*/

#ifndef PUNCHCOUNTERRECEIVER_h
#define PUNCHCOUNTERRECEIVER_h
#include <EEPROM.h>
#include <Segment7_2p3inch_PH.h>
#define default_R_MAX 4999
#define default_L_MAX 4999
#define default_R_L_total 9998
typedef enum mp3{
    mp3_shutup = 1
   ,mp3_useless
   ,mp3_happy_bear
   ,mp3_ridicule
   ,mp3_uselesshere
   ,mp3_reset
   ,mp3_setok
   ,mp3_set_cancel
} mp3_all;

typedef enum red_status{
red_goal_mode = 0
,red_inc_mode
,red_countdown_mode
,red_reset_check_mode
,red_reset_mode
}red_ST;

typedef enum digits_status{
digits_units_mode = 0
,digits_tens_mode 
,digits_hundreds_mode
,digits_thousands_mode
}digits_ST;

typedef enum green_status{
green_play_next_mode = 0
,green_set_goal_mode
,green_cancel_reset_mode
}green_ST;

typedef enum set_goal_status{
setting_goal = 0
,setting_goal_cancel
}set_goal_ST;

typedef enum mp3Status{mp3_ST_pause = 0, mp3_ST_start}mp3_ST;
typedef enum digits_dark_light_status{DTS_light = 0, DTS_dark}DTS_DL_ST;

typedef enum start_pause_done_status{
pause_mode = 0 
,start_mode
,done_mode
}start_pause_done_ST;


enum EEPROM_save_address {
    punch_goalL = 0,
    punch_goalH,
    punchL_nowL,
    punchL_nowH,
    punchR_nowL,
    punchR_nowH
};

class punchCounterReceiver
{
private:
    uint8_t volume_knob;
    uint8_t dataPin,latchPin,clockPin;  //seg7
    uint8_t ctrl_com_pin[com_num];
    int punch_total_goal;
    int punch_total_goal_setBF;
    int set_goal_L,set_goal_R;
    void set_R_L_goal();
    bool seg_switch_goal_now_punch;
    uint8_t mp3_status;
    red_ST red_button_ST;
    uint8_t red_ST_counter;
    int left_arm_number;
    int right_arm_number;
    int left_arm_number_count_down;
    int right_arm_number_count_down;
    uint8_t change_digits_counter;
    green_ST green_button_ST;
    digits_ST digits_button_ST;
    DTS_DL_ST digits_drak_light_ST;
    bool digits_dl; 
    bool digits_sw;
    uint8_t digits_number;
    bool goal_mode;
    set_goal_ST SGST;
    bool set_goal_ST_switch;
    start_pause_done_ST SPD_L,SPD_R;
    bool transmit_reset_flag;
    bool transmit_goal_flag;
    bool play_mp3_cancel_voice;
    bool play_mp3_ridicule_voice;
    bool start_check_stop_5secs;
    int punchL_BF,punchR_BF;
    uint8_t ridicule_voice_counter;
    bool checkL_5secs,checkR_5secs;
    bool play_once;
public:
    punchCounterReceiver();
    ~punchCounterReceiver();
    void initial_punchCounterReceiver(uint8_t volume_Knob, uint8_t segcom_Low, uint8_t segcom_High
                                 ,uint8_t data_pin, uint8_t latch_pin, uint8_t clock_pin);

    void initial_punchCounterMp3();
    void show_punch_data_on7SEG(seg_show word_L, seg_show word_R);
    void show_punch_data_count_down_on7SEG(seg_show word_L, seg_show word_R);
    void show_punch_total_goal_on7SEG(int goal_value);
    void show_reset_check_on7SEG();
    uint8_t get_volume_level();
    void show_mp3player_message(uint8_t type, int value);
    void set_mp3_volume(uint8_t value);
    void mp3_play_selected(int number); 
    void mp3_play_previous(); 
    void mp3_play_next(); 
    void mp3_pause();
    void mp3_start();
    mp3_ST get_mp3_status();
    red_ST get_red_button_ST();
    void change_red_status();
    void red_button_reset();
    void red_button_reset_check();
    void set_left_arm_number_inc(int value);
    void set_right_arm_number_inc(int value);
    void set_left_arm_number_countdown(int value);
    void set_right_arm_number_countdown(int value);
    void set_red_status_reset();
    void set_red_status_goal();
    void set_green_status_goal();
    void set_green_status_play_next();
    green_ST get_green_button_ST();
    void set_green_cancel_reset();
    void change_set_goal_digits();
    digits_ST get_digits_ST();
    void digits_drak_light_change();
    DTS_DL_ST get_digits_dark_light_status();
    void set_digits_sw(bool sw);
    bool get_digits_sw();
    void inc_dec_digits_num();
    void set_digits();
    void set_goal_mode(bool true_false);
    bool get_goal_mode();
    void user_set_goal_ST();
    set_goal_ST user_get_goal_ST();
    int user_get_punch_total_goal();
    void save_punch_total_goal_set_before();
    void cancel_setting_punch_total_goal();
    void save_all_data_to_EEPROM();
    void read_all_data_from_EEPROM();
    void user_set_start_pause_done_status_L(start_pause_done_ST val);
    void user_set_start_pause_done_status_R(start_pause_done_ST val);
    start_pause_done_ST user_get_start_pause_done_status_L();
    start_pause_done_ST user_get_start_pause_done_status_R();
    void user_setting_goal_ok();
    void user_setting_goal_now();
    void user_setting_goal_cancel();
    void user_set_transmit_reset_flag();
    bool user_get_transmit_reset_flag();
    void user_set_transmit_goal_flag();
    bool user_get_transmit_goal_flag();
    int user_get_goal_L();
    int user_get_goal_R();
    void play_cancel_voice();
    void set_play_cancel_voice(bool value);
    void play_ridicule_voice();
    void set_play_ridicule_voice(bool value);
    bool get_play_ridicule_voice();
    void set_5secs_check_flag(bool value);
    bool get_5secs_check_flag();
    void check_start_stop_punch(int numL, int numR);
    void set_check_5secs_L(bool value);
    void set_check_5secs_R(bool value);
    bool get_check_5secs_L();
    bool get_check_5secs_R();
    void set_play_once(bool value);
    bool get_play_once();
};
#endif
