#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

class ESPWiFi {
 private:
  const String version = "0.0.1";
  String ssid;
  String password;
  String hostname;
  String indexHTML;
  bool APEnabled;
  bool DNSEnabled;
  IPAddress ip;
  IPAddress subnet;
  IPAddress gateway;
  ESP8266WebServer webServer;
  const unsigned long wifiCheckInterval = 60000;  // Check every 60 seconds

 public:
  ESPWiFi(String defaultSSID, String defaultPassword);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP);
  ESPWiFi(String defaultSSID, String defaultPassword, IPAddress customIP,
          IPAddress customGateway, IPAddress customSubnet);
  void runMillis(unsigned long millis, std::function<void()> callback);
  String MACAddressToString(uint8_t* mac);
  ESP8266WebServer* webserver();
  void startAsAccessPoint();
  String clientIndexHTML();
  String APIndexHTML();
  void connectToWiFi();
  bool isAccessPoint();
  void startAsClient();
  IPAddress localIP();
  void handleClient();
  String infoString();
  void checkWiFi();
};

#endif
