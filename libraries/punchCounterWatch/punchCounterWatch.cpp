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
    page_counter = 0;
}

punchCounterWatch::~punchCounterWatch(void) {
}

void punchCounterWatch::punchCounterWatch_initial_set(uint8_t sensitivityPin
, uint8_t batteryDetectPin) {
    analogReference(INTERNAL);//an built-in reference, equal to 1.1(1024) volts on the ATmega328
	count_sensitivity_pin = sensitivityPin;
	battery_detect_pin = batteryDetectPin;
	pinMode(sensitivityPin, INPUT);
	pinMode(batteryDetectPin, INPUT);
	punchCounter = getPunchCountFromEEPROM();
	//Serial.println(F("punchCounter initial:"));
	//Serial.println(punchCounter);
    timeCounter = getTimerDataFromEEPROM();
	punch_count.PunchCounter_initial(SDO_1, s8g, punchCounter);
    punch_goal = getPunchGoalFromEEPROM();
	//Serial.println(F("punch_goal initial:"));
	//Serial.println(punch_goal);
    set_pause();
}

int punchCounterWatch::getPunchCountFromEEPROM() {
	byte tempL = EEPROM.read(punch_counterL);
	byte tempH = EEPROM.read(punch_counterH);
	int getCount = (tempH << 8) + tempL;
	return getCount;
}

int punchCounterWatch::getPunchGoalFromEEPROM() {
	byte tempL = EEPROM.read(punch_goalL);
	byte tempH = EEPROM.read(punch_goalH);
	int getGoal = (tempH << 8) + tempL;
	//Serial.println(F("get punch_goal from eeprom:"));
	//Serial.println(getGoal);
	return getGoal;
}


void punchCounterWatch::savePunchCountToEEPROM() {
		int temp;
		uint8_t tempH,tempL;
		if(punchCounter > counter_max) {
			temp = counter_max;
		} else {
			temp = punchCounter;
		}		
		tempL = temp & 0x00ff;
		EEPROM.write(punch_counterL,tempL);
		tempH = temp >> 8;
		EEPROM.write(punch_counterH,tempH);
}

void punchCounterWatch::savePunchGoalToEEPROM(int val) {
		int temp = val;
		uint8_t tempH,tempL;
        //Serial.print("save goal to eeprom:");
        //Serial.println(temp);
		if(temp > punch_goal_default || temp == 0) {
			temp = punch_goal_default;
        }
		tempL = temp & 0x00ff;
		EEPROM.write(punch_goalL,tempL);
		tempH = temp >> 8;
		EEPROM.write(punch_goalH,tempH);
}

void punchCounterWatch::saveTimerDataToEEPROM() {
	arrangeTimerDataForSave(timeCounter);
	EEPROM.write(time_second,TMSF.second);
	EEPROM.write(time_minute,TMSF.minute);
	EEPROM.write(time_hour,TMSF.hour);
	EEPROM.write(time_day,TMSF.day);
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
	TMSF.second = EEPROM.read(time_second);
	TMSF.minute = EEPROM.read(time_minute);
	TMSF.hour = EEPROM.read(time_hour);
	TMSF.day = EEPROM.read(time_day);
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
	//Serial.print("sensitivity: ");
	//Serial.println(val);
	return val;
}

int punchCounterWatch::getHumanPunchCount() {
	float SETY_temp = getSensitivity() / 1.2;
    int SETY = SETY_temp;
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
	/*
    highest voltage:4.09V(1023)
    lowest voltage:3.0V(750)
    You'll get an analogue value of 1023 when 1.1V is applied to the pin.
    divider:
    a suitable voltage divider consisting of two resistors, with the top connected to the battery
    ,and the juncton of the resistors connected to the analogure input pin.
    4.09V * 11k/(11k+30k) = 1.097V -->1023
    3V * 11K/(11K+30K) = 0.804V -->750  
    if 3.8V 
    3.8 * 11K/(11K+30K) = 1.019V -->967
    967 - 750 = 217
    217 / 271(range) = 0.8
    detect = 0.8 * 100 
           = 80%  
    */
    int val = analogRead(battery_detect_pin);
    //Serial.print(F("read battery voltage:"));
    //Serial.println(val);
    int range;
    float detect;
    if(val <= lowest_voltage) {
        val = 0;
    } else {
        val = val - lowest_voltage;
	    range = highest_voltage - lowest_voltage;   
        detect = (float)val / range;   
	    detect = detect * 100;
	    val = (int)detect;
    }
	return val;  
}

uint8_t punchCounterWatch::get_sensitivity_percent() {
	int val = getSensitivity();
	float detect = (float)val / 1023.00;
	detect  = detect * 100;
	val = (int)detect;
	return val;
}

void punchCounterWatch::add_page() {
    page_counter++;
    if (page_counter >= page_counter_max) {
        page_counter = 0;
    }
    change_page = 1;
}

void punchCounterWatch::set_which_page(wh_page pp) {
    switch (pp){
    case 0:
        page_counter = 0;
    break;
    case 1:
        page_counter = 1;
    break;
    case 2:
        page_counter = 2;
    break;
    case 3:
        page_counter = 3;
    break;
    default:
        page_counter = 0;
    break;
    }
} 

wh_page punchCounterWatch::get_page_count() {
    wh_page pp; 
    switch (page_counter){
    case 0:
        pp = page1;
    break;
    case 1:
        pp = page2;
    break;
    case 2:
        pp = page3;
    break;
    case 3:
        pp = page_reset;
    break;
    default:
        pp = page1;
    break;
    }
    return pp;
}

bool punchCounterWatch::change_page_check() {
    if(change_page){
       change_page = 0; 
       return  1;
    } else {
        return 0;
    }
}

int punchCounterWatch::get_punchCounter() {
    return punchCounter;
}

int punchCounterWatch::get_punchGoal() {
    return punch_goal;
}

goal_finish punchCounterWatch::check_goal_done(int goal, int punch_now) {
    if (punch_now == goal) {
       return goal_done; 
    } else {
       return goal_not_yet; 
    }
}

void punchCounterWatch::set_transmit_BT_reset(transmit_BT_reset val) {
    TBR = val;
}

transmit_BT_reset punchCounterWatch::get_transmit_BT_reset() {
    return TBR;
}


