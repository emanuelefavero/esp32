/*
Blink a Cathode RGB LED on pins 2, 4, and 5 of the ESP32 using PWM and a switch on pin 15

TIP: Connect the other part of the switch to the GND pin of the ESP32
*/

#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000

#define LEDC_CHANNEL_0 0 // Red channel
#define LEDC_CHANNEL_1 1 // Green channel
#define LEDC_CHANNEL_2 2 // Blue channel

#define RED_PIN 2 // Red on Pin 2
#define GREEN_PIN 4 // Green on Pin 4
#define BLUE_PIN 5 // Blue on Pin 5

#define SWITCH_PIN 15 // Switch on Pin 15

int brightnessRedMax = 240;
int brightnessGreenMax = 200;
int brightnessBlueMax = 255;

// TIP: We assign different variables and set different values to each led to offset the pwd phase of each led
int brightnessRed = 80;
int brightnessGreen = 30;
int brightnessBlue = 180;

// TIP: Increase the value of fadeAmount to increase the pwd speed
int fadeAmountRed = 1;
int fadeAmountGreen = 3;
int fadeAmountBlue = 4;

bool ledState = false;
bool lastSwitchState = HIGH;

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}

void setup() {
  // Setup switch pin as input with pull-up resistor
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // Setup each led channel
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);

  // Attach each led channel
  ledcAttachPin(RED_PIN, LEDC_CHANNEL_0);
  ledcAttachPin(GREEN_PIN, LEDC_CHANNEL_1);
  ledcAttachPin(BLUE_PIN, LEDC_CHANNEL_2);
}

void loop() {
  bool currentSwitchState = digitalRead(SWITCH_PIN);

  // Check if switch state has changed from high to low (button press)
  if (lastSwitchState == HIGH && currentSwitchState == LOW) {
    // Toggle the LED state
    ledState = !ledState;
  }
  // Store the current switch state as the last switch state for the next loop iteration
  lastSwitchState = currentSwitchState;

  // Use the ledState to turn LEDs on or off
  if (ledState) {
    ledcAnalogWrite(LEDC_CHANNEL_0, brightnessRed, brightnessRedMax); // Red
    ledcAnalogWrite(LEDC_CHANNEL_1, brightnessGreen, brightnessGreenMax); // Green
    ledcAnalogWrite(LEDC_CHANNEL_2, brightnessBlue, brightnessBlueMax); // Blue

    // Change brightness over time
    brightnessRed += fadeAmountRed;
    if (brightnessRed <= 0 || brightnessRed >= brightnessRedMax) {
      fadeAmountRed = -fadeAmountRed; // Reverse the direction for Red
    }
    brightnessGreen += fadeAmountGreen;
    if (brightnessGreen <= 0 || brightnessGreen >= brightnessGreenMax) {
      fadeAmountGreen = -fadeAmountGreen; // Reverse the direction for Green
    }
    brightnessBlue += fadeAmountBlue;
    if (brightnessBlue <= 0 || brightnessBlue >= brightnessBlueMax) {
      fadeAmountBlue = -fadeAmountBlue; // Reverse the direction for Blue
    }
  } else {
    ledcAnalogWrite(LEDC_CHANNEL_0, 0, brightnessRedMax); // Red
    ledcAnalogWrite(LEDC_CHANNEL_1, 0, brightnessGreenMax); // Green
    ledcAnalogWrite(LEDC_CHANNEL_2, 0, brightnessBlueMax); // Blue
  }

  delay(125); // PWD Speed
}
