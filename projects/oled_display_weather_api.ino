/*
128x32 i2c Oled Display

GND -> GND
VCC -> 3.3v
SCK -> G22
SDA -> G21
*/

// #include <stdlib.h> // For atof()
// #include <math.h>   // For round()

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <moonPhase.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// OpenWeatherMap API Key
String apiKey = "YOUR_API_KEY";
String cityId = "Pisa";
String countryCode = "IT";

String serverName = "http://api.openweathermap.org/data/2.5/weather?q=" + String(cityId) + "," + String(countryCode) + "&units=metric&APPID=" + String(apiKey);

moonPhase moonPhase; // include a MoonPhase instance
struct tm timeinfo = {0}; // setup time

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DEFINE BITMAP IMAGES
// Moon
const unsigned char PROGMEM bitmapMoonFirstQuarter[] = {
  0x00, 0x08, 0x0c, 0x0e, 0x0e, 0x0e, 0x0c, 0x08
}; // 8x8 moon first quarter

const unsigned char PROGMEM bitmapMoonFull[] = {
  0x00, 0x38, 0x7c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38
}; // 8x8 moon full

const unsigned char PROGMEM bitmapMoonNew[] = {
  0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x44, 0x38
}; // 8x8 moon new

const unsigned char PROGMEM bitmapMoonThirdQuarter[] = {
  0x00, 0x20, 0x60, 0xe0, 0xe0, 0xe0, 0x60, 0x20
}; // 8x8 moon third quarter

const unsigned char PROGMEM bitmapMoonWaningCrescent[] = {
  0x00, 0x00, 0x40, 0xc0, 0xc0, 0xe0, 0x7c, 0x38
}; // 8x8 moon waning crescent

const unsigned char PROGMEM bitmapMoonWaningGibbous[] = {
  0x00, 0x38, 0x7c, 0xf8, 0xf8, 0xf0, 0x40, 0x00
}; // 8x8 moon waning gibbous

const unsigned char PROGMEM bitmapMoonWaxingCrescent[] = {
  0x00, 0x00, 0x04, 0x06, 0x06, 0x0e, 0x7c, 0x38
}; // 8x8 moon waxing crescent

const unsigned char PROGMEM bitmapMoonWaxingGibbous[] = {
  0x00, 0x18, 0x3c, 0x3e, 0x3e, 0x3e, 0x3c, 0x18
}; // 8x8 moon waxing gibbous

// Weather
const unsigned char PROGMEM bitmapHumidity[] = {
	0x00, 0x18, 0x18, 0x3c, 0x7e, 0x7e, 0x3c, 0x18
}; // 8x8 humidity

const unsigned char PROGMEM bitmapWind[] = {
	0x18, 0x73, 0xc6, 0x1c, 0x70, 0xc3, 0x0e, 0x18
}; // 8x8 wind

const unsigned char PROGMEM bitmapSun[] = {
	0x00, 0x00, 0x01, 0x80, 0x09, 0x90, 0x04, 0x20, 0x21, 0x84, 0x13, 0xc8, 0x04, 0x20, 0x6c, 0x36, 
	0x6c, 0x36, 0x04, 0x20, 0x13, 0xc8, 0x21, 0x84, 0x04, 0x20, 0x09, 0xb0, 0x01, 0x80, 0x00, 0x00
}; // 16x16 sun

const unsigned char PROGMEM bitmapCloud[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x1e, 0x60, 0x38, 0xf6, 0x33, 0xf7, 0x07, 0xf3, 
  0x1f, 0xf8, 0x3f, 0xfc, 0x1f, 0xf8, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // 16x16 cloud

const unsigned char PROGMEM bitmapThunderstorm[] = {
	0x03, 0xc0, 0x07, 0xe0, 0x1f, 0xf8, 0x3f, 0xfc, 0x7f, 0xfe, 0xfc, 0x1f, 0xfd, 0x9f, 0xfb, 0xbf,
  0x73, 0x0e, 0x07, 0xe0, 0x07, 0xe0, 0x01, 0xc0, 0x03, 0x80, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00
}; // 16x16 thunderstorm

const unsigned char PROGMEM bitmapRain[] = {
	0x00, 0x04, 0x10, 0x0c, 0x30, 0x0e, 0x38, 0x1e, 0x7c, 0x0c, 0x7c, 0xc0, 0x39, 0xc0, 0x11, 0xe0,
  0x03, 0xf0, 0x07, 0xf0, 0x07, 0xf8, 0x0f, 0xf8, 0x0f, 0xf8, 0x07, 0xf8, 0x03, 0xf0, 0x01, 0xe0
}; // 16x16 rain

const unsigned char PROGMEM bitmapSnow[] = {
	0x00, 0x00, 0x01, 0x80, 0x03, 0xc0, 0x19, 0x98, 0x79, 0x9e, 0x39, 0x9c, 0x3d, 0xbc, 0x03, 0xc0,
  0x03, 0xc0, 0x3d, 0xbc, 0x39, 0x9c, 0x79, 0x9e, 0x19, 0x98, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00
}; // 16x16 snow

const unsigned char PROGMEM bitmapMist[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x3f, 0xf8,
  0x3f, 0xf8, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; // 16x16 mist

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Get Time through WiFi
  Serial.println("Connected. Getting time...");
  configTzTime( "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "0.pool.ntp.org" ); // Timezone: Rome, Italy
  while (!getLocalTime( &timeinfo, 0)) {
    vTaskDelay( 10 / portTICK_PERIOD_MS );
  }
  
  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
  
  // Initial display message
  display.println("Fetching");
  display.println("weather...");
  display.display();
}

void loop() {
  // Perform the HTTP request
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    http.begin(serverName);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);

      JSONVar response = JSON.parse(payload);

      if (JSON.typeof(response) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      // Display data
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);

      // TEMPERATURE
      float tempMin = double(response["main"]["temp_min"]);
      int tempMinInt = round(tempMin);
      display.print(String(tempMinInt));

      display.setTextSize(2);
      float temperature = double(response["main"]["temp"]);
      int temperatureInt = round(temperature);
      display.print(String(temperatureInt) + "C");

      display.setTextSize(1);
      float tempMax = double(response["main"]["temp_max"]);
      int tempMaxInt = round(tempMax);
      display.print(String(tempMaxInt));
      display.print(" ");

      // CITY NAME (same line as temperature)
      // const char* cityName = response["name"];
      // display.print(cityName);

      // MOON PHASE
      getLocalTime(&timeinfo); // get local time through WiFi
      Serial.print("Time: ");
      Serial.print(asctime(&timeinfo));

      // Moon phase (angle 0-360)
      moonData_t moon = moonPhase.getPhase(); // get moon phase
      int moonAngle = moon.angle; // get phase 0-360
      int moonPercentage = round(double(moon.percentLit * 100));

      // Determine the moon phase based on the moon angle
      if (moonAngle >= 360 && moonAngle < 45) {
          display.print("NM");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonNew, 8, 8); // new moon
      } else if (moonAngle >= 45 && moonAngle < 90) {
          display.print("WxC");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonWaxingCrescent, 8, 8); // waxing crescent
      } else if (moonAngle >= 90 && moonAngle < 135) {
          display.print("FQ");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonFirstQuarter, 8, 8); // first quarter
      } else if (moonAngle >= 135 && moonAngle < 180) {
          display.print("WxG");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonWaxingGibbous, 8, 8); // waxing gibbous
      } else if (moonAngle >= 180 && moonAngle < 225) {
          display.print("Full");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonFull, 8, 8); // full moon
      } else if (moonAngle >= 225 && moonAngle < 270) {
          display.print("WnG");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonWaningGibbous, 8, 8); // waning gibbous
      } else if (moonAngle >= 270 && moonAngle < 315) {
          display.print("TQ");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonThirdQuarter, 8, 8); // third quarter
      } else if (moonAngle >= 315 && moonAngle < 360) {
          display.print("WnC");
          display.print(moonPercentage);
          display.print("% ");
          drawBitmap(bitmapMoonWaningCrescent, 8, 8); // waning crescent
      }

      // Move to the next line for Wind text
      int textSize = 2;
      int baseFontHeight = 8; // Base height of the font in pixels
      int textHeight = textSize * baseFontHeight;
      display.setCursor(0, textHeight); // Position cursor to next line based on the 2x text height

      // WIND
      textSize = 1; // // Ensure text size is set to normal
      display.setTextSize(textSize);
      drawBitmap(bitmapWind, 8, 8);
      display.print(" ");
      display.print(int(round(double(response["wind"]["speed"]) * 3.6)));
      display.print("km/h");
      display.print(" ");

      // HUMIDITY
      drawBitmap(bitmapHumidity, 8, 8);
      display.print(" ");
      display.print(response["main"]["humidity"]);
      display.print("%");

      display.print("   ");

      // WEATHER ICON
      const char* weatherMain = response["weather"][0]["main"];

      // Display a different bitmap image depending on weather description
      if (strcmp(weatherMain, "Clear") == 0) {
        drawBitmap(bitmapSun, 16, 16);
      } else if (strcmp(weatherMain, "Clouds") == 0) {
        drawBitmap(bitmapCloud, 16, 16);
      } else if (strcmp(weatherMain, "Thunderstorm") == 0) {
        drawBitmap(bitmapThunderstorm, 16, 16);
      } else if (strcmp(weatherMain, "Drizzle") == 0) {
        drawBitmap(bitmapRain, 16, 16);
      } else if (strcmp(weatherMain, "Rain") == 0) {
        drawBitmap(bitmapRain, 16, 16);
      } else if (strcmp(weatherMain, "Snow") == 0) {
        drawBitmap(bitmapSnow, 16, 16);
      } else if (strcmp(weatherMain, "Atmosphere") == 0) {
        drawBitmap(bitmapMist, 16, 16);
      }
      
      display.println(""); // go to next line

      // WEATHER DESCRIPTION
      // Capitalize and display weather description
      const char* weatherDescription = response["weather"][0]["description"];
      char capitalizedDescription[strlen(weatherDescription) + 1]; // Create a new array to store the modified string
      strcpy(capitalizedDescription, weatherDescription); // Copy the original string to the new array
      capitalizedDescription[0] = toupper(capitalizedDescription[0]); // Capitalize the first letter
      display.println(capitalizedDescription); // Print the modified weather description

      // Display everything
      display.display();
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  // Delay for 10 seconds
  // delay(10000);

  // Delay for 10 minutes
  delay(600000);
}

void drawBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height) {
  // Save current cursor position
  int16_t cursorX = display.getCursorX();
  int16_t cursorY = display.getCursorY();

  // Draw bitmap slightly higher to align with text
  int16_t adjustedY = cursorY - 1; // Adjust for better Y alignment
  display.drawBitmap(cursorX, adjustedY, bitmap, width, height, SSD1306_WHITE);

  // Move cursor position to the right of the bitmap
  display.setCursor(cursorX + width, cursorY);
}
