#ifndef ESPWIFI_WIFI_H
#define ESPWIFI_WIFI_H

#include "ESPWiFi.h"

void ESPWiFi::connectToWifi() {
  String ssid = config["client"]["ssid"];
  String password = config["client"]["password"];
  WiFi.begin(ssid, password);
  Serial.println("\n\nConnecting to:");
  Serial.println("\tSSID: " + ssid);
  Serial.println("\tPassword: " + password);
  int connectionAttempts = maxConnectAttempts;
  while (WiFi.status() != WL_CONNECTED) {
    if (--connectionAttempts == 0) {
      Serial.println("Failed to connect to WiFi");
      config["mode"] = "ap";
      startAP();
      return;
    }
    if (connectSubroutine != nullptr) {
      connectSubroutine();
    }
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("\tWiFi connected");
  Serial.print("\tIP Address: ");
  Serial.println(WiFi.localIP());
}

int ESPWiFi::selectBestChannel() {
  int channels[14] = {
      0};  // Array to hold channel usage counts, 14 for 2.4 GHz band
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; i++) {
    int channel = WiFi.channel(i);
    if (channel > 0 &&
        channel <= 13) {  // Ensure the channel is within a valid range
      channels[channel]++;
    }
  }
  int leastCongestedChannel = 1;  // Default to channel 1
  for (int i = 1; i <= 13; i++) {
    if (channels[i] < channels[leastCongestedChannel]) {
      leastCongestedChannel = i;
    }
  }
  return leastCongestedChannel;
}

void ESPWiFi::startAP() {
  String ssid = config["ap"]["ssid"];
  String password = config["ap"]["password"];
  int bestChannel = selectBestChannel();
  WiFi.softAP(ssid, password, bestChannel);
  Serial.println("\n\nStarting Access Point:");
  Serial.println("\tSSID: " + ssid);
  Serial.println("\tPassword: " + password);
  Serial.print("\tIP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("\tChannel: ");
  Serial.println(bestChannel);
}

void ESPWiFi::handleClient() {
  webServer.handleClient();
#ifdef ESP8266
  MDNS.update();
#endif
}

#endif  // ESPWIFI_WIFI_H