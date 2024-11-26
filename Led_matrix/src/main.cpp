#include <Arduino.h>

const int PINS_COUNT = 4;
const int EASY_EVENTS_COUNT = 8;
const int COMPLICATE_EVENTS_COUNT = 8;
const int PINS_ARRAY[PINS_COUNT] = {3, 4, 6, 7};
const int STEP_COUNT = 5;
const int EASY_EVENTS_ARRAY[EASY_EVENTS_COUNT][PINS_COUNT] = {{1,0,0,1}, {1,0,1,0}, {0,1,0,1}, {0,1,1,0}, {1,0,0,0}, {0,1,0,0}, {1,1,0,1}, {1,1,1,0}};
const int COMPLICATE_EVENTS_ARRAY[EASY_EVENTS_COUNT][PINS_COUNT] =  {{1,4,0,0}, {2,3,0,0}, {1,2,3,0}, {2,3,4,0}, {3,4,1,0}, {4,1,2,0}, {1,2,3,4}, {0,0,0,0}};


void setup() {
  Serial.begin(9600);

  for (int i = 0; i < PINS_COUNT; i++){
    pinMode(PINS_ARRAY[i], OUTPUT);
  }
}


void turnLightOFF(){
    for (int i = 0; i < PINS_COUNT/2; i++){
      digitalWrite(PINS_ARRAY[i], LOW);
    }
    for (int i = PINS_COUNT/2; i < PINS_COUNT; i++){
      digitalWrite(PINS_ARRAY[i], HIGH);
    }
}

void turnEasyLightON(const int array[PINS_COUNT]){
  for (int i = 0; i < PINS_COUNT; i++){
    if (array[i] == 1){
      digitalWrite(PINS_ARRAY[i], HIGH);
    }else{
      digitalWrite(PINS_ARRAY[i], LOW);
    }
  }
}

void turnComplecatedLightON(const int array[PINS_COUNT]){
  for (int i = 0; i < PINS_COUNT; i++){
    if (array[i] > 0){
      turnEasyLightON(EASY_EVENTS_ARRAY[array[i]-1]);
    }
    delay(2);
    turnLightOFF();
  }
}


const int test_interval = 1500;
const int blink_interval = 5;

void loop() {
  // contain previous time
  static unsigned long previousMillis = 0; 
  // contain previous blink time
  static unsigned long previousBlinkMillis = 0; 
  // current time
  unsigned long currentMillis = millis();

  // tests counter
  static int counter = 0;


  if (currentMillis - previousMillis >= test_interval){
    previousMillis = currentMillis;
    
    if (counter < EASY_EVENTS_COUNT + COMPLICATE_EVENTS_COUNT){
    counter++;
    }else{
      counter = 0;
    }

    // turn all pist off
    turnLightOFF();
  }

  if (currentMillis - previousBlinkMillis >= blink_interval){
    if (counter < EASY_EVENTS_COUNT){
      turnEasyLightON(EASY_EVENTS_ARRAY[counter]);
    }else if (counter < EASY_EVENTS_COUNT + COMPLICATE_EVENTS_COUNT){
      turnComplecatedLightON(COMPLICATE_EVENTS_ARRAY[counter - EASY_EVENTS_COUNT]);
    }
    previousBlinkMillis = currentMillis;
  }
}

