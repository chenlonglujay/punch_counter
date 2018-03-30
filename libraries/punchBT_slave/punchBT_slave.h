//------------------------
//bluetooth(HC05) module 
//slave:arduino pro mini
//use software Serial port
//function
//1.slave transmits punch count data to master
//	data format:
//	left arm: slave transmits	punch count: "lxxxx." ex:"l1000."  expresses punch count is 1000
//	right arm: slave transmits  punch count: "rxxxx." ex:"r0887." expresses punch count is 887
//2.slave trnasmits reset to master
//	data format:
//	left arm: slave transmits "l0000." to master ,expresses slave is reset, master needs to clear punch data
//	right arm: slave transmits "r0000." to master ,expresses slave is reset,master needs to clear punch data
//3.slave receives reset from master
//	data format:
//	left arm: slave receives "lREST." from master,expresses master is reset, slave needs to clear punch data
//	right arm: slave receives "rREST." from master,expresses master is reset, slave needs to clear punch data
//4.slave tramsmits pause to master
//	data format:
//	left arm: slave transmits "lPUSE." to master ,expresses slave is pause
//	right arm: slave transmits "rPUSE." to master ,expresses slave is pause
//!!use BT module need to pair echo other!!
//First,you need use AT_mode to pair BT module echo other,please refer to BT_pair document
//when you pair BT module succeed each other(master and slave),that can receive or transmit data
//!!!CAUTION!!!
/*
at AT_mode only can use one BT module to get information
if you have two BT modules needs to use AT_mode to get information
please follow as below
step1
	punchBT_slave punch_BT_R
	//punchBT_slave punch_BT_L;
	void setup(){
			Serial.begin(9600);
			punch_BT_R.punchBT_initial_set(serial1, AT_mode,left);
			//punch_BT_L.punchBT_initial_set(serial2, AT_mode,left);
	}

	void loop() {
    punch_BT_R.AT_mode_function();
		//punch_BT_L.AT_mode_function();
	}

step2
if you get and set BT modlue succeed ,then you can let another BT module to use AT_mode

		//punchBT_slave punch_BT_R
		punchBT_slave punch_BT_L;
		void setup(){
			Serial.begin(9600);
			//punch_BT_R.punchBT_initial_set(serial1, AT_mode,left);
			 punch_BT_L.punchBT_initial_set(serial2, AT_mode,left);
		 }

			void loop() {
			  //punch_BT_R.AT_mode_function();
					punch_BT_L.AT_mode_function();
			}
*/
//------------------------
#ifndef PUNCHBT_SLAVE_h
#define PUNCHBT_SLAVE_h
#include <SoftwareSerial.h>
typedef enum chooseDir {left='l', right='r'} ch_dir;
typedef enum chooseMode {AT_mode=0, Slave_mode} ch_mode;
typedef enum choosePunch_Pause {punch=0, pause} ch_punch_pause;
typedef enum resetStatus {reset_no=0, reset_yes} reset_status;
#define BT_baudrate 38400

class punchBT_slave
{
private:
	char transmitData_buf[6];
	uint8_t receiveSeq;
	char val;
	bool mode;
	bool is_reset;
	char dir;
	int transmitData;
	bool punch_pause;
	void arrangeData(char *transmit_data_buf, bool show_data);
public:
	punchBT_slave();
	~punchBT_slave();
	void punchBT_slave_initial_set(ch_mode modex, ch_dir arm_dir);
	void transmitData_add();
	void transmitData_dec();
	void set_transmitData(int val);
	int  get_transmitData();
	void set_punch_pause(ch_punch_pause val);
	void AT_mode_function();
	bool Slave_mode_receive_reset();
	void Slave_mode_transmit(bool show_data);
};

#endif
	
