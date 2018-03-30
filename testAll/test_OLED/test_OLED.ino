#include <Wire.h>
#include <CN_SSD1306_Wire.h>

//#include "codetab.c"   //codetab

CN_SSD1306_Wire lucky(8);//HardWare I2C

void setup()
{
  lucky.Initial();
   lucky.Fill_Screen(0xff,0xff);
  delay(2000);
  lucky.Fill_Screen(0x00,0x00); 
  delay(10);
}

void loop()
{
  
   show_CN();
  //show_number(); 
  //show_SSYM(); 
    //show_picture1();
    //show_picture2();
    delay(5000);
    // lucky.Fill_Screen(0x00,0x00);
        // delay(10); 
      lucky.Fill_Screen(0xff,0xff);
  delay(2000);
  lucky.Fill_Screen(0x00,0x00); 
 delay(2000);

}

void show_picture1() {
  unsigned char x,y;
  unsigned char count=0;

    for(y = 0; y < 8 ; y++){
      for(x = 0; x < 8 ; x++){
        lucky.ShowPicture1(x*16, y, count); 
        count++;                    
     }
    }
}

void show_number() {
  unsigned char x;
  unsigned char count=0;
  for(x = 0; x < 10; x++){
    lucky.ShowNUM(x*8,0,count);
    count++;
  }   
}
/*
void show_ENC() {
  unsigned char x;
  unsigned char count=0;
  for(x = 0; x < 16; x++){
    lucky.ShowENS(x*8,2,count);
    count++;
  }   
   for(x = 0; x < 11; x++){
    lucky.ShowENS(x*8,4,count);
    count++;
  }   
}*/

void show_SSYM() {
  unsigned char x;
  unsigned char count=0;
  for(x = 0; x < 16; x++){
    lucky.ShowSSYM(x*8,2,count);
    count++;
  }   
   for(x = 0; x < 16; x++){
    lucky.ShowSSYM(x*8,4,count);
    count++;
  }   
}
void show_CN() {
  int x=0,y=0;
  int count=0;
   for(y=0;y<4;y++){
      for(x=0;x<8;x++){      
        lucky.ShowCN(x*16,y*2,count);
        count++;     
      }
   }
}


