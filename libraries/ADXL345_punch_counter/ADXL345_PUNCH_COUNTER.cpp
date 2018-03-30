//For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <ADXL345_PUNCH_COUNTER.h>

PunchCounter::PunchCounter(void){
	punchSensitivity=defaultSensitivity;
	punch_counter=0;
	save_counter=0;
	saveNow=0;
	saveBefore=0;
}

PunchCounter::~PunchCounter(void){

}

void PunchCounter::PunchCounter_initial(devID dev_id,SG CHGY, int now_punch_counter){
	adxl345.getGravity=set_gravity(CHGY);
	adxl345.getDeviceID=set_deviceID(dev_id);
	adxl345.ADXL345_initial_set(adxl345.getDeviceID, adxl345.getGravity);
	punch_counter=now_punch_counter;	//read punch count from eeprom
}

uint8_t PunchCounter::set_gravity(SG CHGY){
	if(CHGY == s2g){
		return 0;	
	}else if(CHGY == s4g){
		return 1;
	}else if(CHGY ==s8g){
		return 2;
	}else{
		return 3;
	}
}

void PunchCounter::set_measureMode(){
	adxl345.enableMeasureMode();
}

void PunchCounter::set_standbyMode(){
	adxl345.enableStandbyMode();
}

uint8_t PunchCounter::set_deviceID(devID dev_id){
	if(dev_id ==SDO_1){
		return 0;
	}else{
		return 1;
	}
}

int PunchCounter::get_punch_count(int tuneSensitivity, bool printSW){
	int yAxis;
	saveBefore=saveNow;
	yAxis = adxl345.get_xyzAxis_org(yDataL, yDataH);
	saveNow=yAxis;
	//adxl345.show_xyzAxis_gravity();
	if(abs(saveNow-saveBefore) >= (punchSensitivity+tuneSensitivity)){
	  //move situation is fist punch and get back that will get data has big changed.
		//freeze situation data only has small changed.
		save_counter++;
		if(save_counter == 1){
				if(printSW){
					show_punch_count(saveNow,saveBefore,0);
				}
		}

		if(save_counter == 2){
				save_counter=0;
				punch_counter++;
				if(printSW){			  
					show_punch_count(saveNow,saveBefore,1);
				}
		}

	}
	return punch_counter;
	delay(5);
}

void PunchCounter::clean_punch_counter(){
	punch_counter=0;
}

void PunchCounter::show_punch_count(int saveNow, int saveBefore, bool is2){
	Serial.println("-------------------------- ");
	Serial.print("saveBefore: ");
	Serial.print(saveBefore);
	Serial.print("                   ");
	Serial.print("saveNow: ");
	Serial.println(saveNow);
	Serial.println("-------------------------- ");
	if(is2){
		Serial.print("punch count: ");
		Serial.println(punch_counter);
	}
	Serial.println("                             ");						
}
