const int IMG_HEIGHT = 6;
const int IMG_WIDTH = 6;

// output pins
const uint8_t DDRD_ARRAY[IMG_HEIGHT] = {DDD2, DDD3, DDD4, DDD5, DDD6, DDD7}; // 2-7
const uint8_t DDRB_ARRAY[IMG_WIDTH] = {DDB0, DDB1, DDB2, DDB3, DDB4, DDB5}; // 8-13

// write pins
const uint8_t PORTD_ARRAY[IMG_HEIGHT] = {PORTD2, PORTD3, PORTD4, PORTD5, PORTD6, PORTD7}; // 2-7
const uint8_t PORTB_ARRAY[IMG_WIDTH] = {PORTB0, PORTB1, PORTB2, PORTB3, PORTB4, PORTB5}; // 8-13


void setup() {
  // turn global stopping off (to set timer safely)
  cli();
  
  // clearing registers
  TCCR1A = 0;
  TCCR1B = 0;

  // set counter start value 
  TCNT1 = 0;
  // set timer interval
  OCR1A = 195;
  TCCR1B |= (1 << WGM12);

  // for 256 prescaler
  TCCR1B = TCCR1B | (1 << CS12);  //bitSet(TCCR1B, CS12);
  // for 1024 prescaler
  //TCCR1B = TCCR1B | ((1 << CS12) | (1 << CS10));

  // set restarting timer when it match with OCR1A
  TIMSK1 |= (1 << OCIE1A);

  // set pin (2-7) output
  for (int i = 0; i < IMG_HEIGHT; i++){
    DDRD |= (1 << DDRD_ARRAY[i]);   
  }

  // set pin (8-13) output
  for (int i = 0; i < IMG_WIDTH; i++){
    DDRB |= (1 << DDRB_ARRAY[i]);   
  }

  // turn global stopping off
  sei();
}


int img_n = 0;
const int img_n_max = 11;
int step = 0;
const int duration = 10;

const int ALL_IMG[img_n_max][IMG_HEIGHT][IMG_WIDTH] = {
  // 1
  {{0,1,0,0,0,1},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {0,0,0,0,1,0},
  {0,0,0,0,0,1},
  {0,0,0,0,0,0}},
  // 2
  {{1,0,0,0,1,0},
  {0,1,0,0,0,1},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {0,0,0,0,1,0},
  {0,0,0,0,0,1}},
  // 3
  {{0,0,0,1,0,0},
  {1,0,0,0,1,0},
  {0,1,0,0,0,1},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {0,0,0,0,1,0}},
  // 4
  {{0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {1,0,0,0,1,0},
  {0,1,0,0,0,1},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0}},
  // 5
  {{0,1,0,0,0,0},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {1,0,0,0,1,0},
  {0,1,0,0,0,1},
  {0,0,1,0,0,0}},
  // 6
  {{1,0,0,0,0,0},
  {0,1,0,0,0,0},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {1,0,0,0,1,0},
  {0,1,0,0,0,1}},
  // 7
  {{0,0,0,0,0,0},
  {1,0,0,0,0,0},
  {0,1,0,0,0,0},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {1,0,0,0,1,0}},
  // 8
  {{0,0,0,0,0,1},
  {0,0,0,0,0,0},
  {1,0,0,0,0,0},
  {0,1,0,0,0,0},
  {0,0,1,0,0,0},
  {0,0,0,1,0,0}},
  // 9
  {{0,0,0,0,1,0},
  {0,0,0,0,0,1},
  {0,0,0,0,0,0},
  {1,0,0,0,0,0},
  {0,1,0,0,0,0},
  {0,0,1,0,0,0}},
  // 10
  {{0,0,0,1,0,0},
  {0,0,0,0,1,0},
  {0,0,0,0,0,1},
  {0,0,0,0,0,0},
  {1,0,0,0,0,0},
  {0,1,0,0,0,0}},
  // 11
  {{0,0,1,0,0,0},
  {0,0,0,1,0,0},
  {0,0,0,0,1,0},
  {0,0,0,0,0,1},
  {0,0,0,0,0,0},
  {1,0,0,0,0,0}}
  };


ISR(TIMER1_COMPA_vect) {

  // turn pin up or down
  for (int i = 0; i < IMG_HEIGHT; i++){
    // switch horizontal
    // digitalWrite(PINS_ARRAY[i], HIGH);
    PORTD = PORTD | (1 << PORTD_ARRAY[i]);
    
    for (int j = 0; j < IMG_WIDTH; j++){
      // switch vertical
      if (ALL_IMG[img_n][i][j] == 1){
        // digitalWrite(PINS_ARRAY[j+IMG_HEIGHT], LOW);
        PORTB = PORTB & ~(1 << PORTB_ARRAY[j]);
      }else{
        // digitalWrite(PINS_ARRAY[j+IMG_HEIGHT], HIGH);
        PORTB = PORTB | (1 << PORTB_ARRAY[j]);
      }
    }
  
    delay(1);

    // digitalWrite(PINS_ARRAY[i], LOW);
    PORTD = PORTD & ~(1 << PORTD_ARRAY[i]);

    for (int j = 0; j < IMG_WIDTH; j++){
      // digitalWrite(PINS_ARRAY[j+IMG_HEIGHT], HIGH);
      PORTB = PORTB | (1 << PORTB_ARRAY[j]);
    }
  }


  if (step == duration){
    step = 0;
    img_n++;
    if (img_n == img_n_max){
      img_n = 0;
    }
    
  }else{
    step++;
  }
}


void loop() {
}