#include <ADXL345.h>
ADXL345 testADXL345;
void setup() {
     Serial.begin(9600);
     testADXL345.ADXL345_initial_set(SDO_HIGH, c2g);    //c2g c4g c8g c16g
     delay(5);
     Serial.println("start");
}

void loop() {
  
      testADXL345.show_xAxis_org();
      testADXL345.show_xAxis_gravity();
      
      testADXL345.show_yAxis_org();
      testADXL345.show_yAxis_gravity();
      
      testADXL345.show_zAxis_org();
      testADXL345.show_zAxis_gravity();
      
      testADXL345.show_xyzAxis_org();
      testADXL345.show_xyzAxis_gravity();
}


