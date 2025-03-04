#define LEFT_DIR_PIN 7
#define LEFT_SPEED_PIN 6

#define RIGHT_DIR_PIN 4
#define RIGHT_SPEED_PIN 5

#define LEFT_SIDE_FORWARD HIGH
#define RIGHT_SIDE_FORWARD LOW

#define LEFT_SIDE_BACKWARD LOW
#define RIGHT_SIDE_BACKWARD HIGH



void setup() {
  for (int i = 4; i <=7; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void move(bool left_dir, int speed_left, 
          bool right_dir, int speed_right){

  digitalWrite(LEFT_DIR_PIN, left_dir);
  analogWrite(LEFT_SPEED_PIN, speed_left);

  digitalWrite(RIGHT_DIR_PIN, right_dir);
  analogWrite(RIGHT_SPEED_PIN, speed_right);
}

void move_one_right(int speed, bool forvard = true){
  move(0, 0, RIGHT_SIDE_FORWARD * forvard, speed);
}

void move_one_left(int speed, bool forvard = true){
  move(LEFT_SIDE_FORWARD * forvard, speed, 0, 0);
}

void go_forvard(int speed){
  move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_FORWARD, speed);
}

void go_back(int speed){
  move(LEFT_SIDE_BACKWARD, speed, RIGHT_SIDE_BACKWARD, speed);
}

void turn_left(int speed, float ratio){
  move(LEFT_SIDE_FORWARD, (speed * ratio), RIGHT_SIDE_FORWARD, speed);
}

void turn_right(int speed, float ratio){
  move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_FORWARD, (speed * ratio));
}

void turn_left_onspot(int speed){
  move(LEFT_SIDE_BACKWARD, speed, RIGHT_SIDE_FORWARD, speed);
}

void turn_right_onspot(int speed){
  move(LEFT_SIDE_FORWARD, speed, RIGHT_SIDE_BACKWARD, speed);
}

void stop(){
  move(0, 0, 0, 0);
}


void loop() {
  go_forvard(255);
  // go_back(255);
  // turn_right_onspot(255);

}
















