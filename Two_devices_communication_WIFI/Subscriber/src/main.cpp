#include <Arduino.h>
#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


ESP8266WebServer server(80);    

String topic_str = "";

void getTopicFromServer() {
  HTTPClient httpClient;
  httpClient.begin(wifiClient, "http://copift.ru:5000/get_id");
  
  // Send HTTP GET request
  int httpResponseCode = httpClient.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = httpClient.getString();
    Serial.println(payload);
    int a=payload.indexOf(":")+2;
    int b=payload.indexOf('"',a);
    topic_str=payload.substring(a,b);

    Serial.println("Parsed topic: " + topic_str);
  }else{
    Serial.print("Error on HTTP request. Code: ");
    Serial.println(httpResponseCode);
  }
  httpClient.end();
}

// HTML form to switch led (send us ti "/LED")
void handleRoot() {                         
  server.send(200, 
              "text/html", 
              "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

// switch led and return us to "/"
void handleLED() {                          
  digitalWrite(led, !digitalRead(led));
  server.sendHeader("Location","/"); // redirection to keep button on the screen
  server.send(303);
}

// red signal from A0
void handleSENSOR() {                          
  int data = analogRead(A0);
  //server.sendHeader("Location","/");
  server.send(200, "text/html", String(data));
}

// Error page
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); 
}

// server inisialisation
void server_init() {
  server.on("/", HTTP_GET, handleRoot);     
  server.on("/LED", HTTP_POST, handleLED);  
  server.on("/SENSOR", HTTP_GET, handleSENSOR);  
  server.onNotFound(handleNotFound);        

  server.begin();                          
  Serial.println("HTTP server started");    
}

void setup(void){
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  Serial.println("Start connecting");
  

  // connect to wi-fi
  WIFI_init(false);
  
  // set web-server
  server_init();

  // get topic
  getTopicFromServer();

  // make mqtt connection (create topic)
  const char* topic = topic_str.c_str(); 
  MQTT_init(topic);
  
  Serial.println("Finish connecting");
}


void loop() {
    // Проверка подключения к Wi-Fi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi disconnected. Reconnecting...");
        WIFI_init(false); // Переподключаемся к Wi-Fi
    }

    // Проверка подключения к MQTT брокеру
    if (!mqtt_cli.connected()) {
        Serial.println("MQTT disconnected. Reconnecting...");
        const char* topic = topic_str.c_str(); 
        MQTT_init(topic); // Переподключаемся к брокеру
    }

    if (topic_str.isEmpty()) {
        mqtt_cli.subscribe(topic_str.c_str());
        Serial.println("Resubscribed to topic.");
    }

    // Запускаем MQTT-клиент (очень важно для получения сообщений)
    mqtt_cli.loop();

}
