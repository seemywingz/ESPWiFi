#ifndef UTILS_H
#define UTILS_H

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <WiFiClient.h>

String getFileExtension(const String& filename) {
  int dotIndex = filename.lastIndexOf(".");
  if (dotIndex == -1) {
    return "";
  }
  return filename.substring(dotIndex + 1);
}

String makeHTTPSRequest(const String& method, const String& url,
                        const String& token = "",
                        const String& contentType = "",
                        const String& payload = "") {
  WiFiClientSecure client;  // Using WiFiClientSecure for HTTPS
  client.setInsecure();     // Disable certificate verification (not recommended
                            // for production)
  HTTPClient http;
  http.begin(client, url);  // Start the client with URL

  // Set headers if necessary
  if (method == "POST") {
    if (!contentType.isEmpty()) {
      http.addHeader("Content-Type", contentType);
    }
    if (!token.isEmpty()) {
      http.addHeader("Authorization", "Bearer " + token);
    }
  }

  // Make the HTTP request according to the method
  int httpCode = (method == "POST") ? http.POST(payload) : http.GET();

  // Process the HTTP response
  String response;
  if (httpCode > 0) {  // Check is there's a response
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      response = http.getString();  // Get the response payload
    }
  } else {
    response = "Error on HTTP request: " +
               http.errorToString(httpCode);  // Handle errors
  }

  http.end();  // End the connection
  return response;
}

#endif UTILS_H