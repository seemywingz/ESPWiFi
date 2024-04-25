#ifndef ESPWIFI_OPENAI_H
#define ESPWIFI_OPENAI_H

#include "ESPWiFi.h"

String ESPWiFi::openAI_Chat(String text) {
  if (config["openAI"]["apiKey"] == "") {
    Serial.println("OpenAI key not set");
    return "";
  }

  // Prepare the JSON payload
  DynamicJsonDocument doc(512);
  doc["model"] = "gpt-3.5-turbo";
  doc["messages"] = JsonArray();
  doc["messages"].add(JsonObject());
  doc["messages"][0]["role"] = "system";
  doc["messages"][0]["content"] =
      config["openAI"]["system_message"].as<String>();
  doc["messages"].add(JsonObject());
  doc["messages"][1]["role"] = "user";
  doc["messages"][1]["content"] = text;
  doc["max_tokens"] = 120;

  String payload;
  serializeJson(doc, payload);

  WiFiClientSecure client;
  client.setInsecure();     // Disable certificate verification
  client.setTimeout(9000);  // 15 seconds

  HTTPClient http;
  http.begin(client, openAI_URL + openAI_ChatEndpoint);

  // Set headers
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization",
                 "Bearer " + config["openAI"]["apiKey"].as<String>());

  int httpCode = http.POST(payload);

  if (httpCode == HTTP_CODE_OK) {
    DynamicJsonDocument resDoc(512);
    deserializeJson(resDoc, http.getString());
    String resString = resDoc["choices"][0]["message"]["content"].as<String>();
    http.end();
    if (resString != "") {
      return resString;
    } else {
      return "Error parsing response";
    }
  } else {
    String error =
        "OpenAI Chat HTTP POST, error: " + http.errorToString(httpCode);
    Serial.println(error);
    http.end();
    return error;
  }

  return "Sorry, I didn't understand that. Please try again.";
}

void ESPWiFi::openAI_TTS(String text, String filePath) {
  if (config["openAI"]["apiKey"] == "") {
    Serial.println("OpenAI key not set,");
    return;
  }

  // Prepare the JSON payload
  DynamicJsonDocument doc(512);
  doc["model"] = "tts-1";
  doc["input"] = text;
  doc["voice"] = config["openAI"]["voice"].as<String>();
  doc["response_format"] = getFileExtension(filePath);

  String payload;
  serializeJson(doc, payload);

  // Configure the secure client and make the request
  WiFiClientSecure client;
  client.setInsecure();  // Disable certificate verification (not recommended
                         // for production)
  HTTPClient http;
  http.begin(client, openAI_URL + openAI_TTSEndpoint);

  // Set headers
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization",
                 "Bearer " + config["openAI"]["apiKey"].as<String>());

  int httpCode = http.POST(payload);

  if (httpCode == HTTP_CODE_OK) {
    // Open the file for writing in binary mode
    Serial.println("OpenAI TTS request successful");
    Serial.println("Writing to file: " + filePath);
    File file = LittleFS.open(filePath, "w+");
    if (!file) {
      Serial.println("Failed to open file for writing");
      http.end();  // End the connection
      return;
    }
    // Stream the response into the file
    http.writeToStream(&file);
    file.close();
    http.end();
    Serial.println("File written successfully");
  } else {
    Serial.print("HTTP POST failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
    http.end();
  }
}

#endif  // ESPWIFI_OPENAI_H