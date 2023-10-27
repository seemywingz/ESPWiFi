# ESPWiFi Library

This is a versatile library for managing WiFi functionalities on ESP8266 boards. It simplifies the process of connecting to WiFi networks, starting an Access Point (AP), and hosting a basic web server.

## Features

- Connect to a WiFi network as a client
- Create an Access Point (AP) for other devices to connect to
- Host a basic web server with informative pages
- Display WiFi connection and board information
- Customizable IP settings for advanced users

## Installation

1. Download the library as a ZIP file.
2. In the Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library`.
3. Choose the downloaded ZIP file and click open.
4. The library should now be installed.

## Usage

### Initialization

To use the library, first include it in your sketch:

```cpp
#include <ESPWiFi.h>
```

Create an instance of the ESPWiFi class:

```cpp
ESPWiFi wifi("Your_SSID", "Your_Password");
```

Optionally, you can specify custom IP, Gateway, and Subnet:

```cpp
IPAddress customIP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
ESPWiFi wifi("Your_SSID", "Your_Password", customIP, gateway, subnet);
```

### Connecting to a WiFi Network

To connect to a WiFi network as a client:

```cpp
wifi.startAsClient();
```

### Starting as an Access Point

To start the ESP8266 as an Access Point:

```cpp
wifi.startAsAccessPoint();
```

### Running the Web Server

To handle client requests on the web server:

```cpp
wifi.handleClient();
```

### Additional Methods

- `wifi.isAccessPoint()`: Check if the device is running as an Access Point.
- `wifi.localIP()`: Get the local IP address.
- `wifi.infoString()`: Get a string containing WiFi information.
- `wifi.checkWiFi()`: Check the WiFi connection status.

## Examples
Example of starting an ESPWiFi Access Point
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
Link to [Additional Examples](https://github.com/seemywingz/ESPWiFi/tree/main/examples).

## License

This library is open-sourced and licensed under the MIT License.

## Support

For issues and feature requests, please [Open a GitHub Issue](https://github.com/seemywingz/ESPWiFi/issues).