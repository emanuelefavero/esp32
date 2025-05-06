#include <SPI.h>
#include <TFT_eSPI.h>


TFT_eSPI tft = TFT_eSPI();

const int WIDTH = 135; // Screen width
const int HEIGHT = 240; // Screen height
const int TEXT_SIZE_MULTIPLIER = 2; // Text size multiplier
const int DEFAULT_CHAR_WIDTH = 6; // Default character width at text size 1
const int DEFAULT_CHAR_HEIGHT = 8; // Default character height at text size 1
const int CHAR_WIDTH = DEFAULT_CHAR_WIDTH * TEXT_SIZE_MULTIPLIER;
const int CHAR_HEIGHT = DEFAULT_CHAR_HEIGHT * TEXT_SIZE_MULTIPLIER;
const char* MESSAGE = "Zzz..."; // Message to print

const int MESSAGE_WIDTH = strlen(MESSAGE) * CHAR_WIDTH;
const int POSITION_X = (WIDTH - MESSAGE_WIDTH) / 2;
const int POSITION_Y = (HEIGHT - CHAR_HEIGHT) / 2;

void setup(void) {
  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true);
  printMessage(MESSAGE); // Print message
  delay(1000);
}

void loop() {}

void printMessage(const char* message) {
  tft.fillScreen(TFT_BLACK); // Clear the screen
  tft.setCursor(POSITION_X, POSITION_Y); // Set cursor position
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(TEXT_SIZE_MULTIPLIER);
  tft.println(message); // Print message
}
