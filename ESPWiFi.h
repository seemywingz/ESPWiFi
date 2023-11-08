#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

class ESPWiFi {
 private:
  const String version = "0.0.1";
  String ssid;
  String password;
  String hostname;
  String indexHTML;
  String domain;
  bool APEnabled;
  bool mDNSEnabled;
  IPAddress ip;
  IPAddress subnet;
  IPAddress gateway;
  ESP8266WebServer webServer;
  void (*connectSubroutine)();
  const unsigned long wifiCheckInterval = 60000;  // Check every 60 seconds

 public:
  ESPWiFi(String defaultSSID, String defaultPassword);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP,
          IPAddress customGateway, IPAddress customSubnet);
  void runMillis(unsigned long millis, std::function<void()> callback);
  void setConnectSubroutine(void (*subroutine)());
  String MACAddressToString(uint8_t* mac);
  void enableMDNS(String domainName);
  ESP8266WebServer* webserver();
  void startAsAccessPoint();
  String pageNotFoundHTML();
  String sharedHTMLStyle();
  String clientIndexHTML();
  String boardInfoHTML();
  String APIndexHTML();
  void connectToWiFi();
  void startAsClient();
  IPAddress localIP();
  void handleClient();
  String infoString();
  void checkWiFi();
};

#endif
