// NodeMcu Mini Wireless D1 Module: ESP8266 ESP-12F
#include <ESPWiFi.h>

#include "indexHTML.h"

// setup the wifi
String ssid = "connectedness";             // your network SSID (name)
String password = "ReallyLongPassword123!@#";  // your network password
ESPWiFi espWiFi(ssid, password);

void setup() {
  Serial.begin(115200);  // output to the serial terminal for debugging
  initHandlers();
  espWiFi.startAsClient();  // start the web server
}

void loop() {
  espWiFi.handleClient();  // Handling of incoming requests
  espWiFi.runMillis(1000, []() {
    Serial.println(espWiFi.infoString());
  });  // print info every second
}

void toggleLED() {  // toggle the LED
  if (digitalRead(LED_BUILTIN) == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
    espWiFi.webserver()->send(200, "text/plain", "LED on");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    espWiFi.webserver()->send(200, "text/plain", "LED off");
  }
  Serial.println("LED toggle");
}

void initHandlers() {  // initialize the handlers
  espWiFi.webserver()->on("/", HTTP_GET, []() {
    espWiFi.webserver()->send(200, "text/html", indexHTML);
  });

  espWiFi.webserver()->on("/led/toggle", []() { toggleLED(); });

  espWiFi.webserver()->onNotFound(
      []() { espWiFi.webserver()->send(404, "text/html", boardInfo()); });
}

String boardInfo() {  // return the board info as a HTML string
  String message = "<html><body>";
  message += "<h1>Board Information</h1>";
  message += "<p>URI: " + espWiFi.webserver()->uri() + "</p>";
  message +=
      "<p>Method: " +
      String(espWiFi.webserver()->method() == HTTP_GET ? "GET" : "POST") +
      "</p>";
  message += "<h2>Request Parameters</h2>";
  message += "<ul>";
  for (uint8_t i = 0; i < espWiFi.webserver()->args(); i++) {
    message += "<li>" + espWiFi.webserver()->argName(i) + ": " +
               espWiFi.webserver()->arg(i) + "</li>";
  }
  message += "</ul>";
  message += "<h2>Board Details</h2>";
  message += "<ul>";
  message +=
      "<li><strong>Board Type:</strong> NodeMcu Mini Wireless D1 Module "
      "(ESP8266 ESP-12F)</li>";
  message += "<li><strong>Firmware Version:</strong> " +
             String(ESP.getCoreVersion()) + "</li>";
  message += "<li><strong>SDK Version:</strong> " +
             String(ESP.getSdkVersion()) + "</li>";
  message += "<li><strong>Flash Chip ID:</strong> " +
             String(ESP.getFlashChipId(), HEX) + "</li>";
  message += "<li><strong>Flash Chip Size:</strong> " +
             String(ESP.getFlashChipSize() / (1024 * 1024)) + " MB</li>";
  message += "<li><strong>Free Heap:</strong> " + String(ESP.getFreeHeap()) +
             " bytes</li>";
  message +=
      "<li><strong>Chip ID:</strong> " + String(ESP.getChipId(), HEX) + "</li>";
  message += "<li><strong>CPU Frequency:</strong> " +
             String(ESP.getCpuFreqMHz()) + " MHz</li>";
  message += "<li><strong>Flash Frequency:</strong> " +
             String(ESP.getFlashChipSpeed() / 1000000) + " MHz</li>";
  message += "<li><strong>Flash Mode:</strong> " +
             String(ESP.getFlashChipMode()) + "</li>";
  message += "</ul>";
  message += "<h2>LED State</h2>";
  message += "<p>" +
             String((digitalRead(LED_BUILTIN) == HIGH) ? "LED off" : "LED on") +
             "</p>";
  message += "</body></html>";
  return message;
}
