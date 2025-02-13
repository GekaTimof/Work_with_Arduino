#include <Arduino.h>

#define LIGHTSENSOR_GET_PIN A0
#define TRIG_PIN 4
#define ECHO_PIN 3

// checking parameters
volatile long int triger_time = 0;
bool checking = false;
const int CHECKING_TIME = 10;

// sendung data parameters
const float SPEED_OF_SOUND = 0.0343;
volatile float distance  = 0.0;
volatile int s = 0;
bool send_data = false;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echo_detecor, FALLING);
  // attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echo_detecor, RISING);
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
      triger_time = micros();
      digitalWrite(TRIG_PIN, HIGH);
      checking = true;
    }
  }

  // send data
  if (send_data) {
    int distance_fix = constrain(static_cast<int>(distance), 0, 8192);
    int s_fix = constrain(static_cast<int>(s), 0, 1023);

    char buffer[9];
    snprintf(buffer, sizeof(buffer), "%04d%04d", distance_fix, s_fix); 
    
    Serial.println(buffer);
    send_data = false;

    digitalWrite(TRIG_PIN, LOW);
    checking = false;

    // Serial.print(distance);
    // Serial.print(" - ");
    // Serial.println(s);
    // send_data = false;
  }

  // get current time
  unsigned long current_time = micros();
  // turn Triger off
  if (checking and (current_time - triger_time >= CHECKING_TIME)){
    digitalWrite(TRIG_PIN, LOW);
    checking = false;
  }
}

void echo_detecor(){
  long int echo_time = micros();
  distance  = (echo_time - triger_time) * SPEED_OF_SOUND / 2.0; 
  s = analogRead(LIGHTSENSOR_GET_PIN); // replace for registers 
  send_data = true;
}