# ESPWiFi Library

The ESPWiFi library is designed to simplify managing WiFi functionalities on ESP8266 boards. It enables effortless connections to WiFi networks, Access Point (AP) setup, and basic web server operations.

## Features

- Easy connection to WiFi networks as a client
- Access Point (AP) setup for other devices to connect
- Hosting of a basic web server
- Display of WiFi connection and board information
- Advanced configuration with customizable IP settings

## Installation

1. Download the library as a ZIP file.
2. In the Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library`.
3. Select the downloaded ZIP file and click open.
4. The library is now ready for use.

## Usage

Example: Launching an ESPWiFi Access Point

```cpp
#include <ESPWiFi.h>  // include the ESPWiFi library

String ssid = "ESPWiFiAP";     // your network SSID (name)
String password = "12345678";  // your network password
// create an instance of the ESPWiFi class with
// the SSID, password and Custom IP address
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
[Additional Examples](https://github.com/seemywingz/ESPWiFi/tree/main/examples).

## License

The library is released under the MIT License.

## Support

For support and feature requests, [Open a GitHub Issue](https://github.com/seemywingz/ESPWiFi/issues).