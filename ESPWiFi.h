#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

class ESPWiFi {
 private:
  const unsigned long wifiCheckInterval = 6000;  // 6 seconds
  int connectionRestarts = 0;
  int maxConnectionRestarts = 6;
  const String connectionAttemptsFile = "/connection-attempts";
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
  void saveConnectionAttempts(int attempts);
  int loadConnectionAttempts();
  void (*connectSubroutine)();
  void initializeMDNS();
  void startWebServer();

 public:
  ESP8266WebServer webServer;
  String indexHTMLFile = "/index.html";
  String wifiCredentialsFile = "/wifi-credentials";
  ESPWiFi(String defaultSSID, String defaultPassword);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP,
          IPAddress customGateway, IPAddress customSubnet);
  void oncePerMillis(unsigned long millis, std::function<void()> callback);
  void saveWiFiCredentials(const String& ssid, const String& password);
  void setConnectSubroutine(void (*subroutine)());
  String MACAddressToString(uint8_t* mac);
  String getContentType(String filename);
  void enableMDNS(String domainName);
  String eraseCredentialsPageHTML();
  void clearWiFiCredentials();
  bool loadWiFiCredentials();
  void startAsAccessPoint();
  String pageNotFoundHTML();
  String sharedHTMLStyle();
  String clientIndexHTML();
  void checkWiFiStartup();
  String setupPageHTML();
  String boardInfoHTML();
  String APIndexHTML();
  void connectToWiFi();
  void startAsClient();
  IPAddress localIP();
  void handleClient();
  String infoString();
  void checkWiFi();
  void Start();
};

#endif
