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
#define counter_max 9999
#define page_counter_max 3
#define punch_goal_default 4999

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
};

#endif
