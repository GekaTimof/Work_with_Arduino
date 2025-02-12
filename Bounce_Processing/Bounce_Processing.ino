#define BUTTON_PIN 2 
#define BOUNCE_TIME 50
#define PRESSED HIGH

volatile long int press_time = 0;
volatile bool pressed_candidate = false;
volatile long int hold_time = 0;
volatile int press_count = 0;
volatile bool report_to_user = false;
bool button_pressed = false;

volatile long int last_time_check = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), process_button_click, RISING);
}

void loop() {
  // send messege about getting candidate 
  if (report_to_user) {
    Serial.println("Press candidate");
    report_to_user = false;
  }

   // get current time
  unsigned long current_time = millis();
  
  // check candidate
  if (pressed_candidate and (current_time - last_time_check >= BOUNCE_TIME)){
    // save time
    last_time_check = current_time;

    // check button
    if (digitalRead(BUTTON_PIN) == LOW){
      // button close
      if (!button_pressed){
        Serial.println("Button is holding");
        button_pressed = true;
      }
    } else {
      // button open
      if (button_pressed){
        // it is holding
        hold_time = last_time_check - press_time;
        Serial.print("It was holding, duration - ");
        Serial.print(hold_time);
        Serial.println(" millis");
      }else{
        // it is pressing
        Serial.println("It was click");
      }

      press_count++;
      Serial.print("Press count = ");
      Serial.println(press_count);
      pressed_candidate = false;
      button_pressed = false;
    }
  }

  delay(1);
}
void process_button_click() {
  if (pressed_candidate == false) {
    press_time = millis();
    pressed_candidate = true;
    report_to_user = true;
    hold_time = 0;

    button_pressed = false;
    last_time_check = press_time;
  }
  
}