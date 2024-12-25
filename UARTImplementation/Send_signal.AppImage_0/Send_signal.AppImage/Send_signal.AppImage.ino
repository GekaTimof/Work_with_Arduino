void setup() {
    Serial.begin(9600); 
}

void loop() {
    Serial.write(0x55); 
    delay(1000);
}
