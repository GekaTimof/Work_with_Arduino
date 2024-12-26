#include <Arduino.h>
#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


ESP8266WebServer server(80);    



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
  digitalWrite(2, LOW);

  Serial.println("Start connecting");
  

  // connect to wi-fi
  WIFI_init(false);
  
  // set web-server
  server_init();

  // make mqtt connection (create topic)
  MQTT_init();
  
  mqtt_cli.publish(topic, "create topic");

  //отправка сгенерированного топика на сервер
  HTTPClient httpClient;
  // Connect to server and try to send the message
  httpClient.begin(wifiClient,"http://copift.ru:5000/refresh");

  httpClient.addHeader("Content-Type", "application/json");
  int httpResponseCode = httpClient.POST("{\"message\":\"" + (String)topic +"\"}");

  String resultInfo;
  if (httpResponseCode > 0){
        // Serial.println(httpClient.getString());
        resultInfo = "Success, HTTP Response Code: " + (String)httpResponseCode ;
    }
    else{
        resultInfo = "Error, HTTP Response Code: " + (String)httpResponseCode ;
    }
    httpClient.end();

  // mqtt_cli.subscribe("esp8266/command");
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
        MQTT_init(); // Переподключаемся к брокеру
    }

    // Запускаем MQTT-клиент (очень важно для получения сообщений)
    mqtt_cli.loop();

    // Ваш основной код обработки сенсора и отправки данных
    static long previousMillis = 0; 
    long currentMillis = millis();

    if (currentMillis - previousMillis >= stepMillis) {
        previousMillis = currentMillis; 

        // Получаем данные с датчика освещенности
        int data = analogRead(A0);
        char payload[4]; 
        itoa(data, payload, 10);
        
        // Отправляем данные в топик через MQTT
        mqtt_cli.publish(topic, payload);

        Serial1.println("Send - " + String(payload) + " to topic - " + topic);
    }
}
