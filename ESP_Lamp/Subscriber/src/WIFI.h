#include <ESP8266WiFi.h>
#include <EEPROM.h>

void StartAPMode() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());
    Serial.println("AP Mode started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
}

void StartCLIMode(String ssid, String password) {
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("Connected to WiFi");
}

bool isWiFiSaved() {
    EEPROM.begin(512);
    String ssid = "";
    for (int i = 0; i < 32; ++i) {
        ssid += char(EEPROM.read(i));
    }
    return !ssid.isEmpty();
}

void saveWiFiCredentials(String ssid, String password) {
    EEPROM.begin(512);
    for (int i = 0; i < 32; ++i) {
        EEPROM.write(i, i < ssid.length() ? ssid[i] : 0);
    }
    for (int i = 32; i < 96; ++i) {
        EEPROM.write(i, i - 32 < password.length() ? password[i - 32] : 0);
    }
    EEPROM.commit();
}

void loadWiFiCredentials(String& ssid, String& password) {
    for (int i = 0; i < 32; ++i) {
        ssid += char(EEPROM.read(i));
    }
    for (int i = 32; i < 96; ++i) {
        password += char(EEPROM.read(i));
    }
}
