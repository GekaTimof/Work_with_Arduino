#include <Arduino.h>

#include "Config.h"
#include "WIFI.h"
// #include "Server.h"
// #include "MQTT.h"
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);    

void handleRoot() {                         
  server.send(200, 
              "text/html", 
              "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void handleLED() {                          
  digitalWrite(led, !digitalRead(led));
  server.sendHeader("Location","/"); // redirection to keep button on the screen
  server.send(303);
}

void handleSENSOR() {                          
  int data = analogRead(A0);
  //server.sendHeader("Location","/");
  server.send(200, "text/html", String(data));
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); 
}

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
  WIFI_init(true);
  
  server_init();
  // MQTT_init();
  pinMode(led, OUTPUT);
  // mqtt_cli.publish("esp8266/state", "hello emqx");
  // mqtt_cli.subscribe("esp8266/command");
}

void loop(void){
  server.handleClient();                   
  // mqtt_cli.loop();
}