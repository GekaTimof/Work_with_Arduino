#ifndef CONFIG_H
#define CONFIG_H

String ssidAP = "WiFi_Lamp"; // имя контроллера и точки доступа
String passwordAP = "12301230"; // пароль точки доступа

char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
String topic_str = "Lamp/1234/turn";

const int led = 2;
const int stepMillis = 1000;

#endif