#include <Arduino.h>

#define LIGHTSENSOR_GET_PIN A0
#define TRIG_PIN 3
#define ECHO_PIN 4

volatile long int triger_time = 0;
bool checking = false;
const int CHECKING_TIME = 10;
const float SPEED_OF_SOUND = 0.343;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echo_detecor, RISING);
  Serial.begin(9600); 
}

void loop() {
  // serial listener 
  String buf = "";
  if (Serial.available() > 0) {
    buf = Serial.readString();
    buf.trim();
    if (buf == "g"){
      // turn Triger on
      triger_time = millis();
      digitalWrite(TRIG_PIN, HIGH);
      checking = true;
    }
  }

  // get current time
  unsigned long current_time = millis();
  if (checking and (current_time - triger_time >= CHECKING_TIME)){
     // turn Triger off
    digitalWrite(TRIG_PIN, LOW);
    checking = false;
  }

  // get data from light sensor 
  // float s = analogRead(LIGHTSENSOR_GET_PIN);
  // Serial.println(s);

}

void echo_detecor(){
  long int echo_time = millis();
  int distance  = (echo_time - triger_time) * SPEED_OF_SOUND / 2;
  int s = analogRead(LIGHTSENSOR_GET_PIN);
  Serial.print(distance);
  Serial.print(" - ");
  Serial.println(s);
}