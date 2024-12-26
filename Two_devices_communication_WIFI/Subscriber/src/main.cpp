#include <Arduino.h>
#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


ESP8266WebServer server(80);    
// String topic_str = id() + mqtt_topic; 
String topic_str = mqtt_topic; 
const char* topic = topic_str.c_str(); 


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
  Serial.begin(115200);
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

  // HTTPClient httpClient;
  // // Connect to server and try to send the message
  // httpClient.begin(topic, "http://copift.ru:5000/refresh");

  // mqtt_cli.subscribe("esp8266/command");
  Serial.println("Finish connecting");
}

void loop(){
  // server.handleClient();                   
  // mqtt_cli.loop();

  Serial1.println("=====");

  static long previousMillis = 0; 
  long currentMillis = millis();

  delay(100);
  Serial1.println("" + (currentMillis - previousMillis));

  if (currentMillis - previousMillis >= stepMillis) {
    previousMillis = currentMillis; 

    // get data from sensor
    int data = analogRead(A0);
    char payload[4]; 
    itoa(data, payload, 10);
    
    // Sending data to topic
    mqtt_cli.publish(topic, payload);

    Serial1.println("Send - " + String(payload) + " to topic - " + topic);
  }


}