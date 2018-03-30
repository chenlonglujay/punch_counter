//blue tooth(HC05) module test
//Master:Arduino pro mini
#include <SoftwareSerial.h>
SoftwareSerial BT(10,11); //arduino RX(white wire) ,TX(orange wire)
#define AT_Mode 0
#define transmit_Mode 1
char val;

void setup() {
  Serial.begin(9600);
#if AT_Mode
  Serial.println("enter AT commands");
#endif

#if transmit_Mode
 Serial.println("transmit_Mode");
#endif

  BT.begin(38400);
}

void loop() {
 #if AT_Mode  
    AT_Mode_function();
 #else if transmit_Mode
   transmit_Mode_function();
 #endif
}

void AT_Mode_function(){
  if (Serial.available()) { 
      val = Serial.read();
      BT.print(val); 
  }
  if (BT.available()) {
    val = BT.read();
    Serial.print(val);
  } 
}

void transmit_Mode_function(){
  BT.print("w"); 
  delay(1000);
  /*
  if (BT.available()) {
    val = BT.read();
    Serial.print(val);
  } */
}

