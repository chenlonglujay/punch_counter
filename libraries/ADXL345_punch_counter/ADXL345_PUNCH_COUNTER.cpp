//For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <ADXL345_PUNCH_COUNTER.h>

PunchCounter::PunchCounter(void){
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

int PunchCounter::get_punch_count(int tuneSensitivity,bool detect, bool printSW){
	int yAxis,zAxis = 0;
    int z,i;
    int punch_value;
	yAxis = adxl345.get_xyzAxis_org(yDataL, yDataH);
    yAxis = abs(yAxis);
    //Serial.print(F("yAxis: "));
    //Serial.println(yAxis);
    //Serial.print(F("tuneSensitivity: "));
    //Serial.println(tuneSensitivity);
    //punch_value = yAxis + tuneSensitivity;
    //Serial.print(F("punch_value: "));
    //Serial.println(punch_value);
    
#if 1
    if(!detect) {
        //Serial.println(F("!detect"));
        return punch_counter;
    }
    punch_value = yAxis + tuneSensitivity;
    if(punch_value > detect_punch_ok_value) {
        for(i=0; i<20; i++) {
             z = adxl345.get_xyzAxis_org(zDataL, zDataH);
           /*
            Serial.print("num");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(z);
            */
            zAxis = zAxis + z;
        }
        //Serial.print("zAxis: ");
        //Serial.println(zAxis);
        if(zAxis > 0) {
            punch_counter++;
        }
    }
    
    return punch_counter;

#endif

#if 0
    int yAxis;
    int punch_value;
	saveBefore = saveNow;
    //adxl345.show_xyzAxis_gravity();
	yAxis = adxl345.get_xyzAxis_org(yDataL, yDataH);
    yAxis = abs(yAxis);
    saveNow = yAxis;
    //Serial.print(F("yAxis: "));
    //Serial.println(yAxis);
    /*
    Serial.print(F("saveBefore: "));
    Serial.print(saveBefore);
    Serial.print(F("  "));
    Serial.print(F("saveNow: "));
    Serial.print(saveNow);
    Serial.print(F("  "));
    */
    
    //Serial.print(F("tuneSensitivity: "));
    //Serial.println(tuneSensitivity);
    punch_value = abs(saveNow-saveBefore)+tuneSensitivity;
    //Serial.print(F("punch_value: "));
    //Serial.println(punch_value);
    if((saveNow > saveBefore) && punch_value >= detect_punch_ok_value && save_counter == 0) {
        //ex: user punches
        //gets values:saveBefoe:87---->saveNow:2048
        //watches surface(O) + fist(D) picture:
        // O=D(gets one:87) original position 
        // ---------> O=D(gets two:2048) punch position
       /* z > 0 that expresses the watches face up when user's punches
        this can avoid to get punched value is correctly(expresses detect user actually is punching)  
        but user's watches actually face down,this is not correct punch way*/  
        for(int i=0; i<20; i++) {
            int z = adxl345.get_xyzAxis_org(zDataL, zDataH);
           /*
            Serial.print("num");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(z);
            */
            zAxis = zAxis + z;
            //delay(1);
        }
        //Serial.print("zAxis: ");
        //Serial.println(zAxis);
        if(zAxis > 0) {
            save_counter = 1;
            if(printSW) {
                show_punch_count(saveNow,saveBefore,0);
            }
        }
        //return punch_counter;
    } else
      
    if((saveNow < saveBefore) &&/* punch_value >= detect_go_back_ok_value &&*/ save_counter == 1) {
        //ex: user punches that going back
        //gets values:saveBefoe:33<-----saveNow:2048
        //watches surface(O) + fist(D) picture: 
        //<------ O=D(gets two:2048) punch position,this is going back
        //O=D(gets three:33) original position,going back that has finished
            save_counter = 0;
            punch_counter++;
	        saveBefore = saveNow;
            if(printSW) {
                show_punch_count(saveNow,saveBefore,1);
            }
            //return punch_counter;
    } /*else if(save_counter == 1){
            //if punch gets back was no detected,cancel this detect
            save_counter = 0;
    }   */
    //delay(1); 
	saveBefore = saveNow;
    return punch_counter;
#endif


}

void PunchCounter::clear_punch_counter(){
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

