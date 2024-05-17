/*
128x32 i2c Oled Display

GND -> GND
VCC -> 3.3v
SCK -> G22
SDA -> G21
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int16_t textX;       // X position of the text
uint16_t textWidth;  // Width of the text

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(2); // Set text size
  display.setTextColor(SSD1306_WHITE); // Set text color to white

  // Calculate the width of the text
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds("Hello", 0, 0, &x1, &y1, &w, &h);
  textWidth = w;

  // Start the text offscreen to the right
  textX = SCREEN_WIDTH;
}

void loop() {
  scrollText("Hello", textX, textWidth, 2);
  delay(20); // Adjust the delay to control the scroll speed
}

// Scroll the text across the screen infinitely towards the left and reset the position when it goes offscreen
void scrollText(const char* text, int16_t& xPos, uint16_t textWidth, uint8_t textSize) {
  display.clearDisplay();
  display.setTextSize(textSize); // Set text size
  display.setTextColor(SSD1306_WHITE); // Set text color to white

  // Calculate the y position to place the text at the bottom
  int16_t yPos = SCREEN_HEIGHT - textSize * 8;

  // Set the cursor to the calculated position
  display.setCursor(xPos, yPos);

  // Print the text
  display.print(text);

  // Display the text
  display.display();

  // Move the text to the left
  xPos--;

  // Reset the text position when it goes offscreen
  if (xPos < -textWidth) {
    xPos = SCREEN_WIDTH;
  }
}
