#include <Arduino.h>

const int FIRST_PIN = 2; 
const int IMG_HEIGHT = 6;
const int IMG_WIDTH = 6;
const int PINS_COUNT = IMG_HEIGHT + IMG_WIDTH;
const int PINS_ARRAY[PINS_COUNT] = {2,3,4,5,6,7,8,9,10,11,12,13};


void setup() {
  Serial.begin(9600);

  for (int i = 0; i < PINS_COUNT - 1; i++){
    pinMode(PINS_ARRAY[i], OUTPUT);
  }
}

void set_picture(int img_height, int img_widght, int img[IMG_HEIGHT][IMG_WIDTH]){
  for (int i; i < img_height; i++){
    // switch horizontal
    digitalWrite(PINS_ARRAY[i], HIGH);
    for (int j; j < img_widght; j++){
      // switch vertical
      if (img[i][j] == 1){
        digitalWrite(img_height+PINS_ARRAY[i], LOW);
      }else{
        digitalWrite(img_height+PINS_ARRAY[i], HIGH);
      }
    }
    delay(10);
    digitalWrite(PINS_ARRAY[i], LOW);
  }
  // return true;
}

void loop() {
  static int IMG[IMG_HEIGHT][IMG_WIDTH] = {{0,1,0,0,1,0},{1,1,1,1,1,1},{1,1,1,1,1,1},{0,1,1,1,1,0},{0,0,1,1,0,0},{0,0,0,0,0,0}};
  for (int i = 0; i < IMG_HEIGHT - 1; i++){
    digitalWrite(i, HIGH);
  }
  for (int i = 0; i < IMG_WIDTH - 1; i++){
    digitalWrite(i + IMG_HEIGHT, HIGH);
  }
  // set_picture(IMG_HEIGHT, IMG_WIDTH, IMG);
}