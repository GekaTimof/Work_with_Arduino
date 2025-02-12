#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot() {
    String html = "<form action=\"/setWiFi\" method=\"POST\">"
                  "SSID: <input type=\"text\" name=\"ssid\"><br>"
                  "Password: <input type=\"text\" name=\"password\"><br>"
                  "<input type=\"submit\" value=\"Submit\">"
                  "</form>";
    server.send(200, "text/html", html);
}

void handleSetWiFi() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    saveEEPROM(ssid, password);
    server.send(200, "text/html", "WiFi credentials saved. Restarting...");
    delay(2000);
    ESP.restart();
}

void handleNotFound() {
    server.send(404, "text/plain", "404: Not found");
}

void server_init() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/setWiFi", HTTP_POST, handleSetWiFi);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}
