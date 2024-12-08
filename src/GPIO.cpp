#ifndef ESPWIFI_GPIO_H
#define ESPWIFI_GPIO_H

#include <ArduinoJson.h>

#include "ESPWiFi.h"

void ESPWiFi::enableGPIO() {
  webServer.on("/gpio/on", HTTP_POST, [this]() {
    digitalWrite(LED_BUILTIN, HIGH);
    webServer.send(200, "text/plain", "LED on");
  });

  webServer.on("/gpio/off", HTTP_POST, [this]() {
    digitalWrite(LED_BUILTIN, LOW);
    webServer.send(200, "text/plain", "LED off");
  });

  webServer.on("/gpio", HTTP_POST, [this]() {
    String body = webServer.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
      webServer.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

    int number = doc["number"];
    String mode = doc["mode"];
    String state = doc["state"];

    if (mode == "OUTPUT") {
      pinMode(number, OUTPUT);
    } else if (mode == "INPUT") {
      pinMode(number, INPUT);
    } else {
      webServer.send(400, "application/json", "{\"error\":\"Invalid mode\"}");
      return;
    }

    if (state == "HIGH") {
      digitalWrite(number, HIGH);
    } else if (state == "LOW") {
      digitalWrite(number, LOW);
    } else {
      webServer.send(400, "application/json", "{\"error\":\"Invalid state\"}");
      return;
    }

    webServer.send(200, "application/json", "{\"status\":\"Success\"}");
  });
}

#endif  // ESPWIFI_GPIO_H