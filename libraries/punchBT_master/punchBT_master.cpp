//For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <punchBT_master.h>

punchBT_master::punchBT_master(void){
	//default
	memset(receiveData_buf, 0 , sizeof(receiveData_buf));
	arrangeData = 0;
	receiveSeq = 0;
	dir = 'l';
	serialN = serial1;
	punch_pause = punch;
	Serial.begin(9600);
}


punchBT_master::~punchBT_master(void){

}

void punchBT_master::punchBT_master_initial_set(ch_serial serial_num, ch_mode modex, ch_dir arm_dir){
	mode = modex;
	dir = arm_dir;
	serialN = serial_num;
	if(serialN	== serial1) {
			Serial1.begin(BT_baudrate);		
	} else if (serialN == serial2) {
			Serial2.begin(BT_baudrate);		
  } else if (serialN == serial3) {
			Serial3.begin(BT_baudrate);		
	}
}

void punchBT_master::AT_mode_function() {

if(mode == AT_mode) {
	//若收到序列埠監控視窗的資料,則送到藍牙模組
	//Receives from serial monitor , sends to bluetooth module 
	if(Serial.available()) { 
			val = Serial.read();
			if(serialN	== serial1) {
				Serial1.print(val);  
			} else if (serialN == serial2) {
				Serial2.print(val);  
  		} else if (serialN == serial3) {
				Serial3.print(val);  
			}
	}

	//若收到藍牙模組的資料,則送到序列埠監控視窗
	//Receives from bluetooth module,sends to serial monitor
			if(serialN	== serial1) {
				if(Serial1.available()) {
					val = Serial1.read();
        	Serial.print(val);
				}
			} else if (serialN == serial2) {
				if(Serial2.available()) {
					val = Serial2.read();
        	Serial.print(val);
				}
  		} else if (serialN == serial3) {
				if(Serial3.available()) {
					val = Serial3.read();
        	Serial.print(val);
				}			
			}
	}
}

int punchBT_master::Master_mode_receive(){
	if(mode == Master_mode) {
			arrangeData = 0;
			if(serialN	== serial1) {
					if (Serial1.available()) {
          	val = Serial1.read();  
						//Serial.println(val);
        		receive_data_function(&val, &receiveSeq, receiveData_buf);               
					}        
			} else if (serialN == serial2) {
					if (Serial2.available()) {
          	val = Serial2.read();    
        		receive_data_function(&val, &receiveSeq, receiveData_buf);               
					}        
  		} else if (serialN == serial3) {
					if (Serial3.available()) {
          	val = Serial3.read();    
        		receive_data_function(&val, &receiveSeq, receiveData_buf);               
					}        
			}
      
			if(receiveSeq == 6) {
					receiveSeq = 0;
					if(punch_pause == pause){
						//Serial.println("pause");
						return 10000;		//10000 = pause			
					} else {
          	arrange_receive_data(receiveData_buf,&arrangeData);	
						return arrangeData; //0-9999(max)
					}
			}       
		  
	} 
	return 0;  	
}

void punchBT_master::receive_data_function(char *val, uint8_t *seq, uint8_t *receive_data_buf) {
		uint8_t check = 0;
    char rl = dir;
    if(*val == rl && *seq == 0) {
      *seq = 1;
    } else if(*seq == 1) {
			//Serial.print(*val);
      if(*val == 'P') {
					*seq = 2;
					punch_pause = pause;
			} else {
					check = *val - '0';
       		if(check >=0 && check <= 9) {
            receive_data_buf[0] = check;
            *seq = 2;
						punch_pause = punch;
       		}
			}
    } else if(*seq == 2) {
			//Serial.print(*val);
      if(*val == 'U') {
					*seq = 3;
					punch_pause = pause;
			}else {
				check = *val - '0';
       	if(check >=0 && check <= 9) {
            receive_data_buf[1] = check;
            *seq = 3;
						punch_pause = punch;
       	}
			}
    } else if(*seq == 3) {
			//Serial.print(*val);
      if(*val == 'S') {
				*seq = 4;
				punch_pause = pause;
			} else {
				check = *val - '0';
       	if(check >=0 && check <= 9) {
            receive_data_buf[2] = check;
            *seq = 4;
						punch_pause = punch;
       	}
			}
    } else if(*seq == 4) {
			//Serial.println(*val);
			if(*val == 'E') {
				*seq = 5;
				punch_pause = pause;
			} else {
      	check = *val - '0';
       	if(check >=0 && check <= 9) {
            receive_data_buf[3] = check;
            *seq = 5;
						punch_pause = punch;
       	}
			}
    } else if(*val == '.' && *seq == 5) {     
        *seq = 6;         
    }    
}

void punchBT_master::arrange_receive_data(uint8_t *receive_data, int *arrange_data) {
    int i = 0;
    int unit[4] = {1000,100,10,1}; 
    for(i = 0; i < 4; i++){
      *arrange_data=*arrange_data + receive_data[i] *unit[i];
       receive_data[i] = 0;
    }  
}

void punchBT_master::Master_mode_transmit_reset() {
		if(mode == Master_mode) {
			if(serialN	== serial1) {
					if(dir == left){
						Serial1.print("lRSET.");
					} else {
						Serial1.print("rRSET.");
					}
			} else if (serialN == serial2) {
					if(dir == left){
						Serial1.print("lRSET.");
					} else {
						Serial1.print("rRSET.");
					}
  		} else if (serialN == serial3) {
					if(dir == left){
						Serial1.print("lRSET.");
					} else {
						Serial1.print("rRSET.");
					}
			}
			
		}  
}


