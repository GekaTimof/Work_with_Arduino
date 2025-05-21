// Pins to move
#define LEFT_DIR_PIN 7
#define LEFT_SPEED_PIN 6

#define RIGHT_DIR_PIN 4
#define RIGHT_SPEED_PIN 5

#define LEFT_SIDE_FORWARD HIGH
#define RIGHT_SIDE_FORWARD LOW

#define LEFT_SIDE_BACKWARD LOW
#define RIGHT_SIDE_BACKWARD HIGH

// Pins to get distance
#define TRIG_FRONT_PIN 2
#define ECHO_FRONT_PIN 3

#define TRIG_RIGHT_PIN 8
#define ECHO_RIGHT_PIN 9

void setup() {  
  for (int i = 4; i <=7; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  // front sensor initiation
  pinMode(TRIG_FRONT_PIN, OUTPUT);
  pinMode(ECHO_FRONT_PIN, INPUT);

  // left sensor initiation
  pinMode(TRIG_RIGHT_PIN, OUTPUT);
  pinMode(ECHO_RIGHT_PIN, INPUT);

  Serial.begin(9600);
}

float getDistance(int trigPin, int echoPin) {
  // Send impulse 10 ms
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // get signal duration
  long duration = pulseIn(echoPin, HIGH);

  // get distance in sm
  float distance = duration * 0.0343 / 2.0;
  return distance;
}

void move(bool left_dir, int speed_left, bool right_dir, int speed_right){
  digitalWrite(LEFT_DIR_PIN, left_dir);
  analogWrite(LEFT_SPEED_PIN, speed_left);

  digitalWrite(RIGHT_DIR_PIN, right_dir);
  analogWrite(RIGHT_SPEED_PIN, speed_right);
}

void go_forvard(int speed){
  move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_FORWARD, speed);
}

void go_back(int speed){
  move(LEFT_SIDE_BACKWARD, speed, RIGHT_SIDE_BACKWARD, speed);
}

void turn_left(int speed){
  move(LEFT_SIDE_BACKWARD, speed, RIGHT_SIDE_FORWARD, speed);
}

void turn_right(int speed){
  move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_BACKWARD, speed);
}

void stop(){
  move(0, 0, 0, 0);
}


float distance_front = 0.0;
int front_min = 30; 
float distance_right = 0.0;
int right_min = 20;
int right_max = 30;

int state = 1;
int counter = 0;

int step_time = 10;
int move_speed = 220;
int turn_speed = 220;

void loop() {
  if (state == 0){
    go_forvard(move_speed);
    delay(5000);
    go_back(move_speed);
    delay(5000);
    turn_left(turn_speed);
    delay(5000);
    turn_right(turn_speed);
    delay(5000);
  }

  distance_front = getDistance(TRIG_FRONT_PIN, ECHO_FRONT_PIN);
  distance_right = getDistance(TRIG_RIGHT_PIN, ECHO_RIGHT_PIN);
  // Serial.print("Front: ");
  // Serial.println(distance_front);
  // Serial.print("Right: ");
  // Serial.println(distance_right);

  // to close to right
  if (state == 1 and distance_front > front_min and distance_right < right_min){
    turn_left(turn_speed);
    state = 2;
    counter = 1;
  }
  else if (state == 2 and distance_right < right_min and counter % 5 == 0){
    go_forvard(move_speed);
    counter += 1;
  }
  else if (state == 2 and distance_right < right_min and counter % 5 > 0){
    turn_left(turn_speed);
    counter += 1;
  }
  else if (state == 2 and distance_right >= right_min){
    turn_right(turn_speed);
    state = 1;
    counter = 0;
  }


  // before wall 
  else if (state == 1 and distance_front <= front_min){
    turn_left(turn_speed);
    state = 3;
    counter = 1;
  }
  else if (state == 3 and distance_front <= front_min){
    turn_left(turn_speed);
    counter += 1;
  }
  else if (state == 3 and distance_front > front_min){
    turn_left(turn_speed);
    state = 1;
    counter = 0;
  }


  // no wall on right
  if (state == 1 and distance_front > front_min and distance_right > right_max){
    turn_right(turn_speed);
    state = 4;
    counter = 1;
  }
  else if (state == 4 and distance_right > right_max and counter % 5 == 0){
    go_forvard(move_speed);
    counter += 1;
  }
  else if (state == 4 and distance_right > right_max and counter % 5 > 0){
    turn_right(turn_speed);
    counter += 1;
  }
  else if (state == 4 and distance_right >= right_max){
    turn_left(turn_speed);
    state = 1;
    counter = 0;
  }


  // all ok and we need to move forward
  else if (state == 1){
    go_forvard(move_speed);
  } 


  // reset if we are stuck in cycle
  if (counter >= 10){
    state = 1;
    counter = 0;
  }

  delay(step_time);
}





