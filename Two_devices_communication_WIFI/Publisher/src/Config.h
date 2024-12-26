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
// String topic_str = id() + mqtt_topic; 
String topic_str = mqtt_topic; 
const char* topic = topic_str.c_str(); 

const int led = 2;

const int stepMillis = 1000;


// // WiFi settings
// #define wifi_ssid     "HONOR30"
// #define wifi_pass     "12301230"

// // MQTT
// #define mqtt_server   "35.172.255.228"
// #define mqtt_port     1883
// #define mqtt_login    ""
// #define mqtt_pass     ""


#endif