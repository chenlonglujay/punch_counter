//For Arduino 1.0 and earlier
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <punchBT_slave.h>
SoftwareSerial BT(10,11); //arduino RX(BT TX) ,TX(BT RX)

punchBT_slave::punchBT_slave(void) {
	//default
	transmitData = 0;
	receiveSeq = 0;
	dir = 'l';
	punch_pause = punch;
	is_reset = reset_no;
	Serial.begin(9600);
	BT.begin(BT_baudrate);
    goal_value = 0;
}


punchBT_slave::~punchBT_slave(void) {

}

void punchBT_slave::punchBT_slave_initial_set(ch_mode modex, ch_dir arm_dir) {
	mode = modex;
	dir = arm_dir;
	if(dir == left) {
		transmitData_buf[0] = 'l';
	} else {
		transmitData_buf[0] = 'r';
	}
	
	transmitData_buf[1] = '0';
	transmitData_buf[2] = '0';
	transmitData_buf[3] = '0';
	transmitData_buf[4] = '0';
	transmitData_buf[5] = '.';

}

void punchBT_slave::AT_mode_function() {

	if(mode == AT_mode) {
		if(Serial.available()) { 
      val = Serial.read();
      BT.print(val); 
  	}
  	if (BT.available()) {
    	val = BT.read();
    	Serial.print(val);
  	} 	
	}
}

void punchBT_slave::Slave_mode_transmit(bool show_data) {
	if(punch_pause == pause) {
			if(dir == right)    
    		BT.print("rPUSE.");
			else 
    		BT.print("lPUSE.");

  } else {
    arrangeData(transmitData_buf, show_data);
    BT.print(transmitData_buf);   
  }  
}



bool punchBT_slave::Slave_mode_receive_reset() {
  if (BT.available()) {
      val = BT.read();
        if(val == dir && receiveSeq == 0){
          receiveSeq = 1;
        } else if (val == 'R' && receiveSeq == 1){
          receiveSeq = 2;
        } else if (val == 'S' && receiveSeq == 2){
          receiveSeq = 3;
        } else if (val == 'E' && receiveSeq == 3){
          receiveSeq = 4;
        } else if (val == 'T' && receiveSeq == 4){
          receiveSeq = 5;
        } else if (val == '.' && receiveSeq == 5){
          receiveSeq = 6;
        }

				if (receiveSeq < 6) {
					return reset_no;
				} else if (receiveSeq == 6) {
					receiveSeq = 0;
					transmitData = 0;
					return reset_yes;
				}
  }
	return reset_no;
	 
}



//change int data to char
void punchBT_slave::arrangeData(char *transmit_data_buf, bool show_data) {
     int i = 0;
     int buf = transmitData; 
		 if(show_data) {
		 		Serial.print("transmitData:");
		 		Serial.println(transmitData);
		 }
     int unit[4] = {1000, 100, 10, 1};
     uint8_t check = 0;
		 if(show_data) {
		 		Serial.print("buf[0]:");
		 		Serial.print(transmit_data_buf[0]);
		 		Serial.print("  ");
		 }
     for(i = 0; i < 4; i++) {        
        transmit_data_buf[i+1]= (buf / unit[i]) + '0';   //int to char
        buf = buf % unit[i];
				if(show_data) {
						Serial.print("buf");
						Serial.print("[");
						Serial.print(i+1);
						Serial.print("]:");
						Serial.print(transmit_data_buf[i+1]);
						Serial.print("  ");
				}
     }
		 if(show_data) {
		 		Serial.print("buf[5]:");
		 		Serial.println(transmit_data_buf[5]);
			}
}

void  punchBT_slave::transmitData_add() {
	transmitData++;
}

void punchBT_slave::transmitData_dec() {
	transmitData--;
}

void punchBT_slave::set_transmitData(int val) {
	transmitData = val;
}


int punchBT_slave::get_transmitData() {
	return transmitData;
}

void punchBT_slave::set_punch_pause(ch_punch_pause val) {
		punch_pause = val;
}

dw_status punchBT_slave::Slave_mode_receive_goal_or_reset() {
  if (BT.available()) {
      val = BT.read();
        if(val == dir && receiveSeq == 0) {
            receiveSeq = 1;
            dw_r_g = dw_reset;
        } else if (val == 'g' && receiveSeq ==0) {
            receiveSeq = 1;
            dw_r_g = dw_goal;
        }
        
        if(dw_r_g == dw_reset) {
            return  deal_with_reset_event(val);
        } else {
            return  deal_with_goal_event(val);
        }
 
    }
 return nothing;
}

dw_status punchBT_slave::deal_with_reset_event(char val) {

        if (val == 'R' && receiveSeq == 1){
          receiveSeq = 2;
        } else if (val == 'S' && receiveSeq == 2){
          receiveSeq = 3;
        } else if (val == 'E' && receiveSeq == 3){
          receiveSeq = 4;
        } else if (val == 'T' && receiveSeq == 4){
          receiveSeq = 5;
        } else if (val == '.' && receiveSeq == 5){
          receiveSeq = 6;
        }

		if (receiveSeq < 6) {
			return donot_reset;
		} else if (receiveSeq == 6) {
			receiveSeq = 0;
			transmitData = 0;
			return do_reset;
		}
}

dw_status punchBT_slave::deal_with_goal_event(char val) {
        int temp;
        if (val == 'g') {
            return nothing;
        } 
        temp = val - '0';   //char convert to int
        //Serial.print(F("val:"));
        //Serial.println(val);
        //Serial.print(F("temp:"));
        //Serial.println(temp);


        if(receiveSeq == 1){
            goal_value = temp *1000; 
           // Serial.print(F("seq1:"));
            //Serial.println(goal_value);
            receiveSeq = 2;
        } else if (receiveSeq == 2){
            goal_value = goal_value + temp *100; 
            //Serial.print(F("seq2:"));
            //Serial.println(goal_value);
            receiveSeq = 3;
        } else if (receiveSeq == 3){
            goal_value = goal_value + temp *10; 
            //Serial.print(F("seq3:"));
            //Serial.println(goal_value);
            receiveSeq = 4;
        } else if (receiveSeq == 4){
            goal_value = goal_value + temp; 
            //Serial.print(F("seq4:"));
            //Serial.println(goal_value);
            receiveSeq = 5;
        } else if (val == '.' && receiveSeq == 5){
            receiveSeq = 0;
            return get_goal;
        }
        
    if(receiveSeq < 5){
        return nothing;
    } 

}
	

int punchBT_slave::get_goal_value() {
    return goal_value;
}

