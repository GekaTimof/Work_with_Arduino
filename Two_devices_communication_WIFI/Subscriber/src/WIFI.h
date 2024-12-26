#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>


ESP8266WiFiMulti wifiMulti;    
WiFiClient wifiClient;

String ip = "(IP unset)"; 

String id(){
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  return macID;
}

bool StartAPMode() {
  IPAddress apIP(192, 168, 4, 1);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP((ssidAP + "_" + id()).c_str(), passwordAP.c_str());
  Serial.println("");
  Serial.println("WiFi up in AP mode with name: "+ ssidAP + "_" + id());
  return true;
}

void StartCLIMode() {
  wifiMulti.addAP(ssidCLI, passwordCLI);
  Serial.print("Connecting to wi-fi:");
  
  //wait for connect to wi-fi
  while(wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.println("wait for connection");
  }
   Serial.println("");
   Serial.println("Connection success");
}

void WIFI_init(bool mode_ap) {
    if (mode_ap) {
      StartAPMode();
      ip = WiFi.softAPIP().toString();
    } else {
      StartCLIMode();
      ip = WiFi.localIP().toString();
    }
    Serial.print("IP address: - ");
    Serial.println(ip);  
}