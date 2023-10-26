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
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
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
}

void ESPWiFi::startAsAccessPoint() {
  APEnabled = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  webServer.on("/", HTTP_GET,
               [this]() { webServer.send(200, "text/html", APIndexHTML()); });
  webServer.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

String ESPWiFi::clientIndexHTML() {
  bool autoRefresh = false;
  String refreshScript = "";

  if (webServer.hasArg("autoRefresh")) {
    if (webServer.arg("autoRefresh") == "true") {
      autoRefresh = true;
      refreshScript =
          "<script>"
          "setTimeout(function() {"
          "location.href = '/?autoRefresh=true';"
          "}, 3000);"
          "</script>";
    }
  }

  String indexHTML =
      "<!DOCTYPE html><html><head><meta name='viewport' "
      "content='width=device-width, initial-scale=1'>";

  indexHTML +=
      "<style>body{font-family: Arial, sans-serif; background-color: "
      "silver; text-align: center; margin-top: 50px;}"
      ".container {background-color: #ffffff; border-radius: 10px; "
      "box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.1); padding: 20px; "
      "text-align: left; display: inline-block;}"
      ".section {margin-bottom: 20px;}"
      ".header {font-size: 24px; color: #007bff; margin-bottom: 10px; "
      "text-align: center;}"
      ".info {font-size: 18px; color: #333; margin-bottom: 5px; text-align: "
      "center;}"
      ".wifi-signal { display: inline-block; width: 50px; height: 25px; "
      "position: relative; }"
      ".bar { width: 10%; height: 20%; background-color: silver; position: "
      "absolute; bottom: 0; }"
      ".bar.filled { background-color: black; }"
      ".bar:nth-child(1) { left: 0; }"
      ".bar:nth-child(2) { left: 20%; height: 40%; }"
      ".bar:nth-child(3) { left: 40%; height: 60%; }"
      ".bar:nth-child(4) { left: 60%; height: 80%; }"
      ".bar:nth-child(5) { left: 80%; height: 100%;}"
      "</style>" +
      refreshScript + "</head><body>";

  indexHTML += "<div class='container'>";
  indexHTML += "<div class='section'>";
  indexHTML += "<div class='header'>Client Information</div>";

  // RSSI Bars Visualization
  int rssi = WiFi.RSSI();
  int filledBars =
      map(rssi, -100, -30, 0, 5);  // Map the RSSI value to number of bars
  String rssiDisplay = "<div class='wifi-signal'>";
  for (int i = 0; i < 5; i++) {
    rssiDisplay += "<div class='bar" + String(i < filledBars ? " filled" : "") +
                   "'></div>";
  }
  rssiDisplay += "</div>";

  indexHTML += "<div class='info'>SSID: " + String(WiFi.SSID()) + "</div>";
  indexHTML +=
      "<div class='info'>IP Address: " + WiFi.localIP().toString() + "</div>";
  indexHTML += "<div class='info'>RSSI: " + String(rssi) + " dBm " +
               rssiDisplay + "</div>";
  indexHTML += "<div class='section'>";
  indexHTML += "<div class='header'>Connected Hosts</div>";

  // Get the ESP8266's IP address
  IPAddress startIP = ip;  // Start IP address
  startIP[3] = 1;          // Set the last octet to 1

  // Calculate the end IP address (adjust this based on your network size)
  IPAddress endIP = ip;  // End IP address
  endIP[3] = 254;        // Set the last octet to 254

  indexHTML += "</div>";
  indexHTML += "<button onclick=\"location.href='/?autoRefresh=" +
               String((autoRefresh ? "false" : "true")) + "'\">" +
               String((autoRefresh ? "Disable" : "Enable")) +
               " Auto-Refresh</button>";
  indexHTML += "</div>";
  indexHTML += "</div></body></html>";

  return indexHTML;
}

String ESPWiFi::APIndexHTML() {
  String indexHTML =
      "<!DOCTYPE html><html><head><meta name='viewport' "
      "content='width=device-width, initial-scale=1'>";
  indexHTML +=
      "<style>body{font-family: Arial, sans-serif; background-color: "
      "silver; text-align: center; margin-top: 50px;}";
  indexHTML +=
      ".container {background-color: #ffffff; border-radius: 10px; "
      "box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.1); padding: 20px; "
      "text-align: left; display: inline-block;}";
  indexHTML += ".section {margin-bottom: 20px;}";
  indexHTML +=
      ".header {font-size: 24px; color: #007bff; margin-bottom: 10px; "
      "text-align: center;}";
  indexHTML +=
      ".info {font-size: 18px; color: #333; margin-bottom: 5px; text-align: "
      "center;}";
  indexHTML += "</style></head><body>";
  indexHTML += "<div class='container'>";
  indexHTML += "<div class='section'>";
  indexHTML += "<div class='header'>WiFi</div>";
  indexHTML +=
      "<div class='info'>Channel: " + String(WiFi.channel()) + "</div>";
  indexHTML +=
      "<div class='info'>IP Address: " + WiFi.softAPIP().toString() + "</div>";
  indexHTML +=
      "<div class='info'>MAC Address: " + WiFi.softAPmacAddress() + "</div>";
  indexHTML +=
      "<div class='info'>SSID: " + String(WiFi.softAPSSID()) + "</div>";
  indexHTML += "</div>";

  unsigned char number_client = wifi_softap_get_station_num();
  indexHTML += "<div class='section'>";
  indexHTML += "<div class='header'>Connected Clients (" +
               String(number_client) + ")</div>";

  if (number_client > 0) {
    struct station_info* stat_info = wifi_softap_get_station_info();
    unsigned char clientCounter = 1;

    while (stat_info != NULL) {
      // Client ID
      indexHTML += "<div class='info'><strong>Client " + String(clientCounter) +
                   "</strong></div>";
      // Client IP address
      String clientIP = IPAddress(stat_info->ip.addr).toString();
      indexHTML += "<div class='info'>IP: <a href='http://" + clientIP + "'>" +
                   clientIP + "</a></div>";
      // Client MAC address
      String clientMacAddress = MACAddressToString(stat_info->bssid);
      indexHTML +=
          "<div class='info'>MAC Address: " + clientMacAddress + "</div>";
      stat_info = STAILQ_NEXT(stat_info, next);
      clientCounter++;
    }
    wifi_softap_free_station_info();
  } else {
    indexHTML += "<div class='info'>No connected clients</div>";
  }
  indexHTML += "</div>";

  indexHTML += "<div class='section'>";
  indexHTML += "<div class='header'>Access Point</div>";
  indexHTML +=
      "<div class='info'>Chip ID: " + String(ESP.getChipId()) + "</div>";
  indexHTML +=
      "<div class='info'>Flash Chip ID: " + String(ESP.getFlashChipId()) +
      "</div>";
  indexHTML +=
      "<div class='info'>Flash Chip Speed: " + String(ESP.getFlashChipSpeed()) +
      " Hz</div>";
  indexHTML += "<div class='info'>Free Heap: " + String(ESP.getFreeHeap()) +
               " bytes</div>";
  indexHTML += "</div>";
  indexHTML += "</div></body></html>";
  return indexHTML;
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

void ESPWiFi::runMillis(unsigned long m, std::function<void()> callback) {
  static unsigned long lastTime = 0;
  if (millis() - lastTime >= m) {
    lastTime = millis();
    callback();
  }
}

ESP8266WebServer* ESPWiFi::webserver() { return &webServer; }

bool ESPWiFi::isAccessPoint() { return APEnabled; }

IPAddress ESPWiFi::localIP() { return WiFi.softAPIP(); }
void ESPWiFi::handleClient() {
  webServer.handleClient();
  checkWiFi();
}
