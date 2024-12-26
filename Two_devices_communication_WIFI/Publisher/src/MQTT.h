#include <PubSubClient.h>
#include "Config.h"

PubSubClient mqtt_cli(wifiClient);


void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        message+=(char)payload[i];
    }
    int res=message.toInt();
    Serial.println(res);
    Serial.println("--------------------");
  
    if (res < 100) {
      digitalWrite(2, HIGH); 
    }
    else {
      digitalWrite(2, LOW);
    }
}


void MQTT_init() {
    Serial.println("Connecting to broker:");
    mqtt_cli.setServer(mqtt_broker, mqtt_port);
    mqtt_cli.setCallback(callback);

    // Логика подключения с ограничением попыток
    String client_id = "esp8266-" + String(WiFi.macAddress());
    int attempt = 0;

    while (!mqtt_cli.connected() && attempt < 5) {
        attempt++;
        Serial.print("Attempt ");
        Serial.print(attempt);
        Serial.println(": Connecting to MQTT broker...");

        if (mqtt_cli.connect(client_id.c_str())) {
            Serial.println("MQTT Connected");

            // Подписываемся на топик после успешного подключения
            mqtt_cli.subscribe(topic);
        } else {
            Serial.print("Failed to connect. State: ");
            Serial.println(mqtt_cli.state());
            delay(2000); // Задержка перед новой попыткой
        }
    }

    if (!mqtt_cli.connected()) {
        Serial.println("Failed to connect to MQTT broker after 5 attempts. Restarting...");
        ESP.restart(); // Перезагружаем устройство
    }
}
