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

  drawCenteredText("Hello", 1); // TIP: The second argument is the size (1 default)
}

void loop() {}

void drawCenteredText(const char* text, uint8_t textSize) {
  display.clearDisplay();
  display.setTextSize(textSize);              // Set text size
  display.setTextColor(SSD1306_WHITE);        // Set text color to white

  // Calculate the width of the text at the specified scale
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Calculate the x and y positions to center the text
  int16_t x = (SCREEN_WIDTH - w) / 2;
  int16_t y = (SCREEN_HEIGHT - h) / 2;

  // Set cursor to the calculated position
  display.setCursor(x, y);

  // Print the text
  display.print(text);

  // Display the text
  display.display();
}
