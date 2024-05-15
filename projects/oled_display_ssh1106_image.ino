/*
ESP32 - Print an image in a ssh1106 128x64 oled display display

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

// Bitmap of a smiley face (16x16 pixels)
const unsigned char bitmap_smiley[] U8X8_PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00111111, 0b11111000,
  0b01000000, 0b00000100,
  0b10000000, 0b00000010,
  0b10000000, 0b00000010,
  0b10000000, 0b00000010,
  0b10000000, 0b00000010,
  0b10000000, 0b00000010,
  0b01000000, 0b00000100,
  0b00100000, 0b00001000,
  0b00010000, 0b00010000,
  0b00001111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

int yPos = 0; // Initial Y position of the bitmap
unsigned long previousMillis = 0; // Store the last time the bitmap was updated
const long interval = 1000; // Interval at which to update the bitmap (milliseconds)

void setup() {
  // Initialize the display
  u8g2.begin();
}

void loop() {
  unsigned long currentMillis = millis(); // Get the current time

  // Check if it's time to update the bitmap position
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the current time

    // Clear the buffer
    u8g2.clearBuffer();
    
    // Draw the bitmap at the current Y position
    u8g2.drawXBMP(0, yPos, 16, 16, bitmap_smiley);
    
    // Send the buffer to the display
    u8g2.sendBuffer();
    
    // Update the Y position
    yPos += 10; // Move the bitmap down by 10 pixels

    // Reset to the top if the bitmap reaches the bottom
    if (yPos > u8g2.getHeight()) {
      yPos = 0; // Reset to the top
    }
  }
}
