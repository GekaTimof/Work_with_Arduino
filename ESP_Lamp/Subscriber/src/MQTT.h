#include <PubSubClient.h>
#include "Config.h"

WiFiClient wifiClient;
PubSubClient mqtt_cli(wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    if (message == "ON") {
        digitalWrite(led, HIGH);
    } else if (message == "OFF") {
        digitalWrite(led, LOW);
    }
}

void MQTT_init(const char* topic) {
    mqtt_cli.setServer(mqtt_broker, mqtt_port);
    mqtt_cli.setCallback(callback);

    while (!mqtt_cli.connected()) {
        String client_id = "ESP8266Client-" + String(WiFi.macAddress());
        if (mqtt_cli.connect(client_id.c_str())) {
            mqtt_cli.subscribe(topic);
        } else {
            delay(2000);
        }
    }
}
