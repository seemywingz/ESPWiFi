#ifndef ESPWIFI_WEBSERVER_H
#define ESPWIFI_WEBSERVER_H

#include "ESPWiFi.h"

void ESPWiFi::startWebServer() {
  webServer.on("/", HTTP_GET, [this]() {
    File file = LittleFS.open("/index.html", "r");
    if (file) {
      webServer.streamFile(file, "text/html");
      file.close();
    }
  });

  // Generic handler for all file requests
  webServer.onNotFound([this]() {
    String path = webServer.uri();
    if (LittleFS.exists(path)) {
      File file = LittleFS.open(path, "r");
      String contentType =
          getContentType(path);  // Determine the file's MIME type
      webServer.streamFile(file, contentType);
      file.close();
    } else {
      webServer.send(200, "text/html", "404: Not Found");
    }
  });

  webServer.on("/config", HTTP_GET, [this]() {
    String response;
    serializeJson(config, response);
    webServer.send(200, "application/json", response);
  });

  webServer.on("/config", HTTP_POST, [this]() {
    String body = webServer.arg("plain");
    DeserializationError error = deserializeJson(config, body);
    if (error) {
      webServer.send(400, "application/json",
                     "{\"error:\"" + String(error.c_str()) + "\"}");
    } else {
      saveConfig();
      String response;
      serializeJson(config, response);
      webServer.send(200, "application/json", response);
    }
  });

  webServer.on("/restart", HTTP_GET, [this]() {
    webServer.send(200, "application/json", "{\"success\":true}");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  });

  // Handler to list files
  webServer.on("/list-files", HTTP_GET, [this]() { listFilesHandler(); });
  webServer.begin();
};

void ESPWiFi::listFilesHandler() {
  String path = "/";
  File root = LittleFS.open(path, "r");
  String fileNames = "[";  // Start JSON array
  File file = root.openNextFile();
  while (file) {
    if (fileNames.length() > 1) {
      fileNames += ",";
    }
    fileNames += "\"" + String(file.name()) + "\"";
    file = root.openNextFile();
  }
  fileNames += "]";                                    // Close JSON array
  webServer.send(200, "application/json", fileNames);  // Send JSON response
};

#endif  // ESPWIFI_WEBSERVER_H