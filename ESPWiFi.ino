
#include "src/ESPWiFi.h"

// Web Server
ESPWiFi wifi;

bool runOnce = false;

void setup() { wifi.start(); }

void loop() {
  wifi.handleClient();
  if (!runOnce) {
    runOnce = true;
    String response = wifi.openAIChat("Hello, world!");
    wifi.openAI_TTS(response, "/hello.mp3");
  }
}
