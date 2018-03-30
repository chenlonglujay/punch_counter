//blue tooth(HC05) module test
//1.master BT key pin => 3.3v
//2.push master BT module key bttton down and power(3.3v) on
//3.slave BT power on
//4.master Arduino open Serial Monitor to get slave BT address
//Master:Arduino Mega 2560
//#include <SoftwareSerial.h>
//hardware serial
//RX1 19
//TX1 18
#define AT_Mode 1
#define receive_Mode 0
char val;

void setup() {
  Serial.begin(9600);
  
#if AT_Mode
  Serial.println("enter AT commands");
#else if  receive_Mode
 Serial.println("receive_Mode");
#endif
  Serial1.begin(38400);
  
}

void loop() {
 #if AT_Mode  
    AT_Mode_function();
 #else if receive_Mode
   receive_Mode_function();
 #endif
}

void receive_Mode_function(){
   if (Serial1.available()) {
         val = Serial1.read();
        Serial.print("receive val: ");
       Serial.println(val);
          if (val == 'w'){
              Serial.println("it is w");
          }
          Serial.print("--------\n");
      }         
}

void AT_Mode_function(){
    //若收到序列埠監控視窗的資料,則送到藍牙模組
      //Receives from serial monitor , sends to bluetooth module 
      if (Serial.available()) { 
          val = Serial.read();
          Serial1.print(val);  
      }

      //若收到藍牙模組的資料,則送到序列埠監控視窗
    //Receives from bluetooth module,sends to serial monitor
      if (Serial1.available()) {
         val = Serial1.read();
        Serial.print(val);
      } 
}


