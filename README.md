# ESPWiFi

The ESPWiFi library is designed to simplify managing WiFi functionalities on ESP8266 boards. It enables effortless connections to WiFi networks, Access Point (AP) setup, and basic web server operations.

## Features

- Easy connection to WiFi networks as a client
- Access Point (AP) setup for other devices to connect
- Hosting of a basic web server
- Display of WiFi connection and board information
- Advanced configuration with customizable IP settings

## Installation

### Via GitHub (Recommended)

1. Navigate to the [ESPWiFi GitHub repository](https://github.com/seemywingz/ESPWiFi).
2. Click on the "Code" button and select "Download ZIP".
3. In the Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library`.
4. Choose the downloaded ZIP file and click open.

### Manual Installation

Alternatively, you can clone the repository directly:

1. Open a terminal or command prompt.
2. Change to your Arduino libraries directory.
3. Run the git clone command:
git clone https://github.com/seemywingz/ESPWiFi.git


## Usage

Example: Launching an ESPWiFi Access Point

```cpp
#include <ESPWiFi.h>  // include the ESPWiFi library

String ssid = "ESPWiFiAP";     // your network SSID (name)
String password = "12345678";  // your network password
// create an instance of the ESPWiFi class with
// the SSID, password, and Custom IP address
ESPWiFi espWiFi(ssid, password, IPAddress(9, 9, 9, 9));

void setup() {
Serial.begin(115200);          // output to the serial terminal for debugging
espWiFi.startAsAccessPoint();  // start the web server
}

void loop() {
espWiFi.handleClient();         // Handle incoming requests
espWiFi.runMillis(1000, []() {  // print info every second
 Serial.println(espWiFi.infoString());
});
}
```
Additional Examples.

License
The library is released under the MIT License.

Support
For support and feature requests, Open a GitHub Issue.





