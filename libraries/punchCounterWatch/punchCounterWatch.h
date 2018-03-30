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
typedef struct timerSaveFormat{
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}timerSaveFMT;

enum {pause=0, start};
class punchCounterWatch
{
private:
	uint8_t count_sensitivity_pin;
	int punchCounter;
	unsigned long timeCounter;
	void arrangeTimerDataForSave(unsigned long timeCounter);
	unsigned long arrangeTimerDataForRead();
	timerSaveFMT TMSF;
	int getSensitivity();
	bool start_pause_status;
public:
	punchCounterWatch();
	~punchCounterWatch();
	int getPunchCountFromEEPROM();
	void savePunchCountToEEPROM();
	void punchCounterWatch_initial_set(uint8_t senstivityPin);
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
};

#endif
