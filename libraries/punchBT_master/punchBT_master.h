//------------------------
//bluetooth(HC05) module 
//Master:arduino Mega 2560
//use hardware Serial port
//RX1=19(connect BT module TX) TX1=18(connect BT module RX)
//RX2=17(connect BT module TX) TX2=16(connect BT module RX)
//RX3=15(connect BT module TX) TX3=14(connect BT module RX)
//function
//1.master receives punch count data from slave 
//	data format:
//	left arm: slave transmits	punch count: "lxxxx." ex:"l1000."  expresses punch count is 1000
//	right arm: slave transmits  punch count: "rxxxx." ex:"r0887." expresses punch count is 887
//2.master receives reset from slave
//	data format:
//	left arm: slave transmits "l0000." to master ,expresses slave is reset, master needs to clear punch data
//	right arm: slave transmits "r0000." to master ,expresses slave is reset,master needs to clear punch data
//3.master transmits reset to slave
//	data format:
//	left arm: slave receives "lREST." from master,expresses master is reset, slave needs to clear punch data
//	right arm: slave receives "rREST." from master,expresses master is reset, slave needs to clear punch data
//4.msater receives pause from slave
//	data format:
//	left arm: master receives "lPUSE." from slave ,expresses slave is pause, master needs to stop checking human punch 
//						count is too slow(too slow master needs to play music or voice to remind player needs to punch quickly)
//	right arm: master receives "rPUSE." from slave ,expresses slave is pause, master needs to stop checking human punch 
//						count is too slow(too slow master needs to play music or voice to remind player needs to punch quickly)
//5.master will check slave transmits data, if master receives data is not complete at a specific time(time out!) 
//  ,punch receiveData_buf needs to clear.
//!!use BT module need to pair echo other!!
//First,you need use AT_mode to pair BT module echo other,please refer to BT_pair document
//when you pair BT module succeed each other(master and slave),that can receive or transmit data
//!!!CAUTION!!!
/*
at AT_mode only can use one BT module to get information
if you have two BT modules needs to use AT_mode to get information
please follow as below
step1
	punchBT_master punch_BT_R
	//punchBT_master punch_BT_L;
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

		//punchBT_master punch_BT_R
		punchBT_master punch_BT_L;
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
#ifndef PUNCHBT_MASTER_h
#define PUNCHBT_MASTER_h

typedef enum chooseDir {left='l', right='r'} ch_dir;
typedef enum chooseSerial {serial1=1, serial2 ,serial3} ch_serial;
typedef enum chooseMode {AT_mode=0, Master_mode} ch_mode;
typedef enum choosePunch_Pause {punch=0, pause} ch_punch_pause;
#define BT_baudrate 38400
class punchBT_master
{
private:
	uint8_t receiveData_buf[4];
	int arrangeData;
	uint8_t receiveSeq;
	char val;
	bool mode;
	bool punch_pause;
	char dir;
	uint8_t serialN;
	void receive_data_function(char *val, uint8_t *seq, uint8_t *receive_data_buf);
	void arrange_receive_data(uint8_t *receive_data, int *arrange_data);
public:
	punchBT_master();
	~punchBT_master();
	void punchBT_master_initial_set(ch_serial serial_num, ch_mode modex, ch_dir arm_dir);
	void AT_mode_function();
	int Master_mode_receive();
	void Master_mode_transmit_reset();
};

#endif
	
