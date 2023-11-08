#include "ESPWiFi.h"

ESPWiFi::ESPWiFi(String ssid, String password) {
  this->subnet = IPAddress(255, 255, 255, 0);
  this->ip = IPAddress(10, 10, 10, 10);
  this->password = password;
  this->gateway = ip;
  this->ssid = ssid;
}

ESPWiFi::ESPWiFi(String ssid, String password, IPAddress ip) {
  this->ip = ip;
  this->ssid = ssid;
  this->gateway = ip;
  this->password = password;
  this->subnet = IPAddress(255, 255, 255, 0);
}

ESPWiFi::ESPWiFi(String ssid, String password, IPAddress ip, IPAddress gateway,
                 IPAddress subnet) {
  this->ip = ip;
  this->ssid = ssid;
  this->subnet = subnet;
  this->gateway = gateway;
  this->password = password;
}

void ESPWiFi::connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    if (connectSubroutine != NULL) {
      connectSubroutine();
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
    }
  }
  webServer.begin();
  ip = WiFi.localIP();
  gateway = WiFi.gatewayIP();
  subnet = WiFi.subnetMask();
  Serial.println(infoString());
  analogWrite(LED_BUILTIN, 255 / 2);
  Serial.print("Connected to WiFi Network: ");
}

void ESPWiFi::startAsClient() {
  APEnabled = false;
  connectToWiFi();
  webServer.on("/", HTTP_GET, [this]() {
    webServer.send(200, "text/html", clientIndexHTML());
  });
  webServer.onNotFound(
      [this]() { webServer.send(200, "text/html", pageNotFoundHTML()); });
}

void ESPWiFi::startAsAccessPoint() {
  APEnabled = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  webServer.on("/", HTTP_GET,
               [this]() { webServer.send(200, "text/html", APIndexHTML()); });
  webServer.onNotFound(
      [this]() { webServer.send(200, "text/html", pageNotFoundHTML()); });
  webServer.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

String ESPWiFi::MACAddressToString(uint8_t* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1],
           mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

String ESPWiFi::infoString() {
  String infoString = "ESPWiFi Version: " + version + "\n";
  infoString += "SSID: " + ssid + "\n";
  // infoString += "Password: " + password + "\n";
  infoString += "IP Address: " + ip.toString() + "\n";
  infoString += "Gateway: " + gateway.toString() + "\n";
  infoString += "Subnet: " + subnet.toString() + "\n";
  return infoString;
}

void ESPWiFi::runMillis(unsigned long m, std::function<void()> callback) {
  static unsigned long lastTime = 0;
  if (millis() - lastTime >= m) {
    lastTime = millis();
    callback();
  }
}

void ESPWiFi::checkWiFi() {
  if (APEnabled) {
    return;
  }
  runMillis(1000, [this]() {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Lost Wi-Fi connection. Reconnecting...");
      connectToWiFi();
    }
  });
}

void ESPWiFi::setConnectSubroutine(void (*subroutine)()) {
  connectSubroutine = subroutine;
}

void ESPWiFi::handleClient() {
  webServer.handleClient();
  checkWiFi();
}

bool ESPWiFi::isAccessPoint() { return APEnabled; }
IPAddress ESPWiFi::localIP() { return WiFi.softAPIP(); }
ESP8266WebServer* ESPWiFi::webserver() { return &webServer; }
