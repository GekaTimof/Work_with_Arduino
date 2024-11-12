#include <Arduino.h>

const int PIN_NUM3 = 3;
const int PIN_NUM4 = 4;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_NUM3, OUTPUT);
  pinMode(PIN_NUM4, OUTPUT);
}

void loop() {
  String buf = "";
  if (Serial.available() > 0) {
    buf = Serial.readString();
    buf.trim();
    if (buf == "u3"){
      digitalWrite(PIN_NUM3, HIGH);
      Serial.print("led on");
    } else if (buf == "d3"){
      digitalWrite(PIN_NUM3, LOW);
      Serial.print("led off");

    } else if (buf == "u4"){
      digitalWrite(PIN_NUM4, HIGH);
      Serial.print("led on");
    } else if (buf == "d4"){
      digitalWrite(PIN_NUM4, LOW);
      Serial.print("led off");
    }
  }
}