#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <EEPROM.h>

ESP8266WiFiMulti wifiMulti;
WiFiClient wifiClient;

String ip = "(IP unset)";
String ssidCLI;
String passwordCLI;

void readEEPROM() {
    EEPROM.begin(512);
    char ssid[32], password[32];
    for (int i = 0; i < 32; i++) {
        ssid[i] = char(EEPROM.read(i));
        password[i] = char(EEPROM.read(32 + i));
    }
    ssidCLI = String(ssid);
    passwordCLI = String(password);
    EEPROM.end();
}

void saveEEPROM(String ssid, String password) {
    EEPROM.begin(512);
    for (int i = 0; i < 32; i++) {
        EEPROM.write(i, ssid[i]);
        EEPROM.write(32 + i, password[i]);
    }
    EEPROM.commit();
    EEPROM.end();
}

bool StartAPMode() {
    IPAddress apIP(192, 168, 4, 1);
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP((ssidAP + " " + id()).c_str(), passwordAP.c_str());
    return true;
}

void StartCLIMode() {
    wifiMulti.addAP(ssidCLI.c_str(), passwordCLI.c_str());
    while (wifiMulti.run() != WL_CONNECTED) {
        delay(500);
    }
}

void WIFI_init(bool mode_ap) {
    if (mode_ap) {
        StartAPMode();
        ip = WiFi.softAPIP().toString();
    } else {
        readEEPROM();
        StartCLIMode();
        ip = WiFi.localIP().toString();
    }
    Serial.println("IP address: ");
    Serial.println(ip);
}
