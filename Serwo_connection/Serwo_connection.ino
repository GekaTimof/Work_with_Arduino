#include <Servo.h>

Servo myServo;  // create a servo object

void setup() {
  myServo.attach(9);   // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);  // open a serial connection to your computer
}

int I = 0;
const int I_max = 18;
const int POS_ARRAY[I_max] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 179};

void loop() {
  myServo.write(POS_ARRAY[I]);
  delay(200);

  I++;
  if (I >= I_max){
    I = 0;
  }

}
