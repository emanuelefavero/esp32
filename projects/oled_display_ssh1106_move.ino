/*
ESP32 - Print a moving "Hello World" in a ssh1106 128x64 oled display display

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

// Initial Y position of the text
const int initialPosition = 10;
int yPos = initialPosition;
unsigned long previousMillis = 0; // Store the last time the text was updated
const long interval = 1000; // Interval at which to update the text (milliseconds)

void setup() {
  // Initialize the display
  u8g2.begin();
  u8g2.enableUTF8Print(); // Enable UTF8 support for the Arduino print() function
  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font
}

void loop() {
  unsigned long currentMillis = millis(); // Get the current time

  // Check if it's time to update the text position
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the current time

    // Clear the buffer
    u8g2.clearBuffer();
    
    // Draw "Hello, world!" at the current Y position
    u8g2.drawStr(0, yPos, "Hello, world!");
    
    // Send the buffer to the display
    u8g2.sendBuffer();
    
    // Update the Y position
    yPos += 10; // Move the text down by 10 pixels

    // Reset to the top if the text reaches the bottom
    if (yPos > u8g2.getHeight()) {
      yPos = initialPosition; // Reset to the top
    }
  }
}
