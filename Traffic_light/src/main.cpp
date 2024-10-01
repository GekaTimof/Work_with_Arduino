#include <Arduino.h>

const int PINS_COUNT = 4;
const int PINS_ARRAY[PINS_COUNT] = {7, 6, 5, 6};
const int STEP_COUNT = 5;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < PINS_COUNT - 1; i++){
    pinMode(PINS_ARRAY[i], OUTPUT);
  }
}

int step = 1;
int last_step = STEP_COUNT;
int pin = 0;
int last_pin = PINS_COUNT-1;
int control_type = 1;
int index=0;
bool blink_togle = false;
// time intervals (blink time, green, blink green, yellow, red, yellow)
long intervals[] = {250, 7000, 2000, 2000, 10000, 2000};
// time imterval for hand running type
int hand_interval = 60000;
// time interval for blinkung in hand control type 
int hand_blink = 2000;
unsigned long handPeriodMillis = 0;

// change type animation
void change_control_type(){
  // turn all pins off 
  for (int i = 0; i < PINS_COUNT - 1; i++){
    digitalWrite(PINS_ARRAY[i], LOW);
  }
}

void loop() {
  // hand togle
  static int hand_togle = -1;

  String buf = "";
  if (Serial.available() > 0) {
    buf = Serial.readString();
    buf.trim();
    if (buf == "t1"){
      control_type = 1;
      buf = "";
      blink_togle = false;
      change_control_type();
    }else if (buf == "t2"){
      control_type = 2;
      buf = "";
      blink_togle = false;
      change_control_type();
      hand_togle = -1;
      handPeriodMillis = millis();
    }
    Serial.write(" get data \n");
  }

  if (control_type == 1){
    // contain previous time
    static unsigned long previousMillis = 0; 
    // contain previous blink time
    static unsigned long previousMillisBlink = 0; 
    // current time
    unsigned long currentMillis = millis();

    // if blink interval finished
    if((last_step == 2) and (currentMillis - previousMillisBlink >= intervals[0])){
      previousMillisBlink = currentMillis;
      if (blink_togle){
        digitalWrite(PINS_ARRAY[last_pin], HIGH);
        blink_togle = false;
      }else{
        digitalWrite(PINS_ARRAY[last_pin], LOW);
        blink_togle = true;
      }
    }

    // if interval finished
    if (currentMillis - previousMillis >= intervals[last_step]) {
      previousMillis = currentMillis; 

      if (not(last_step == 1)){
        digitalWrite(PINS_ARRAY[last_pin], LOW);
        digitalWrite(PINS_ARRAY[pin], HIGH);

        last_pin = pin;
        pin++;
   
        if (pin >= PINS_COUNT){
          pin = 0;
        }
      }

      last_step = step;
      step++;
      if (step > STEP_COUNT ){
        step = 1;
      }
    }
  }else if (control_type == 2){
    // current time
    unsigned long currentMillis = millis();
    
    // if blink interval finished
    if(currentMillis - handPeriodMillis < hand_interval){
      // try to convert baf to Int
      if (buf != "" and buf.toInt()){
        int hand_step = buf.toInt();

        // if pin exist
        if (hand_step == 1 or hand_step == 2){
          if (hand_step == 1){
            hand_step = 0;
          }
          // in no light active
          if (hand_togle == -1){  
            digitalWrite(PINS_ARRAY[hand_step], HIGH);  
            hand_togle = hand_step;
          // if ve need to change light  
          } else if (hand_togle != hand_step){
            digitalWrite(PINS_ARRAY[hand_togle], LOW); 

            // blinks parametrs
            long startMillisBlink = millis();
            long periodMillisBlink = 0;
            long lastBlink = 0;
            // blinking during blinking time
            while (periodMillisBlink - startMillisBlink <= hand_blink){
              periodMillisBlink = millis();
              // make a blink at the right interval
              if (periodMillisBlink - lastBlink > intervals[0]){
                if (blink_togle){
                  digitalWrite(PINS_ARRAY[1], LOW);
                  blink_togle = false;
                }else{
                  digitalWrite(PINS_ARRAY[1], HIGH);
                  blink_togle = true;
                }
                lastBlink = periodMillisBlink;
              }
            }
            // turn center pin off
            digitalWrite(PINS_ARRAY[1], LOW);
            // turn required pin on
            digitalWrite(PINS_ARRAY[hand_step], HIGH);
            // remember turned pin
            hand_togle = hand_step;
          }
        }
      }
    }else{
      control_type = 1;
    }
  }
}
