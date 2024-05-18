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

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// * Set your SSID and Password
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// OpenWeatherMap API Key
// * Get your API Key from https://home.openweathermap.org/users/sign_up
String apiKey = "YOUR_API_KEY";
String cityId = "PISA";
String countryCode = "IT";

String serverName = "http://api.openweathermap.org/data/2.5/weather?q=" + String(cityId) + "," + String(countryCode) + "&units=metric&APPID=" + String(apiKey);

// * Set the OLED display width and height in pixels
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  
  // * Initial display message
  display.println("Fetching");
  display.println("weather...");
  display.display();
}

void loop() {
  // Perform the HTTP request every 10 minutes
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

      float temperature = double(response["main"]["temp"]);
      String tempString = String(temperature, 1) + " C";

      // * Display the temperature on the OLED
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println(tempString);
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
  delay(10000);

  // Delay for 10 minutes
  // delay(600000);
}
