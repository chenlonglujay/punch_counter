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
    transmitData_buf[0] = 'g';
    transmitData_buf[5] = '.';
    BTL_rceive_check = 0; 
    BTL_time_out = 0;
    BTR_rceive_check = 0;
    BTR_time_out = 0;
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
    BTL_rceive_check = 0; 
    BTL_time_out = 0;
    BTR_rceive_check = 0;
    BTR_time_out = 0;
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
        if(dir == left){
            if(get_BTL_receive_check() && BTL_time_out==1){
                //Serial.println(F("BTL timeout!!"));
                set_BTL_receive_check(0);
                set_BTL_time_out(0);
                receiveSeq = 0;
	            return BT_nothing;  	
            }
        } else if(dir == right) {
            if(get_BTR_receive_check() && BTR_time_out==1){
                //Serial.println(F("BTR timeout!!"));
                set_BTR_receive_check(0);
                set_BTR_time_out(0);
                receiveSeq = 0;
	            return BT_nothing;  	
            }
        }

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
						return BT_pause;		//10000 = pause			
					} else {
          	            arrange_receive_data(receiveData_buf,&arrangeData);	
						return arrangeData; //0-9999(max)
					}
			}       
		  
	} 
	return BT_nothing;  	
}

void punchBT_master::receive_data_function(char *val, uint8_t *seq, uint8_t *receive_data_buf) {
		uint8_t check = 0;
    char rl = dir;
      
    if(*val == rl && *seq == 0) {
      *seq = 1;
        if(rl == left){
            set_BTL_receive_check(1);
            //Serial.println(F("set_BTL_receive_check(1)"));
        } else if(rl == right) {
            set_BTR_receive_check(1);
            //Serial.println(F("set_BTR_receive_check(1)"));
        }
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
        if(rl == left){
            set_BTL_receive_check(0);
            set_BTL_time_out(0);
            //Serial.println(F("set_BTL_receive_check(0)"));
            //Serial.println(F("-----------------------"));
        } else if(rl == right) {
            set_BTR_receive_check(0);
            //Serial.println(F("set_BTR_receive_check(0)"));
            //Serial.println(F("-----------------------"));
        }
     
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
						Serial1.print("lREST.");
					} else {
						Serial1.print("rREST.");
					}
			} else if (serialN == serial2) {
					if(dir == left){
						Serial2.print("lREST.");
					} else {
						Serial2.print("rREST.");
					}
  		} else if (serialN == serial3) {
					if(dir == left){
						Serial3.print("lREST.");
					} else {
						Serial3.print("rREST.");
					}
			}
			
		}  
}

void punchBT_master::Master_mode_transmit_goal(int goal_value) {

    arrange_transmit_data(transmitData_buf, goal_value, 0);

	if(mode == Master_mode) {
		if(serialN	== serial1) {
			Serial1.print(transmitData_buf);
		} else if (serialN == serial2) {
			Serial2.print(transmitData_buf);
  		} else if (serialN == serial3) {
			Serial3.print(transmitData_buf);
		}
			
	}  

}

//change int data to char
void punchBT_master::arrange_transmit_data(char *transmit_data_buf, int goal_value, bool show_data) {
     int i = 0;                                                                                                                                      
     int buf = goal_value;
         if(show_data) {
                Serial.print(F("goal_value:"));
                Serial.println(goal_value);
         }     
     int unit[4] = {1000, 100, 10, 1};
     uint8_t check = 0;
         if(show_data) {
                Serial.print(F("buf[0]:"));
                Serial.print(transmit_data_buf[0]);
                Serial.print("  ");
         }     
     for(i = 0; i < 4; i++) {
        transmit_data_buf[i+1]= (buf / unit[i]) + '0';   //int to char
        buf = buf % unit[i];
                if(show_data) {
                        Serial.print(F("buf"));
                        Serial.print(F("["));
                        Serial.print(i+1);
                        Serial.print(F("]:"));
                        Serial.print(transmit_data_buf[i+1]);
                        Serial.print("  ");
                }
     }         
         if(show_data) {
                Serial.print(F("buf[5]:"));
                Serial.println(transmit_data_buf[5]);
            }  
}          

void punchBT_master::set_BTL_receive_check(bool value) {
    BTL_rceive_check = value;
}
    
bool punchBT_master::get_BTL_receive_check() {
    return BTL_rceive_check;
}

void punchBT_master::set_BTL_time_out(bool value) {
     BTL_time_out = value;
}

void punchBT_master::set_BTR_receive_check(bool value) {
    BTR_rceive_check = value;
}
    
bool punchBT_master::get_BTR_receive_check() {
    return BTR_rceive_check;
}

void punchBT_master::set_BTR_time_out(bool value) {
     BTR_time_out = value;
}

                                                                                                               
