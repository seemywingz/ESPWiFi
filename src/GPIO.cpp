#ifndef ESPWIFI_GPIO_H
#define ESPWIFI_GPIO_H

#include "ESPWiFi.h"

void ESPWiFi::enableGPIO() {
  webServer.on("/gpio/on", HTTP_GET, [this]() {
    digitalWrite(LED_BUILTIN, HIGH);
    webServer.send(200, "text/plain", "LED on");
  });

  webServer.on("/gpio/off", HTTP_GET, [this]() {
    digitalWrite(LED_BUILTIN, LOW);
    webServer.send(200, "text/plain", "LED off");
  });
}

#endif  // ESPWIFI_GPIO_H