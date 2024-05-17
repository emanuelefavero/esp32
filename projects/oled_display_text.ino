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

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  drawChar();      // Draw characters of the default font

  // display.clearDisplay();
  // delay(1000);

  // Invert and restore display, pausing in-between
  // display.invertDisplay(true);
  // delay(1000);
  // display.invertDisplay(false);
  // delay(1000);
}

void drawChar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Write "Hello" to the display
  display.print("Hello ");

  // Write smiley face and heart symbols using their Code Page 437 codes
  display.write(0x01);  // Smiley face
  display.write(0x03);  // Heart

  // Draw 'inverse' text (black with white background)
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  // Write line of text (it adds a \n at the end)
  // It also stores the string literal in PROGREM instead of RAM to save RAM
  display.println(F(" Hello"));

  // Draw 2X-scale text
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print(F("Hello"));

  display.display();
  delay(2000);
}
