
#include "src/ESPWiFi.h"

// Web Server
ESPWiFi wifi;

bool runOnce = false;

void setup() {
  wifi.start();
  wifi.startAudio();
}

void loop() {
  wifi.handleClient();
  wifi.handleAudio();
  if (!runOnce) {
    runOnce = true;
    String response = wifi.openAIChat("Hello, world!");
    wifi.openAI_TTS(response, "/hello.mp3");
    wifi.playMP3("/hello.mp3");
  }
}
