
#include "src/ESPWiFi.h"

// Web Server
ESPWiFi wifi;

void setup() {
  wifi.start();
  wifi.enableGPIO();
  // wifi.startAudio();
  // String response = wifi.openAI_Chat("Hello, world!");
  // wifi.openAI_TTS(response, "/hello.mp3");
  // wifi.playMP3("/hello.mp3");
}

void loop() {
  wifi.handleClient();
  // wifi.handleAudio();
}
