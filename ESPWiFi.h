#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>

class ESPWiFi {
 private:
  int hidden = 0;
  int channel = 3;
  int maxConnections = 9;
  int connectionRestarts = 0;
  int maxConnectionRestarts = 6;
  const String version = "0.0.1";
  const String connectionAttemptsFile = "/connection-attempts";
  const unsigned long wifiCheckInterval = 30000;  // 30 seconds
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
  int countNetworksOnChannel(int channel);
  int findBestChannel();

 public:
  WebServer webServer;
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
  void start();
};

#endif
