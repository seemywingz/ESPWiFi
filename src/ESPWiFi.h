#ifndef ESPWiFi_h
#define ESPWiFi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <IOPin.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define BCLK 26
#define LRC 25
#define DOUT 22
#define LED_BUILTIN 2

class ESPWiFi {
 public:
  JsonDocument config;

  String configFile = "/config.json";

  WebServer webServer;

  int maxConnectAttempts = 18;

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
    startClientTask();
  }

  // Config
  void saveConfig();
  void readConfig();
  void defaultConfig();
  void resetConfig();

  // WiFi
  void connectToWifi();
  void handleClient();
  void startAP();
  int selectBestChannel();

  // Utils
  String getContentType(String filename);
  String getFileExtension(const String& filename);
  // String makeHTTPSRequest(const String& method, const String& url,
  //                         const String& token = "",
  //                         const String& contentType = "",
  //                         const String& payload = "");
  void runAtInterval(unsigned int interval, unsigned long& lastIntervalRun,
                     std::function<void()> functionToRun);

  // GPIO
  void startGPIO();

  // OpenAI
  // String openAI_URL = "https://api.openai.com";
  // String openAI_TTSEndpoint = "/v1/audio/speech";
  // String openAI_ChatEndpoint = "/v1/chat/completions";
  // String openAI_Chat(String text);
  // void openAI_TTS(String text, String filePath);

  // #############################################################################################################
 private:
  void startMDNS();
  void startWebServer();
  void startClientTask();
  void handleCorsPreflight();
  void listFilesHandler();
};
#endif  // ESPWiFi_h