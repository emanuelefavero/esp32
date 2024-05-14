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
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

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
            client.println("");

            // CSS
            client.println("    <style>");
            client.println("      :root {");
            client.println("        --foreground: #020617;");
            client.println("        --background: #f8fafc;");
            client.println("        --primary: #8b5cf6;");
            client.println("      }");
            client.println("");
            client.println("      @media (prefers-color-scheme: dark) {");
            client.println("        :root {");
            client.println("          --foreground: #f8fafc;");
            client.println("          --background: #020617;");
            client.println("        }");
            client.println("      }");
            client.println("");
            client.println("      * {");
            client.println("        margin: 0;");
            client.println("        padding: 0;");
            client.println("        box-sizing: border-box;");
            client.println("      }");
            client.println("");
            client.println("      body {");
            client.println("        margin-top: 4rem;");
            client.println("        font-family: -apple-system, system-ui, BlinkMacSystemFont, 'SF Pro',");
            client.println("          'San Francisco Pro', 'Helvetica Neue', Helvetica, Roboto, sans-serif;");
            client.println("        color: var(--foreground);");
            client.println("        background-color: var(--background);");
            client.println("        display: flex;");
            client.println("        flex-direction: column;");
            client.println("        align-items: center;");
            client.println("        justify-content: center;");
            client.println("      }");
            client.println("");
            client.println("      section {");
            client.println("        padding-top: 2rem;");
            client.println("        display: flex;");
            client.println("        align-items: center;");
            client.println("        justify-content: center;");
            client.println("      }");
            client.println("");
            client.println("      a {");
            client.println("        min-width: 4rem;");
            client.println("        text-decoration: none;");
            client.println("        text-align: center;");
            client.println("        padding: 0.5rem 1rem;");
            client.println("        margin: 0.5rem;");
            client.println("        border: none;");
            client.println("        border-radius: 0.25rem;");
            client.println("        background-color: var(--primary);");
            client.println("        color: #fff;");
            client.println("        font-size: 1rem;");
            client.println("        font-weight: 500;");
            client.println("        text-transform: uppercase;");
            client.println("        cursor: pointer;");
            client.println("        transition: background-color 0.2s, transform 0.2s;");
            client.println("      }");
            client.println("");
            client.println("      a.red {");
            client.println("        background-color: #f43f5e;");
            client.println("      }");
            client.println("");
            client.println("      a.red:hover {");
            client.println("        background-color: #e11d48;");
            client.println("      }");
            client.println("");
            client.println("      a.green {");
            client.println("        background-color: #22c55e;");
            client.println("      }");
            client.println("");
            client.println("      a.green:hover {");
            client.println("        background-color: #16a34a;");
            client.println("      }");
            client.println("");
            client.println("      a.blue {");
            client.println("        background-color: #3b82f6;");
            client.println("      }");
            client.println("");
            client.println("      a.blue:hover {");
            client.println("        background-color: #2563eb;");
            client.println("      }");
            client.println("");
            client.println("      a:active {");
            client.println("        transform: scale(0.95);");
            client.println("      }");
            client.println("");
            client.println("      a.red.off,");
            client.println("      a.green.off,");
            client.println("      a.blue.off {");
            client.println("        background-color: #64748b;");
            client.println("      }");
            client.println("");
            client.println("      a.red.off:hover,");
            client.println("      a.green.off:hover,");
            client.println("      a.blue.off:hover {");
            client.println("        background-color: #475569;");
            client.println("      }");
            client.println("    </style>");
            client.println("  </head>");
            client.println("  <body>");

            // HTML Body
            client.println("    <h1>ESP32 Web Server</h1>"); // title

            // Buttons container
            client.println("    <section>");

            // Red Button
            if (redPinState=="on") {
              client.println("      <a class=\"red\" role=\"button\" aria-label=\"Turn on Red Led\" href=\"/2/off\">" + redPinState + "</a>");
            } else {
              client.println("      <a class=\"red off\" role=\"button\" aria-label=\"Turn on Red Led\" href=\"/2/on\">" + redPinState + "</a>");
            }

            // Green Button
            if (greenPinState=="on") {
              client.println("      <a class=\"green\" role=\"button\" aria-label=\"Turn on Green Led\" href=\"/4/off\">" + greenPinState + "</a>");
            } else {
              client.println("      <a class=\"green off\" role=\"button\" aria-label=\"Turn on Green Led\" href=\"/4/on\">" + greenPinState + "</a>");
            }

            // Blue Button
            if (bluePinState=="on") {
              client.println("      <a class=\"blue\" role=\"button\" aria-label=\"Turn on Blue Led\" href=\"/5/off\">" + bluePinState + "</a>");
            } else {
              client.println("      <a class=\"blue off\" role=\"button\" aria-label=\"Turn on Blue Led\" href=\"/5/on\">" + bluePinState + "</a>");
            }

            client.println("    </section>");

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
