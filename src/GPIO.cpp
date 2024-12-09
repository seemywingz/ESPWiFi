#ifndef ESPWIFI_GPIO_H
#define ESPWIFI_GPIO_H

#include <ArduinoJson.h>

#include "ESPWiFi.h"

void ESPWiFi::enableGPIO() {
  webServer.on("/gpio/on", HTTP_POST, [this]() {
    digitalWrite(LED_BUILTIN, HIGH);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "text/plain", "LED on");
  });

  webServer.on("/gpio/off", HTTP_POST, [this]() {
    digitalWrite(LED_BUILTIN, LOW);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "text/plain", "LED off");
  });

  webServer.on("/gpio", HTTP_OPTIONS, [this]() { handleCorsPreflight(); });
  webServer.on("/gpio", HTTP_POST, [this]() {
    String body = webServer.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
      webServer.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

    int num = doc["num"];
    String mode = doc["mode"];
    String state = doc["state"];
    int duty = doc["duty"];

    mode.toLowerCase();
    state.toLowerCase();

    if (mode == "out" || mode == "output" || mode == "pwm") {
      pinMode(num, OUTPUT);
    } else if (mode == "input" || mode == "in") {
      pinMode(num, INPUT);
    } else {
      webServer.send(400, "application/json",
                     "{\"error\":\"Invalid mode\": \"" + mode + "\"}");
      return;
    }

    if (state == "high") {
      if (mode == "pwm") {
        analogWrite(num, duty);
      } else {
        digitalWrite(num, HIGH);
      }
    } else if (state == "low") {
      digitalWrite(num, LOW);
    } else {
      webServer.send(400, "application/json",
                     "{\"error\":\"Invalid state\" : \"" + state + "\"}");

      return;
    }

    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", "{\"status\":\"Success\"}");
    Serial.println("GPIO " + String(num) + " " + mode + " " + state + " " +
                   String(duty));
  });
}

#endif  // ESPWIFI_GPIO_H