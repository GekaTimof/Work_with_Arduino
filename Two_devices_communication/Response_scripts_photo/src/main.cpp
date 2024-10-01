#include <Arduino.h>

const int ANALOG_PIN_NUM = 3;
const int RESPONSE_LEN = 4;

void setup() {
  Serial.begin(9600);

}

void loop() {
  String buf = "";
  if (Serial.available() > 0) {
    buf = Serial.readString();
    buf.trim();
    if (buf == "p"){
      int value = analogRead(ANALOG_PIN_NUM);
      String resp_raw = String(value); 
      String zero_add = "";
      // add zeros to responce
      for (int i = 0; i < RESPONSE_LEN - resp_raw.length(); i++){
        zero_add += "0";
      }
      String resp = zero_add + resp_raw;
      Serial.print(resp);
    }
  }
}
