#include <Wire.h>

void setup(){
  Wire.begin(9);                
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop(){
}


void receiveEvent(int length) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
    delay(10); 
  }
}

void requestEvent(){
  Wire.write("Hello from slave!");
}