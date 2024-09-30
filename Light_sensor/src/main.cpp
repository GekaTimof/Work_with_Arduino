#include <Arduino.h>

const int PINS_COUNT = 2;
const int PIN_NUMS[PINS_COUNT] = {2, 3};
const int ANALOG_PIN_NUMS[PINS_COUNT] = {2, 3};

const int get_data_interval = 500;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < PINS_COUNT; i++){
    pinMode(PIN_NUMS[i], OUTPUT);
  }
}


void loop() {
  // contain previous time
  static unsigned long previousMillis = 0; 
  // current time
  unsigned long currentMillis = millis();

  static int ANALOG_PIN_VALUES[PINS_COUNT];

  static int togle = 1;
  if (togle){
    for (int i = 0; i < PINS_COUNT; i++){
      ANALOG_PIN_VALUES[i] = analogRead(ANALOG_PIN_NUMS[i]);
      Serial.print("ANALOG_PIN (");
      Serial.print(i, DEC);
      Serial.print(") = ");
      Serial.print(ANALOG_PIN_VALUES[i], DEC);
      Serial.print(" ");
    }
    Serial.println();
    togle = 0;
  }

  // get analog pins values periodically 
  if (currentMillis - previousMillis >= get_data_interval){
    previousMillis = currentMillis;

    for (int i = 0; i < PINS_COUNT; i++){
      ANALOG_PIN_VALUES[i] = analogRead(ANALOG_PIN_NUMS[i]);
      Serial.print("ANALOG_PIN (");
      Serial.print(i, DEC);
      Serial.print(") = ");
      Serial.print(ANALOG_PIN_VALUES[i], DEC);
      Serial.print(" ");
    }
    Serial.println();
    
  }
}