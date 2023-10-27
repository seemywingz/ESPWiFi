#include "ESPWiFi.h"

String ESPWiFi::sharedHTMLStyle() {
  return "<!DOCTYPE html>"
         "<html>"
         "<head>"
         "<meta name='viewport' content='width=device-width, initial-scale=1'>"
         "<style>"
         "body{font-family: Arial, sans-serif; background-color: silver; "
         "text-align: center; margin-top: 50px;}"
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
         "</style>";
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

  String indexHTML = sharedHTMLStyle();
  indexHTML += refreshScript + "</head><body>";
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
  indexHTML += "<button onclick=\"location.href='/?autoRefresh=" +
               String((autoRefresh ? "false" : "true")) + "'\">" +
               String((autoRefresh ? "Disable" : "Enable")) +
               " Auto-Refresh</button>";
  indexHTML += "</div>";
  indexHTML += "</div></body></html>";

  return indexHTML;
}

String ESPWiFi::APIndexHTML() {
  String indexHTML = sharedHTMLStyle();
  indexHTML += "</head><body>";
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
  indexHTML += boardInfoHTML();
  indexHTML += "</div></body></html>";
  return indexHTML;
}

String ESPWiFi::boardInfoHTML() {
  String html = sharedHTMLStyle();
  html += "<div class='container'>";
  html += "<div class='section'>";
  html += "<div class='header'>ESP8266 Board Details</div>";
  html += "<ul>";
  html +=
      "<li><strong>Board Type:</strong> NodeMcu Mini Wireless D1 Module "
      "(ESP8266 ESP-12F)</li>";
  html += "<li><strong>Firmware Version:</strong> " +
          String(ESP.getCoreVersion()) + "</li>";
  html += "<li><strong>SDK Version:</strong> " + String(ESP.getSdkVersion()) +
          "</li>";
  html += "<li><strong>Flash Chip ID:</strong> " +
          String(ESP.getFlashChipId(), HEX) + "</li>";
  html += "<li><strong>Flash Chip Size:</strong> " +
          String(ESP.getFlashChipSize() / (1024 * 1024)) + " MB</li>";
  html += "<li><strong>Free Heap:</strong> " + String(ESP.getFreeHeap()) +
          " bytes</li>";
  html +=
      "<li><strong>Chip ID:</strong> " + String(ESP.getChipId(), HEX) + "</li>";
  html += "<li><strong>CPU Frequency:</strong> " + String(ESP.getCpuFreqMHz()) +
          " MHz</li>";
  html += "<li><strong>Flash Frequency:</strong> " +
          String(ESP.getFlashChipSpeed() / 1000000) + " MHz</li>";
  html += "<li><strong>Flash Mode:</strong> " + String(ESP.getFlashChipMode()) +
          "</li>";
  html += "</ul>";
  html += "</div></div>";
  return html;
}

String ESPWiFi::pageNotFoundHTML() {
  String html = sharedHTMLStyle();
  html += "</head><body>";
  html += "<div class='container'>";
  html += "<div class='section'>";
  html += "<div class='header'>Page Not Found</div>";
  html += "<div class='info'>URI: " + webServer.uri() + "</div>";
  html += "<div class='info'>Method: " +
          String(webServer.method() == HTTP_GET ? "GET" : "POST") + "</div>";
  html += "<div class='header'>Request Parameters</div>";
  html += "<ul>";
  for (uint8_t i = 0; i < webServer.args(); i++) {
    html += "<li>" + webServer.argName(i) + ": " + webServer.arg(i) + "</li>";
  }
  html += "</ul>";
  html += boardInfoHTML();
  html += "</div></body></html>";
  return html;
}