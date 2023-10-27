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
  Serial.print("IP Address: ");
  Serial.println(espWiFi.infoString());
}