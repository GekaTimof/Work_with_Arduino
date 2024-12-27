#include <Arduino.h>
#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
bool apMode = true;

void handleRoot() {
    String html = "<form action=\"/setWiFi\" method=\"POST\">"
                  "SSID: <input type=\"text\" name=\"ssid\"><br>"
                  "Password: <input type=\"password\" name=\"password\"><br>"
                  "<input type=\"submit\" value=\"Save\"></form>";
    server.send(200, "text/html", html);
}

void handleSetWiFi() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    if (!ssid.isEmpty() && !password.isEmpty()) {
        saveWiFiCredentials(ssid, password);
        server.send(200, "text/plain", "WiFi credentials saved. Rebooting...");
        delay(1000);
        ESP.restart();
    } else {
        server.send(400, "text/plain", "Invalid input.");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(led, OUTPUT);

    String ssid, password;
    if (isWiFiSaved()) {
        loadWiFiCredentials(ssid, password);
        StartCLIMode(ssid, password);
        apMode = false;
    } else {
        StartAPMode();
    }

    server.on("/", HTTP_GET, handleRoot);
    server.on("/setWiFi", HTTP_POST, handleSetWiFi);
    server.begin();

    if (!apMode) {
        MQTT_init(topic_str.c_str());
    }
}

void loop() {
    if (apMode) {
        server.handleClient();
    } else {
        mqtt_cli.loop();
    }
}
