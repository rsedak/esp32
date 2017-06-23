/*
 WiFi Access Point Web Server NeoPixel 8x8 grid

 A simple web server that lets you to turn on and off NeoPixel array via the web.
 You can open address 10.10.10.1 in a web browser
 to turn on and off NeoPixel connected on pin 5.

 http://10.10.10.1/ON turns the LED on
 http://10.10.10.1/OFF turns it off

 This example is written for a network using WPA encryption.
 
 */

#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the ESP32 is connected to the NeoPixels?
#define PIN          5

// How many NeoPixels LEDs are attached to the ESP32?
#define NUMPIXELS   64

// We define birghtness of NeoPixel LEDs
#define BRIGHTNESS  20

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Set SSID
const char* ssid     = "esp32";

// Set password
const char* password = "12345678";

// Set Access Point IP
IPAddress   apIP(10, 10, 10, 1);

// Set web server to listen port 80
WiFiServer server(80);

void setup()
{
    // Set Access Point configuration
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00
    WiFi.softAP(ssid,password);

    // Set NeoPixel configuration 
    matrix.setBrightness(BRIGHTNESS);

    // Start NeoPixel library with all LEDs off
    matrix.begin();

    // Show settings of LEDs in NeoPixel array
    matrix.show();

    // Start web server
    server.begin();

}

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<html><body>");
            client.print("Click <a href=\"/ON\">here</a> to turn the LED MATRIX on pin 5 on.<br>");
            client.print("Click <a href=\"/OFF\">here</a> to turn the LED MATRIX on pin 5 off.<br>");
            client.print("</body></html>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /ON" or "GET /OFF":
        if (currentLine.endsWith("GET /ON")) {
          colorWipe(matrix.Color(0, 255, 0), 40);
        }
        if (currentLine.endsWith("GET /OFF")) {
          colorWipe(matrix.Color(0, 0, 0), 40);
        }
      }
    }
    // close the connection:
    client.stop();
  }
}

// Set color for LEDs in array with delay between setting each LED
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<matrix.numPixels(); i++) {
    matrix.setPixelColor(i, c);
    matrix.show();
    delay(wait);
  }
}
