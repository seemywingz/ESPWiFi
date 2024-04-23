#ifndef ESPWIFI_OPENAI_H
#define ESPWIFI_OPENAI_H

#include "ESPWiFi.h"

String openAI_URL = "https://api.openai.com/v1/";

String ESPWiFi::openAIChat(String text) {
  if (config["openAI"]["apiKey"] == "") {
    Serial.println("OpenAI key not set");
    return "";
  }

  String url = openAI_URL + "chat/completions";

  // Prepare the JSON payload
  DynamicJsonDocument doc(512);
  doc["model"] = "gpt-4-turbo-preview";
  doc["messages"] = JsonArray();
  doc["messages"].add(JsonObject());
  doc["messages"][0]["role"] = "system";
  doc["messages"][0]["content"] =
      config["openAI"]["system_message"].as<String>();
  doc["messages"].add(JsonObject());
  doc["messages"][1]["role"] = "user";
  doc["messages"][1]["content"] = text;
  doc["max_tokens"] = 100;
  String payload;
  serializeJson(doc, payload);
  String contentType = "application/json";

  // Make the request
  String response = makeHTTPSRequest("POST", url, config["openAI"]["apiKey"],
                                     contentType, payload);

  DynamicJsonDocument resDoc(512);
  deserializeJson(resDoc, response);
  if (resDoc["choices"][0]["message"]["content"].is<String>()) {
    return resDoc["choices"][0]["message"]["content"].as<String>();
  } else {
    return "Error parsing response";
  }

  return "Sorry, I didn't understand that. Please try again.";
}

void ESPWiFi::openAI_TTS(String text, String filePath) {
  if (config["openAI"]["apiKey"] == "") {
    Serial.println("OpenAI key not set,");
    return;
  }

  String url = openAI_URL + "audio/speech";
  String extension = getFileExtension(filePath);

  // Prepare the JSON payload
  DynamicJsonDocument doc(512);
  doc["model"] = "tts-1";
  doc["input"] = text;
  doc["voice"] = config["openAI"]["voice"].as<String>();
  doc["response_format"] = extension;

  String payload;
  serializeJson(doc, payload);

  // Configure the secure client and make the request
  WiFiClientSecure client;
  client.setInsecure();  // Disable certificate verification (not recommended
                         // for production)
  HTTPClient http;
  http.begin(client, url);

  // Set headers
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization",
                 "Bearer " + config["openAI"]["apiKey"].as<String>());

  int httpCode = http.POST(payload);

  if (httpCode == HTTP_CODE_OK) {
    // Open the file for writing in binary mode
    Serial.println("OpenAI TTS request successful, writing to file...");
    File file = LittleFS.open(filePath, "w+");
    if (!file) {
      Serial.println("Failed to open file for writing");
      http.end();  // End the connection
      return;
    }

    // Stream the response into the file
    http.writeToStream(&file);
    file.close();
    Serial.println("File written successfully");
  } else {
    Serial.print("HTTP POST failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}

#endif  // ESPWIFI_OPENAI_H