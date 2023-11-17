#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

class ESPWiFi {
 private:
  const String version = "0.0.1";
  String ssid;
  String domain;
  String password;
  String hostname;
  String indexHTML;
  bool APEnabled;
  bool mDNSEnabled;
  IPAddress ip;
  IPAddress subnet;
  IPAddress gateway;
  ESP8266WebServer webServer;
  void (*connectSubroutine)();
  void initializeMDNS();
  void startWebServer();
  const unsigned long wifiCheckInterval = 60000;  // Check every 60 seconds

 public:
  String indexHTMLPath = "/index.html";
  String wifiCredentialsPath = "/wifi_credentials.txt";
  ESPWiFi(String defaultSSID, String defaultPassword);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP,
          IPAddress customGateway, IPAddress customSubnet);
  void runMillis(unsigned long millis, std::function<void()> callback);
  void saveWiFiCredentials(const String& ssid, const String& password);
  void setConnectSubroutine(void (*subroutine)());
  String MACAddressToString(uint8_t* mac);
  String getContentType(String filename);
  void enableMDNS(String domainName);
  String eraseCredentialsPageHTML();
  ESP8266WebServer* webserver();
  void clearWiFiCredentials();
  bool loadWiFiCredentials();
  void startAsAccessPoint();
  String pageNotFoundHTML();
  String sharedHTMLStyle();
  String clientIndexHTML();
  String setupPageHTML();
  String boardInfoHTML();
  String APIndexHTML();
  void connectToWiFi();
  void startAsClient();
  IPAddress localIP();
  void handleClient();
  String infoString();
  void checkWiFi();
  void setupAPMode();
  void Start();
};

#endif
