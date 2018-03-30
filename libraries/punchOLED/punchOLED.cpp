
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Wire.h"

#include "punchOLED.h"
#include "OLED_database_watch.c"

punchOLED::punchOLED(){
}

punchOLED::~punchOLED(){
}

void punchOLED::punchOLED_initial(){
	initial();	//inherit form OLED_SSD1306
	fill_screen(0x00, 0x00);  //clear screen
}

void punchOLED::showCN(unsigned char x, unsigned char y, unsigned char N){
	display16x16(x, y, N, CN16x16);
}


void punchOLED::showNUM(unsigned char x, unsigned char y, unsigned char N){
	display8x16(x, y, N, NUM8x16);
}

#if ENC_SW
void punchOLED::showENC(unsigned char x, unsigned char y, unsigned char N){
	display8x16(x, y, N, ENC8x16);
}
#endif

#if ENS_SW
void punchOLED::showENS(unsigned char x, unsigned char y, unsigned char N){
	display8x16(x, y, N, ENS8x16);
}
#endif

void punchOLED::showSSYM(unsigned char x, unsigned char y, unsigned char N){
	display8x16(x, y, N, SSYM8x16);
}

#if picture8x16
void punchOLED::showPicture8x16(unsigned char x, unsigned char y, unsigned char N){
	display8x16(x, y, N, picture8x16);
}
#endif

void punchOLED::showPicture16x16(unsigned char x, unsigned char y, unsigned char N){
	display16x16(x, y, N, picture16x16);
}

void punchOLED::showPicture128x64_lab(){
	display128x64(Picture_lab);
}

void punchOLED::showPicture128x64_fist(){
	display128x64(Picture_fist);
}

void punchOLED::show_watch_page1(punch_count_OLED *PCOLED, time_save_OLED *TSOLED){
		uint8_t x_pos16 = 16;
		uint8_t y_pos = 2;
		uint8_t x_pos8 = 8;
		uint8_t crow = 0;
		//row 1 show title
		showCN(2 * x_pos16, 0, 0);	
		showCN(3 * x_pos16, 0, 1);
		showCN(4 * x_pos16, 0, 2);
		showCN(5 * x_pos16, 0, 3);
	
		//row2 	show punch count
		crow = 1;		
		showCN(0 * x_pos16, crow* y_pos, 10);	
		showCN(1 * x_pos16, crow*y_pos, 4);
		showCN(2 * x_pos16, crow*y_pos, 3);
		showSSYM(3 * x_pos16, crow*y_pos, 0);	//:
		showNUM(4 * x_pos16, crow*y_pos, PCOLED->THD);	
		showNUM(5 * x_pos16, crow*y_pos, PCOLED->HUD);	
		showNUM(6 * x_pos16, crow*y_pos, PCOLED->TEN);	
		showNUM(7 * x_pos16, crow*y_pos, PCOLED->ONE);	

		//row 3 show time title
		crow = 2;
		showCN(2 * x_pos16, crow* y_pos, 10);	
		showCN(3 * x_pos16, crow*y_pos, 4);
		showPicture16x16(4 * x_pos16, crow*y_pos, 0);	//timer picture 
		showSSYM(5 * x_pos16, crow*y_pos, 0);	//:

		//row 4 show time
		crow = 3;
		//day
		showNUM(2 * x_pos8, crow*y_pos, TSOLED->day_h);
		showNUM(3 * x_pos8, crow*y_pos, TSOLED->day_l);
		showSSYM(4 * x_pos8, crow*y_pos, 0); //:
		//hour
		showNUM(5 * x_pos8, crow*y_pos, TSOLED->hour_h);
		showNUM(6 * x_pos8, crow*y_pos, TSOLED->hour_l);
		showSSYM(7 * x_pos8, crow*y_pos, 0); //:
		//minute
		showNUM(8 * x_pos8, crow*y_pos, TSOLED->minute_h);
		showNUM(9 * x_pos8, crow*y_pos, TSOLED->minute_l);
		showSSYM(10 * x_pos8, crow*y_pos, 0); //:
		//second
		showNUM(11 * x_pos8, crow*y_pos, TSOLED->second_h);
		showNUM(12 * x_pos8, crow*y_pos, TSOLED->second_l);
}

void punchOLED::show_watch_page2(bool charge,bool left_right, bool mode, punch_count_OLED *sensitivity){
		uint8_t x_pos16 = 16;
		uint8_t y_pos = 2;
		uint8_t x_pos8 = 8;
		uint8_t crow = 0;
		//row1
		if(charge){
			showSSYM(5 * x_pos8, 0, 2);	//8*16 lighting picture
			showSSYM(7 * x_pos8, 0, 2);	//8*16 lighting picture
			showSSYM(9 * x_pos8, 0, 2);	//8*16 lighting picture
		}

		//row2
		crow = 1;
		if(!left_right){
			//left watch
			showCN(3 * x_pos16, crow* y_pos, 8);	
		} else {
			//right watch
			showCN(3 * x_pos16, crow* y_pos, 9);	
		}		
		showCN(4 * x_pos16, crow* y_pos, 11);	
		
		//row3
		crow = 2;
		if(!mode){
			//pause
			showCN(3 * x_pos16, crow* y_pos, 5);	
			showCN(4 * x_pos16, crow* y_pos, 6);	
		} else {
			//start
			showCN(3 * x_pos16, crow* y_pos, 7);	
			showCN(4 * x_pos16, crow* y_pos, 3);	
		}

		//row3
		//show punch sensitivity (0-100%)
		crow = 3;
		showPicture16x16(1 * x_pos16, crow*y_pos, 1);	//sensitivity picture 
		showSSYM(2 * x_pos16, crow*y_pos, 0);	//:
		showNUM(3 * x_pos16, crow*y_pos, sensitivity->HUD);	
		showNUM(4 * x_pos16, crow*y_pos, sensitivity->TEN);	
		showNUM(5 * x_pos16, crow*y_pos, sensitivity->ONE);	
		showSSYM(6 * x_pos16, crow*y_pos, 1);	//%
}

void punchOLED::show_watch_reset(){
		//show R S T epresses reset all data
		uint8_t x_pos8 = 8;
		uint8_t y_pos = 2;
		uint8_t crow = 2;
		showENC(4 * x_pos8, crow*y_pos, 0);	
		showENC(6 * x_pos8, crow*y_pos, 1);	
		showENC(8 * x_pos8, crow*y_pos, 2);
		showPicture16x16(10 * x_pos8, crow*y_pos, 0);	//timer picture 
}


void punchOLED::clear_screen(){
	fill_screen(0x00, 0x00);  //clear screen
}
