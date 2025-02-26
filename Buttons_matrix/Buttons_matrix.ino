const int rows = 3;
const int columns = 3;

const int row_pins[rows] = {2, 3, 4};
const int col_pins[columns] = {5, 6, 7};

bool pressing_matrix[rows][columns];
long int times_matrix[rows][columns];

void setup(){
  for (int i = 0; i < rows; i++){
    pinMode(row_pins[i], OUTPUT);
  }

  for (int i = 0; i < columns; i++){
    pinMode(col_pins[i], INPUT_PULLUP);
  }
  
  for (int row = 0; row < rows; row++){
    for (int column = 0; column < columns; column++){
        pressing_matrix[row][column] = false;
        times_matrix[row][column] = 0;
    } 
  } 
  Serial.begin(9600);
}


void show_buttons(){
  Serial.print("Buttons - ");
  for (int row = 0; row < rows; row++){
    for (int column = 0; column < columns; column++){
      if (pressing_matrix[row][column]){
        Serial.print(row * columns + column + 1);
        Serial.print(" ");
      }
    } 
  } 
  Serial.println(" pressed");
}


void loop(){
  for (int row = 0; row < rows; row++){
    for (int r = 0; r < rows; r++){
      digitalWrite(row_pins[r], HIGH);
    }
    digitalWrite(row_pins[row], LOW);

    for (int column = 0; column < columns; column++){
      bool btn_state = digitalRead(col_pins[column]);

      // if button was pressed
      if (pressing_matrix[row][column]){
        // if button isn't pressed now
        if (btn_state){
          Serial.print("button ");
          Serial.print(row * columns + column + 1);
          Serial.print(" untouched start - ");
          Serial.print(times_matrix[row][column]);
          Serial.print("ms. ");
          Serial.print(" duration - ");
          Serial.print(millis() - times_matrix[row][column]);
          Serial.println("ms. ");


          pressing_matrix[row][column] = false;
        }

      // if button wasn't pressed
      }else{
        // if button is pressed now
        if (!btn_state){
          pressing_matrix[row][column] = true;
          times_matrix[row][column] = millis();
          show_buttons();
        }
      }
    } 
  } 
}

