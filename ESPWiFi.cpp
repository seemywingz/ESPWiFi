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

void ESPWiFi::Start() {
  if (loadWiFiCredentials()) {
    Serial.println("Found WiFi credentials. Connecting to WiFi...");
    startAsClient();  // Use the saved credentials to start as a client
  } else {
    Serial.println("No WiFi credentials found. Starting in AP mode...");
    startAsAccessPoint();  // No credentials found, start in AP mode
  }
}

void ESPWiFi::startAsClient() {
  APEnabled = false;
  connectToWiFi();
  startWebServer();
  initializeMDNS();
}

void ESPWiFi::startAsAccessPoint() {
  APEnabled = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, password);
  startWebServer();
  initializeMDNS();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void ESPWiFi::startWebServer() {
  if (!LittleFS.begin()) {
    Serial.println("An error occurred while mounting LittleFS");
  }

  webServer.on("/", HTTP_GET, [this]() {
    File file = LittleFS.open("/index.html", "r");
    if (file) {
      webServer.streamFile(file, "text/html");
      file.close();
    } else if (APEnabled) {
      webServer.send(200, "text/html", APIndexHTML());
    } else {
      webServer.send(200, "text/html", clientIndexHTML());
    }
  });

  webServer.on("/boardinfo", HTTP_GET,
               [this]() { webServer.send(200, "text/html", boardInfoHTML()); });

  webServer.on("/setup", HTTP_GET,
               [this]() { webServer.send(200, "text/html", setupPageHTML()); });

  webServer.on("/save", HTTP_POST, [this]() {
    // Save the received credentials
    String ssid = webServer.arg("ssid");
    String password = webServer.arg("password");
    saveWiFiCredentials(
        ssid, password);  // Implement this function to save the credentials
    webServer.send(200, "text/html",
                   "<p>Credentials saved. The module will now restart.</p>");
    delay(1000);
    ESP.restart();
  });

  webServer.on("/erase", HTTP_GET, [this]() {
    webServer.send(200, "text/html", eraseCredentialsPageHTML());
  });

  webServer.on("/erase", HTTP_POST, [this]() {
    clearWiFiCredentials();
    webServer.send(
        200, "text/html",
        "<p>WiFi settings have been erased. Device will restart.</p>");
    // Optionally, you can also restart the ESP automatically
    ESP.restart();
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
      webServer.send(200, "text/html", pageNotFoundHTML());
    }
  });

  webServer.begin();
}

void ESPWiFi::connectToWiFi() {
  // WiFi.mode(WIFI_STA);
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
  ip = WiFi.localIP();
  gateway = WiFi.gatewayIP();
  subnet = WiFi.subnetMask();
  analogWrite(LED_BUILTIN, LOW);
}

String ESPWiFi::getContentType(String filename) {
  if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  // Add more MIME types here as needed
  return "text/plain";
}

String ESPWiFi::MACAddressToString(uint8_t* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1],
           mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void ESPWiFi::initializeMDNS() {
  if (mDNSEnabled) {
    if (!MDNS.begin(domain)) {
      Serial.println("Error setting up MDNS responder!");
      while (1) {
        delay(1000);
      }
    } else {
      MDNS.addService("http", "tcp", 80);
    }
  }
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
  if (mDNSEnabled) {
    MDNS.update();
  }
  checkWiFi();
}

void ESPWiFi::enableMDNS(String domainName) {
  domain = domainName;
  mDNSEnabled = true;
}
IPAddress ESPWiFi::localIP() { return WiFi.softAPIP(); }
// ESP8266WebServer* ESPWiFi::webserver() { return &webServer; }

void ESPWiFi::saveWiFiCredentials(const String& ssid, const String& password) {
  File credFile = LittleFS.open(wifiCredentialsPath, "w");
  if (!credFile) {
    Serial.println("Failed to open credentials file for writing");
    return;
  }
  credFile.println(ssid);
  credFile.println(password);
  credFile.close();
}

bool ESPWiFi::loadWiFiCredentials() {
  if (!LittleFS.begin()) {
    Serial.println("An error occurred while mounting LittleFS");
    return false;
  }

  File credFile = LittleFS.open(wifiCredentialsPath, "r");
  if (!credFile) {
    Serial.println("Credentials file not found");
    return false;
  }

  String ssid = credFile.readStringUntil('\n');
  String password = credFile.readStringUntil('\n');
  credFile.close();

  ssid.trim();
  password.trim();

  if (ssid.length() > 0 && password.length() > 0) {
    this->ssid = ssid;
    this->password = password;
    return true;
  }

  return false;
}

void ESPWiFi::clearWiFiCredentials() {
  // Use the file system (LittleFS or SPIFFS) to remove the credentials
  if (LittleFS.begin()) {
    LittleFS.remove(wifiCredentialsPath);
    LittleFS.end();
  }
}