#include <Arduino.h>

const int PINS_COUNT = 2;
const int PIN_NUMS[PINS_COUNT] = {2, 3};
const int ANALOG_PIN_NUMS[PINS_COUNT] = {2, 3};
const int ERROR = 10;

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

  // static int MAX = 100;
  static int ANALOG_PIN_VALUES[PINS_COUNT];

  // remembers the number of last light turned on
  static int light_togle = -1;

  // values to culculate errors (left - LEFT_ERROR)
  static int maxLeft = 0;
  static int maxRight = 0;
  int LEFT_ERROR = maxLeft - maxRight;
  if (maxLeft > maxRight){
    LEFT_ERROR += ERROR;
  } else {
    LEFT_ERROR -= ERROR;
  }


  // get analog pins values periodically 
  if (currentMillis - previousMillis >= get_data_interval){
    previousMillis = currentMillis;
    // get analog pins values and max value
    int max = -1;
    int maxId = 0;
    for (int i = 0; i < PINS_COUNT; i++){
      ANALOG_PIN_VALUES[i] = analogRead(ANALOG_PIN_NUMS[i]);
      Serial.print("ANALOG_PIN (");
      Serial.print(i, DEC);
      Serial.print(") = ");
      Serial.print(ANALOG_PIN_VALUES[i], DEC);
      Serial.print(" ");
    }
    Serial.println();

    // get max left value
    if (ANALOG_PIN_VALUES[0] > maxLeft){
      maxLeft = ANALOG_PIN_VALUES[0];
    }

    // get max right value
    if (ANALOG_PIN_VALUES[0] > maxRight){
      maxRight = ANALOG_PIN_VALUES[0];
    }

    // Chose destination
    if (ANALOG_PIN_VALUES[0] - LEFT_ERROR > ANALOG_PIN_VALUES[1]){
      maxId = 0;
    }else{
      maxId = 1;
    }

    // check finishing (value close to max and close to each other)
    if (abs(ANALOG_PIN_VALUES[0] - LEFT_ERROR - ANALOG_PIN_VALUES[1]) <= 2 * ERROR){
       for (int i = 0; i < PINS_COUNT; i++){
        digitalWrite(PIN_NUMS[i], HIGH);
       }
       light_togle = -1;
    } else {
      // make a blink
      // if light turn off
      if (light_togle == -1){
        // turn best value
        digitalWrite(PIN_NUMS[maxId], HIGH);
        light_togle = maxId;
      // if light turn on
      } else {
        // turn off all lights
        for (int i = 0; i < PINS_COUNT; i++){
          digitalWrite(PIN_NUMS[i], LOW);
        }
        light_togle = -1;
      }
    }
  }
}