/*
Web Server - Control RGB Led on GPIO pins 2, 4, 5 from a web server by typing the ESP32 IP address on a browser

// TIP: Make sure to add your ssid and password credentials of your local network.
// TIP: After uploading the code to the ESP32 go to the Serial Monitor in the Arduino IDE to check the ESP32 IP address.
Then, go the IP address in your browser search bar
*/

// Load Wi-Fi library
#include <WiFi.h>

// Setup WiFi Network
// TIP: Replace ssid and password with your credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Set web server port number
WiFiServer server(80);

// Store HTTP request
String header;

// Auxiliar variables to store the current output state
String redPinState = "off";
String greenPinState = "off";
String bluePinState = "off";

// Assign variables to GPIO pins;
const int redPin = 2;
const int greenPin = 4;
const int bluePin = 5;

// Time
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Initialize led pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Set outputs to low
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected. ");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // listen for incoming clients

  // If a new client connects
  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = ""; // make a String to hold incoming data from the client

    // Loop while the client is connected
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Turn the GPIOs on an off
            // Red on
            if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              redPinState = "on";
              digitalWrite(redPin, HIGH);

            // Red off
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              redPinState = "off";
              digitalWrite(redPin, LOW);
            }

            // Green on
            else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              greenPinState = "on";
              digitalWrite(greenPin, HIGH);

            // Green off
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              greenPinState = "off";
              digitalWrite(greenPin, LOW);
            }

            // Blue on
            else if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              bluePinState = "on";
              digitalWrite(bluePin, HIGH);

            // Blue off
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              bluePinState = "off";
              digitalWrite(bluePin, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\">");
            client.println("  <head>");
            client.println("    <meta charset=\"UTF-8\" />");
            client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />");
            client.println("    <title>ESP32 Web Server</title>"); // head title
            client.println("  </head>");
            client.println("  <body>");

            // Title
            client.println("    <h1>ESP32 Web Server</h1>");

            // Red State
            client.println("<p>Red: " + redPinState + "</p>");
            // Red ON/OFF buttons
            if (redPinState=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button buttonOff\">OFF</button></a></p>");
            }

            // Green State
            client.println("<p>Green: " + greenPinState + "</p>");
            // Green ON/OFF buttons
            if (greenPinState=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button buttonOff\">OFF</button></a></p>");
            }

            // Blue State
            client.println("<p>Blue: " + bluePinState + "</p>");
            // Blue ON/OFF buttons
            if (bluePinState=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button buttonOff\">OFF</button></a></p>");
            }

            // Close HTML file
            client.println("  </body>");
            client.println("</html>");

            // Close the HTTP response
            client.println();
            break;

            // If there is a newline, clear currentLine 
          } else {
            currentLine = "";
          }

          // If you get anything but a carriage return character...
        } else if (c != '\r') {
          currentLine += c; // ...add it to the end of the currentLine
        }
      }
    }

    // Close the connection
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
