#include <Arduino.h>

const int PIN_NUM = 3;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_NUM, OUTPUT);
}

void loop() {
  String buf = "";
  if (Serial.available() > 0) {
    buf = Serial.readString();
    buf.trim();
    if (buf == "u"){
      digitalWrite(PIN_NUM, HIGH);
      Serial.print("led on");
    } else if (buf == "d"){
      digitalWrite(PIN_NUM, LOW);
      Serial.print("led off");
    }
  }
}