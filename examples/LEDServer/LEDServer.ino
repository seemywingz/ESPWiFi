// NodeMcu Mini Wireless D1 Module: ESP8266 ESP-12F
#include <ESPWiFi.h>

#include "indexHTML.h"

// setup the wifi
String ssid = "ESPWiFiAP";     // your network SSID (name)
String password = "12345678";  // your network password
ESPWiFi espWiFi(ssid, password);

void setup() {
  Serial.begin(115200);     // output to the serial terminal for debugging
  initHandlers();           // initialize the web handlers
  espWiFi.startAsClient();  // start the web server
}

void loop() {
  espWiFi.handleClient();         // Handling of incoming requests
  espWiFi.runMillis(1000, []() {  // print info every second
    Serial.println(espWiFi.infoString());
  });
}

void toggleLED() {  // toggle the LED
  if (digitalRead(LED_BUILTIN) == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
    espWiFi.webserver()->send(200, "text/plain", "LED on");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    espWiFi.webserver()->send(200, "text/plain", "LED off");
  }
  Serial.println("LED toggle");
}

void initHandlers() {  // initialize the handlers
  espWiFi.webserver()->on("/led/toggle", []() { toggleLED(); });
  espWiFi.webserver()->on("/", HTTP_GET, []() {
    espWiFi.webserver()->send(200, "text/html", indexHTML);
  });
  espWiFi.webserver()->on("/led/state", HTTP_GET, []() {
    if (digitalRead(LED_BUILTIN) == HIGH) {
      espWiFi.webserver()->send(200, "text/plain", "LED off");
    } else {
      espWiFi.webserver()->send(200, "text/plain", "LED on");
    }
  });
}
