#include "Config.h"
#include "WIFI.h"
#include "Server.h"
#include "MQTT.h"

bool isAPMode = true;

void setup() {
    Serial.begin(115200);
    pinMode(led, OUTPUT);

    readEEPROM();
    if (ssidCLI.isEmpty() || passwordCLI.isEmpty()) {
        isAPMode = true;
    } else {
        isAPMode = false;
    }

    WIFI_init(isAPMode);
    if (isAPMode) {
        server_init();
    } else {
        MQTT_init();
    }
}

void loop() {
    if (isAPMode) {
        server.handleClient();
    } else {
        mqtt_cli.loop();
        if (WiFi.status() != WL_CONNECTED) {
            isAPMode = true;
            StartAPMode();
            server_init();
        }
    }
}
