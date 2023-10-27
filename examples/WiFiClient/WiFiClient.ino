#include <ESPWiFi.h>

String ssid = "ESPWiFiAP";
String password = "12345678";
ESPWiFi espWiFi(ssid, password);

void setup() {
  Serial.begin(115200);
  espWiFi.webserver()->on("/foo", HTTP_GET, []() {
    espWiFi.webserver()->send(200, "text/html",
                              "<html><body><h1>BAR<h1></body></html>");
  });
  espWiFi.startAsClient();
}

void loop() {
  espWiFi.handleClient();
  espWiFi.runMillis(1000, []() { Serial.println(espWiFi.infoString()); });
}
