#include <Arduino.h>

#include "ESPWiFi.h"

// WiFi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Create an instance of ESPWiFi
ESPWiFi espWiFi(ssid, password);

void setup() {
  Serial.begin(115200);
  espWiFi.begin();
}

void loop() {
  espWiFi.handleClient();
  // Your other code here
}
