//For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#define showDetail 0
#include <punchCounterWatch.h>

PunchCounter punch_count;

punchCounterWatch::punchCounterWatch(void) {
	punchCounter = 0;
	timeCounter = 0;
	TMSF.second = 0;
	TMSF.minute = 0;
	TMSF.hour = 0;
	TMSF.day = 0;
	save_sensitivity = 0;
}

punchCounterWatch::~punchCounterWatch(void) {
}

void punchCounterWatch::punchCounterWatch_initial_set(uint8_t sensitivityPin
, uint8_t batteryDetectPin) {
	count_sensitivity_pin = sensitivityPin;
	battery_detect_pin = batteryDetectPin;
	pinMode(sensitivityPin, INPUT);
	pinMode(batteryDetectPin, INPUT);
	punchCounter = getPunchCountFromEEPROM();
	timeCounter = getTimerDataFromEEPROM();
	punch_count.PunchCounter_initial(SDO_1, s8g, punchCounter);
	set_pause();
}

int punchCounterWatch::getPunchCountFromEEPROM() {
	byte tempL = EEPROM.read(0);
	byte tempH = EEPROM.read(1);
	int getCount = (tempH << 8) + tempL;
	return getCount;
}

void punchCounterWatch::savePunchCountToEEPROM() {
		int temp;
		uint8_t tempH,tempL;
		if(punchCounter > 9999) {
			temp = 9999;
		} else {
			temp = punchCounter;
		}		
		tempL = temp & 0x00ff;
		EEPROM.write(0,tempL);
		tempH = temp >> 8;
		EEPROM.write(1,tempH);
}

void punchCounterWatch::saveTimerDataToEEPROM() {
	arrangeTimerDataForSave(timeCounter);
	EEPROM.write(2,TMSF.second);
	EEPROM.write(3,TMSF.minute);
	EEPROM.write(4,TMSF.hour);
	EEPROM.write(5,TMSF.day);
}

void punchCounterWatch::arrangeTimerDataForSave(unsigned long timeCounter) {
	unsigned long temp = timeCounter;
	unsigned long mm_temp,hh_temp;	
	int dd,hh,mm,ss;	
	//Serial.print("save timeCounter:");
	//Serial.println(temp);
  //second
	ss = temp % 60;
	TMSF.second = ss;	
	//minute
	mm_temp = temp / 60;
	mm = mm_temp % 60;
	TMSF.minute = mm;
	//hour
	hh_temp = mm_temp / 60 ;
	if(hh_temp >= 24) {
		hh = hh_temp % 24;
	} else {
		hh = hh_temp;
	}
	TMSF.hour = hh;
	//day
	dd = hh_temp / 24;
	TMSF.day = dd;
	/*
	Serial.println("save dd:hh:mm:ss");
	Serial.print(dd);
	Serial.print(": ");
	Serial.print(hh);	
	Serial.print(": ");
	Serial.print(mm);	
	Serial.print(": ");
	Serial.println(ss);		
	*/
}

unsigned long punchCounterWatch::getTimerDataFromEEPROM() {
	TMSF.second = EEPROM.read(2);
	TMSF.minute = EEPROM.read(3);
	TMSF.hour = EEPROM.read(4);
	TMSF.day = EEPROM.read(5);
	/*		
	Serial.println("get dd:hh:mm:ss");
	Serial.print(TMSF.day);
	Serial.print(": ");
	Serial.print(TMSF.hour);
	Serial.print(": ");
	Serial.print(TMSF.minute);
	Serial.print(": ");
	Serial.println(TMSF.second);
	*/
	return arrangeTimerDataForRead();
}

unsigned long punchCounterWatch::arrangeTimerDataForRead() {
	unsigned long temp = 0;
	unsigned long ch = 3600;
	temp = TMSF.second + TMSF.minute*60;
 	temp = temp + TMSF.hour*ch;
	ch = ch * 24;
	temp = temp + TMSF.day*ch;
	//Serial.print("timer temp: ");
	//Serial.println(temp);
	return temp; 	
}

int punchCounterWatch::getSensitivity() {
	int val = analogRead(count_sensitivity_pin);	//0-1023
	save_sensitivity = val;
	//Serial.print("sensitivity: ");
	//Serial.println(val);
	return val;
}

int punchCounterWatch::getHumanPunchCount() {
	int SETY = getSensitivity();
	punchCounter = punch_count.get_punch_count(SETY,showDetail);
	return punchCounter;	
}

void punchCounterWatch::timer_add_1_second() {
	timeCounter++;		//add 1 second
}

timerSaveFMT punchCounterWatch::getTimeData() {
		return TMSF;
}

void punchCounterWatch::resetAll() {
	//Serial.println("reset all");
	resetPunch();
	resetTime();
}

void punchCounterWatch::resetTime() {
	timeCounter = 0;
	saveTimerDataToEEPROM();
}

void punchCounterWatch::resetPunch() {
	punchCounter = 0;
	punch_count.clear_punch_counter();
	savePunchCountToEEPROM();
}

void punchCounterWatch::set_start() {
	start_pause_status = watch_start;
}

void punchCounterWatch::set_pause() {
	start_pause_status = watch_pause;
}

bool punchCounterWatch::get_start_pause_status() {
	return start_pause_status;
}

uint8_t punchCounterWatch::get_battery_percent() {
	int val = analogRead(battery_detect_pin);	
	float detect = (float)val / 1023.00;
	detect = detect *100;
	val = (int)detect;
	return val;  
}

uint8_t punchCounterWatch::get_sensitivity_percent() {
	int val = save_sensitivity;
	float detect = (float)val / 1023.00;
	detect  = detect * 100;
	val = (int)detect;
	return val;
}
