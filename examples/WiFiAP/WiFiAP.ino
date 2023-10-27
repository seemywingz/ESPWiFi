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