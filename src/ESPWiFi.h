#ifndef ESPWiFi_h
#define ESPWiFi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

class ESPWiFi {
 public:
  DynamicJsonDocument config = DynamicJsonDocument(256);

  String configFile = "/config.json";

  WebServer webServer;

  int maxConnectAttempts = 120;

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
    while (WiFi.status() != WL_CONNECTED) {
      if (--maxConnectAttempts == 0) {
        Serial.println("Failed to connect to WiFi");
        maxConnectAttempts = 90;
        defaultConfig();
        startAP();
        return;
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

  void startAP() {
    String ssid = config["ap"]["ssid"];
    String password = config["ap"]["password"];
    WiFi.softAP(ssid, password);
    Serial.println("\n\nStarting Access Point:");
    Serial.println("\tSSID: " + ssid);
    Serial.println("\tPassword: " + password);
    Serial.print("\tIP Address: ");
    Serial.println(WiFi.softAPIP());
  }

  void handleClient() { webServer.handleClient(); }

  // Utils
  String getContentType(String filename);
  String getFileExtension(const String& filename);
  String makeHTTPSRequest(const String& method, const String& url,
                          const String& token = "",
                          const String& contentType = "",
                          const String& payload = "");

  // OpenAI
  String openAIChat(String text);
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
        File file = LittleFS.open(configFile, "w");
        serializeJson(config, file);
        file.close();
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
    config["ap"]["ssid"] = "ESPWiFi-" + String(ESP.getEfuseMac(), HEX);
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