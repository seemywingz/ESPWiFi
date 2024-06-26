#ifndef ESPWiFi_h
#define ESPWiFi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AudioFileSourceLittleFS.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <IOPin.h>
#include <LittleFS.h>
#include <WiFiClient.h>

#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#define WebServer ESP8266WebServer
#define BCLK 15
#define LRC 2
#define DOUT 3
#else
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <WiFi.h>
#define BCLK 26
#define LRC 25
#define DOUT 22
#endif

class ESPWiFi {
 public:
  JsonDocument config;

  String configFile = "/config.json";

  WebServer webServer;

  int maxConnectAttempts = 10;

  void (*connectSubroutine)() = nullptr;

  ESPWiFi() {}

  void init() {
    Serial.begin(115200);
    while (!Serial) {
      delay(10);
    }

    if (!LittleFS.begin()) {
      Serial.println("An Error has occurred while mounting LittleFS");
    }
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
  }

  void start() {
    init();
    readConfig();

    if (config["mode"] == "ap") {
      startAP();
    } else {
      connectToWifi();
    }

    startWebServer();
    startMDNS();
  }

  void connectToWifi() {
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

  int selectBestChannel() {
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

  void startAP() {
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

  void handleClient() {
    webServer.handleClient();
#ifdef ESP8266
    MDNS.update();
#endif
  }

  void saveConfig() {
    File file = LittleFS.open(configFile, "w");
    serializeJson(config, file);
    file.close();
  }

  // Utils
  String getContentType(String filename);
  String getFileExtension(const String& filename);
  String makeHTTPSRequest(const String& method, const String& url,
                          const String& token = "",
                          const String& contentType = "",
                          const String& payload = "");
  void runAtInterval(unsigned int interval, unsigned long& lastIntervalRun,
                     std::function<void()> functionToRun);

  // Audio
  void startAudio();
  void handleAudio();
  bool pttEnabled = false;
  void playMP3(String file);
  std::function<void()> audioResponse = []() {};

  // OpenAI
  String openAI_URL = "https://api.openai.com";
  String openAI_TTSEndpoint = "/v1/audio/speech";
  String openAI_ChatEndpoint = "/v1/chat/completions";
  String openAI_Chat(String text);
  void openAI_TTS(String text, String filePath);

 private:
  void startMDNS() {
    String domain = config["mdns"];
    if (!MDNS.begin(domain)) {
      Serial.println("Error setting up MDNS responder!");
    } else {
      MDNS.addService("http", "tcp", 80);
      Serial.println("\tDomain Name: " + domain + ".local");
    }
  }

  void startWebServer() {
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
        webServer.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      } else {
        saveConfig();
        webServer.send(200, "application/json", "{\"success\":true}");
      }
    });

    webServer.on("/restart", HTTP_POST, [this]() {
      webServer.send(200, "application/json", "{\"success\":true}");
      Serial.println("Restarting...");
      delay(1000);
      ESP.restart();
    });

    // Handler to list files
    webServer.on("/list-files", HTTP_GET, [this]() { listFilesHandler(); });

    webServer.begin();
  }

  void readConfig() {
    File file = LittleFS.open(configFile, "r");
    if (!file) {
      Serial.println("Failed to open config file");
      defaultConfig();
      return;
    }

    DeserializationError error = deserializeJson(config, file);
    if (error) {
      Serial.println("Failed to read config file: " + String(error.c_str()));
      defaultConfig();
      file.close();
      return;
    }

    if (config["client"]["ssid"] == "" || config["client"]["password"] == "") {
      Serial.println("Invalid client config");
      defaultConfig();
    }

    file.close();
  }

  void defaultConfig() {
    Serial.println("Using default config");
    config["mode"] = "ap";
#ifdef ESP8266
    config["ap"]["ssid"] = "ESPWiFi-" + String(ESP.getChipId(), HEX);
#else
    config["ap"]["ssid"] = "ESPWiFi-" + String(ESP.getEfuseMac(), HEX);
#endif
    config["ap"]["password"] = "abcd1234";
    config["mdns"] = "esp32";
    config["client"]["ssid"] = "";
    config["client"]["password"] = "";
  }

  void listFilesHandler() {
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
  }
};
#endif  // ESPWiFi_h