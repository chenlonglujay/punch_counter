//---------------------
//Arduino Pro mini
//read and save data to internal eeprom
//  punch count MAX:9999 MIN:0
//	timer format:dd:hh:mm:ss
//	max timer 8639999 second(99:23:59:59)
//	if dd > 99 that will set 0 
//	if hh=24 dd will set 01(1 day)
//	if mm=60 hh will set 01(1 hour)
//	if ss=60 mm wikk set 01(1 minute)
//display data on OLED
//reset and page button:
//	reset  and page button used same button, this button set interrput
//	reset: need push down 3 second
//	page:	 push down once will change page1 or page2 on OLED
//---------------------
#ifndef PUNCHCOUNTERWATCH_h
#define PUNCHCOUNTERWATCH_h
#include <ADXL345_PUNCH_COUNTER.h>
#include <EEPROM.h>
#define counter_max 4999
#define page_counter_max 3
#define punch_goal_default 4999
#define highest_voltage 1023  //4V 
#define lowest_voltage 750  //3V 
/*  4.09:3 = 1023:x
      x = 750
*/
//battery detect referance below
//https://forum.arduino.cc/index.php?topic=420527.0


enum EEPROM_save_address {
    punch_counterL = 0,
    punch_counterH,
    time_second,
    time_minute,
    time_hour,                                                                                                                                                                                                     
    time_day,
    punch_goalL,
    punch_goalH
};

typedef struct timerSaveFormat{
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}timerSaveFMT;

enum {watch_pause=0, watch_start};
typedef enum which_page{
    page1 = 0,
    page2,
    page3,
    page_reset
}wh_page;

typedef enum check_goal_finish{goal_not_yet = 0, goal_done}goal_finish; 
typedef enum check_transmit_0{transmit_0_not = 0, transmit_0_done}transmit_BT_reset;

class punchCounterWatch
{
private:
	uint8_t count_sensitivity_pin;
	uint8_t battery_detect_pin;
	int punchCounter;
    int punch_goal;
	unsigned long timeCounter;
	void arrangeTimerDataForSave(unsigned long timeCounter);
	unsigned long arrangeTimerDataForRead();
	timerSaveFMT TMSF;
	int getSensitivity();
	bool start_pause_status;
    uint8_t page_counter;
    bool change_page;
    transmit_BT_reset TBR;
    bool punch_pause_switch_do_once_transmit;
    bool punch_detect;
    int punch_counter_before;
public:
	punchCounterWatch();
	~punchCounterWatch();
	int getPunchCountFromEEPROM();
	int getPunchGoalFromEEPROM();
	void savePunchCountToEEPROM();
	void savePunchGoalToEEPROM(int val);
	void punchCounterWatch_initial_set(uint8_t senstivityPin, uint8_t batteryDetectPin);
	void saveTimerDataToEEPROM();
	unsigned long getTimerDataFromEEPROM();
	int getHumanPunchCount();	
	void timer_add_1_second();
	timerSaveFMT getTimeData(); 
	void resetAll();
	void resetTime();
	void resetPunch();
	void set_start();
	void set_pause();
	bool get_start_pause_status();
	uint8_t get_battery_percent(); 
	uint8_t get_sensitivity_percent();
    void add_page();
    void set_which_page(wh_page pp);
    wh_page get_page_count();
    bool change_page_check();
    int get_punchCounter();
    int get_punchGoal();
    goal_finish check_goal_done(int goal, int punch_now);
    void set_transmit_BT_reset(transmit_BT_reset val);
    transmit_BT_reset get_transmit_BT_reset();
    bool get_punch_pause_switch_do_once_transmit();
    void set_punch_pause_switch_do_once_transmit();
    void set_punch_detect(bool detect);
    bool get_punch_detect();
};

#endif
