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

### Initialization

Include the library in your sketch:

```cpp
#include <ESPWiFi.h>
```

Create an instance of the ESPWiFi class:

```cpp
ESPWiFi espWiFi("Your_SSID", "Your_Password");
```

For custom IP configuration:

```cpp
IPAddress customIP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
ESPWiFi espWiFi("Your_SSID", "Your_Password", customIP, gateway, subnet);
```

### Connecting as a Client

To connect to a WiFi network:

```cpp
espWiFi.startAsClient();
```

### Starting as an Access Point

To set up the ESP8266 as an Access Point:

```cpp
espWiFi.startAsAccessPoint();
```

### Web Server Operations

To manage client requests on the web server:

```cpp
espWiFi.handleClient();
```

### Additional Methods

- `espWiFi.isAccessPoint()`: Verify if the device is running as an Access Point.
- `espWiFi.localIP()`: Obtain the local IP address.
- `espWiFi.infoString()`: Get a string with WiFi information.
- `espWiFi.checkWiFi()`: Check the status of the WiFi connection.

## Examples

Example: Launching an ESPWiFi Access Point

```cpp
#include <ESPWiFi.h>

String ssid = "ESPWiFiAP";
String password = "12345678";
ESPWiFi espWiFi(ssid, password, IPAddress(9, 9, 9, 9));

void setup() {
  Serial.begin(115200);
  espWiFi.startAsAccessPoint();
}

void loop() {
  espWiFi.handleClient();
  espWiFi.runMillis(1000, []() {
    Serial.println(espWiFi.infoString());
  });
}
```
For more examples, visit [Additional Examples](https://github.com/seemywingz/ESPWiFi/tree/main/examples).

## License

The library is released under the MIT License.

## Support

For support and feature requests, [Open a GitHub Issue](https://github.com/seemywingz/ESPWiFi/issues).