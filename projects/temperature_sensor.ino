// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Testing DHT Temperature sensor"));

  dht.begin(); // Initialize the DHT sensor
}

void loop() {
  delay(2000); // Wait a few seconds between measurements.

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature as Celsius (default)
  float f = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
