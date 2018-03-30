#ifndef ADXL345_PiUNCH_COUNTER_h
#define ADXL345_PUNCH_COUNTER_h
//use this class can get human punch count
//only use y-axis data to detect
#include <ADXL345.h>
#define defaultSensitivity 1000
typedef enum setGravity{s2g=0,s4g,s8g,s16g} SG;
typedef enum setDeivceID{SDO_1=0,SDO_0} devID;
class PunchCounter:protected ADXL345
{
private:
	ADXL345 adxl345;
	int punch_counter;
	uint8_t save_counter;
	int saveNow,saveBefore;
	int punchSensitivity;
	void show_punch_count(int saveNow, int saveBefore, bool is2);
	uint8_t set_gravity(SG CHGY);
	uint8_t set_deviceID(devID dev_id);
public:
	PunchCounter();
	~PunchCounter();
	void PunchCounter_initial(devID dev_id, SG CHGY, int now_punch_counter);
	int get_punch_count(int tuneSensitivity, bool printSW);
	void clean_punch_counter();
	void set_measureMode();
	void set_standbyMode();
};

#endif
