#ifndef ESPWIFI_OPENAI_H
#define ESPWIFI_OPENAI_H

#include "ESPWiFi.h"

String ESPWiFi::openAI_Chat(String text) {
  if (config["openAI"]["apiKey"] == "") {
    Serial.println("OpenAI key not set");
    return "";
  }

  // Prepare the JSON payload
  JsonDocument doc;
  doc["model"] = "gpt-4-turbo-preview";
  JsonArray messages = doc["messages"].to<JsonArray>();

  JsonObject messages_0 = messages.add<JsonObject>();
  messages_0["role"] = "system";
  messages_0["content"] = config["openAI"]["system_message"].as<String>();

  JsonObject messages_1 = messages.add<JsonObject>();
  messages_1["role"] = "user";
  messages_1["content"] = text;
  doc["max_tokens"] = 90;

  doc.shrinkToFit();

  String payload;
  serializeJson(doc, payload);

  Serial.println("Sending request to OpenAI Chat");
  Serial.println("\tPayload: " + payload);

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
  String response = http.getString();

  Serial.println("\tResponse: " + response);

  if (httpCode == HTTP_CODE_OK) {
    doc.clear();
    deserializeJson(doc, response);
    String resString = doc["choices"][0]["message"]["content"].as<String>();
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
}

void ESPWiFi::openAI_TTS(String text, String filePath) {
  if (config["openAI"]["apiKey"] == "") {
    Serial.println("OpenAI key not set,");
    return;
  }

  // Prepare the JSON payload
  JsonDocument doc;
  doc["model"] = "tts-1";
  doc["input"] = text;
  doc["voice"] = config["openAI"]["voice"].as<String>();
  doc["response_format"] = getFileExtension(filePath);
  doc.shrinkToFit();

  String payload;
  serializeJson(doc, payload);

  Serial.println("Sending request to OpenAI TTS");
  Serial.println("\tPayload: " + payload);

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
  Serial.println("\tResponse HTTP Code: " + String(httpCode));

  if (httpCode == HTTP_CODE_OK) {
    // Open the file for writing in binary mode
    Serial.println("Writing OpenAI TTS Response to file: " + filePath);
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
    Serial.print("OpenAI TTS HTTP POST, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
    http.end();
  }
}

#endif  // ESPWIFI_OPENAI_H