#include <Arduino.h>

const int ANALOG_PIN_NUM = 3;
const int RESPONSE_LEN = 3;
const int num = 100;

void setup() {
  Serial.begin(9600);

}

void loop() {
  String buf = "";
  if (Serial.available() > 0) {
    buf = Serial.readString();
    buf.trim();
    // send one value
    if (buf == "a"){
      String vals = ""; 

      for (int n = 0; n < num; n++){
        int value = analogRead(ANALOG_PIN_NUM);
        String resp_raw = String(value / 4); 
        String zero_add = "";

        // add zeros to responce
        for (int i = 0; i < RESPONSE_LEN - resp_raw.length(); i++){
          zero_add += "0";
        }
        String resp = zero_add + resp_raw;
        vals += resp;
        delay(1);
      }
      Serial.print(vals);

    // send array of values
    }else if (buf == "p"){
      int value = analogRead(ANALOG_PIN_NUM);
      String resp_raw = String(value / 4); 
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
