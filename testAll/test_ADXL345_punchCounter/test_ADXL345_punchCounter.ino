//test ADXL345_PUNCH_COUNTER  
//test EEPROM(arduino internal EEPROM)
#include <ADXL345_PUNCH_COUNTER.h>
#include <EEPROM.h>
PunchCounter punch_count;
#define showDetail 1
void setup() {
  Serial.begin(9600); 
  int readCount=0;
  readCount=getPunchCounterFromEEPROM();    
  punch_count.PunchCounter_initial(SDO_1, s8g, readCount);
  delay(5);
  Serial.println("start");
}

void loop() {
    int count;
    count=punch_count.get_punch_count(100,showDetail);
    savePunchCount(count);    
    //savePunchCount(1000);     //test count is 1000
}

int getPunchCounterFromEEPROM(){
  byte tempL=EEPROM.read(0);
  byte tempH=EEPROM.read(1);
  int saveCount=(tempH << 8)+tempL;
  //Serial.print("saveCount: ");
  //Serial.println(saveCount);
  return saveCount;
}

void savePunchCount(int count){
  int temp=count;
  byte tempH,tempL;
  tempL=temp & 0x00ff;
  //Serial.print("tempL: ");
  //Serial.println(tempL);
  EEPROM.write(0,tempL);
  temp=count;
  tempH=temp >> 8;
  //Serial.print("tempH: ");
  //Serial.println(tempH);
  EEPROM.write(1,tempH);  
}

