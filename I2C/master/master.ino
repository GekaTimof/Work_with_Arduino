#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join I2C bus as master no address needed
}

void loop() {

  Wire.beginTransmission(9); 
  Wire.write("Hello from master!");
  Wire.endTransmission();

  delay(1000);

  // Receive data from Arduino Board 2
  Wire.requestFrom(9, 17); 
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }

  Serial.println();
  delay(1000);
}