

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 19, /* data=*/ 18, /* reset=*/ U8X8_PIN_NONE);    //Software I2C

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_luBIS08_tf);   // choose a suitable font
}
 
void loop() {
  u8g2.clearBuffer();                   // clear the internal memory
  u8g2.drawStr(0,10,"Hello World!");    // write something to the internal memory
  u8g2.sendBuffer();                    // transfer internal memory to the display
  delay(100);  
}



