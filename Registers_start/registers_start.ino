void setup() {
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  // Value to compare with
  TCCR1B |= (1 << WGM12);
  // for 256 prescaler
  TCCR1B = TCCR1B | (1 << CS12);  //bitSet(TCCR1B, CS12);
  // for 1024 prescaler
  //TCCR1B = TCCR1B | ((1 << CS12) | (1 << CS10));
  // timer overflow interrupt
  TIMSK1 |= (1 << OCIE1A);
  // pinMode(LED_BUILTIN, OUTPUT);
  DDRD |= (1 << DDD2);
  sei();
}

ISR(TIMER1_COMPA_vect) {
  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  PORTD = PORTD ^ (1 << PORTD2 );


}

void loop() {
}