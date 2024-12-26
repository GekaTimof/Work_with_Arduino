#ifndef CONFIG_H
#define CONFIG_H

String ssidAP = "PublisherAP"; // имя контроллера и точки доступа
String passwordAP = "12301230"; // пароль точки доступа

//char* ssidCLI = "YOUR_SSID"; // имя контроллера и точки доступа
//char* passwordCLI = "YOUR_PASS"; // пароль точки доступа

char* ssidCLI = "HONOR_30";
char* passwordCLI = "12301230";

char* mqtt_broker = "broker.emqx.io";

const int mqtt_port = 1883;

const String mqtt_topic = "Sensor/data";


const int led = 2;

const int stepMillis = 1000;

#endif