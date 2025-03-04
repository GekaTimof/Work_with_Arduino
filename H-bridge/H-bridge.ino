#define DIR_FORWARD 4
#define DIR_BACKWARD 6
#define SPEED_PIN 5


void setup() {
  pinMode(DIR_FORWARD, OUTPUT);
  digitalWrite(DIR_FORWARD, LOW);

  pinMode(DIR_BACKWARD, OUTPUT);
  digitalWrite(DIR_BACKWARD, LOW);

  pinMode(SPEED_PIN, OUTPUT);
  digitalWrite(SPEED_PIN, 0);

  Serial.begin(9600);
}

void move(int dest=0, int speed=0){
  speed = constrain(speed, 0, 255);

  switch (dest) {
    // move forward
    case 1:
      digitalWrite(DIR_BACKWARD, LOW);
      digitalWrite(DIR_FORWARD, HIGH);
      break;

    // move backward
    case 2:
      digitalWrite(DIR_FORWARD, LOW);
      digitalWrite(DIR_BACKWARD, HIGH);
      break;

    // stop moving
    default:
      digitalWrite(DIR_FORWARD, LOW);
      digitalWrite(DIR_BACKWARD, LOW);
      speed = 0;
  }
  analogWrite(SPEED_PIN, speed);
}


void loop() {
  // move forward 100
  move(1, 100);
  delay(5000);
  // move backward 100
  move(2, 100);
  delay(5000);
  // stop moving
  move(0);
  delay(2000);


  // move forward 220
  move(1, 220);
  delay(5000);
  // move backward 220
  move(2, 220);
  delay(5000);
  // stop moving
  move(0);
  delay(2000);
}





