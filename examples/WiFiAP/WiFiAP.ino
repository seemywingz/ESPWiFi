#include <Arduino.h>
#include <ESPWiFi.h>

#include "serverInfo.h"
ESPWiFi espWiFi(ssid, password, IPAddress(9, 9, 9, 9));

void setup() {
  Serial.begin(115200);
  espWiFi.startAsAccessPoint();
}

void loop() {
  espWiFi.handleClient();
  Serial.print("IP Address: ");
  Serial.println(espWiFi.infoString());
}
