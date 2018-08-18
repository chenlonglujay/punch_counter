//For Arduino 1.0 and earlier 
#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else     
#include "WProgram.h"
#endif 

#include <punchCounterReceiver.h>
Segment7_2p3inch_PH s7;
punchCounterReceiver::punchCounterReceiver(void) {
    //default
    volume_button = A0;
    for (int i=49;i>41;i--) {
        ctrl_com_pin[abs(i - 49)] = i;
    }
    dataPin =5;
    latchPin = 6;
    clockPin = 7;
    s7.setPin_7seg(dataPin, latchPin, clockPin);
    punch_total_goal = default_R_L_total;   
    set_goal_L = default_L_MAX;
    set_goal_R = default_R_MAX;
}

punchCounterReceiver::~punchCounterReceiver(void) {

}

void punchCounterReceiver::initial_punchCounterReceiver(uint8_t volume_btn, uint8_t segcom_Low, uint8_t segcom_High
                                                   , uint8_t data_pin, uint8_t latch_pin, uint8_t clock_pin
                                                    , int total_goal) {
    volume_button = volume_btn;
    int i = 0;
    for (i=segcom_High;i>segcom_Low;i--) {
        ctrl_com_pin[abs(i - segcom_High)] = i;
    }

    dataPin = data_pin;
    latchPin = latch_pin;
    clockPin = clock_pin;
    s7.setPin_7seg(dataPin, latchPin, clockPin);
    s7.setPin_com(ctrl_com_pin);
    set_R_L_goal(total_goal);

}

void punchCounterReceiver::set_R_L_goal(int total_goal) {
    if (total_goal <= 0) {
        punch_total_goal = default_R_L_total;
        set_goal_L = default_L_MAX; 
        set_goal_R = default_R_MAX; 
    } else {
        int goal = total_goal/2;
        punch_total_goal = total_goal;
        set_goal_L = goal;
        set_goal_R = goal; 
    }
}

void punchCounterReceiver::show_punch_data(uint8_t val_R, uint8_t val_L, seg_show word_L, seg_show word_R) {
    s7.divide_and_output(val_R, val_L, word_L, word_R);
}
