/*
ESP32 - Print "Hello World" in a ssh1106 128x64 oled display display

VDD to 3.3v
GND to GND
SCK to G22
SDA to G21

TIP: Make sure you have the U8g2 library installed in Arduino IDE
*/

#include <U8g2lib.h>
#include <Wire.h>

// Constructor for SH1106 128x64 I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  // Initialize the display
  u8g2.begin();
  u8g2.enableUTF8Print(); // Enable UTF8 support for the Arduino print() function

  // Clear the buffer
  u8g2.clearBuffer();
  
  // Set the font
  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font

  // Draw "Hello, world!" on the display
  u8g2.drawStr(0, 24, "Hello, world!");

  // Send the buffer to the display
  u8g2.sendBuffer();
}

void loop() {
  // Nothing to do here
}
