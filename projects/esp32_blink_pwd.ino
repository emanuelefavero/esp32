/*
Blink an LED on pin 2 of the ESP32 using PWM
*/

#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000

#define LEDC_CHANNEL_0 0 // Red channel
#define LEDC_CHANNEL_1 1 // Green channel
#define LEDC_CHANNEL_2 2 // Blue channel

#define RED_PIN 2 // Red on Pin 2
#define GREEN_PIN 4 // Green on Pin 4
#define BLUE_PIN 5 // Blue on Pin 5

int brightness = 0;
int fadeAmount = 5;

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}

void setup() {
  // Setup each channel
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);

  // Attach each channel
  ledcAttachPin(RED_PIN, LEDC_CHANNEL_0);
  ledcAttachPin(GREEB_PIN, LEDC_CHANNEL_1);
  ledcAttachPin(BLUE_PIN, LEDC_CHANNEL_2);
}

void loop() {
  // Apply PWD to each channel
  ledcAnalogWrite(LEDC_CHANNEL_0, brightness); // Red
  ledcAnalogWrite(LEDC_CHANNEL_1, brightness); // Green
  ledcAnalogWrite(LEDC_CHANNEL_2, brightness); // Blue

  // Change brightness over time
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount; // reverse direction at min or max values
  }

  // PWD Speed
  delay(30);
}
