#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

// matrix corresponding the letters with each of the buttons of the keypad
char letters[14] [4] = {
  {'0', '0', '0', '0'},
  {'a', 'b', 'c', '1'},
  {'d', 'e', 'f', '2'},
  {'g', 'h', 'i', '3'},
  {'j', 'k', 'l', '4'},
  {'m', 'n', 'o', '5'},
  {'p', 'q', 'r', '6'},
  {'s', 't', 'u', '7'},
  {'v', 'w', 'x', '8'},
  {'y', 'z', '9', '9'},
  {',', '.', '-', ':'},
  {'<', '?', '>', '!'},
  {'@', '+', '=', '#'},
  {'(', ')', '*', '/'}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

long latter_time;
long difference;
bool endtext = false;

char last_key_pressed = ' ';
byte num_times_pressed = 0;

char last_character = ' ';

String text = "";

void setup() {
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
}

void loop() {
  char key = keypad.getKey();
  // if the key '#' has been pressed, it prints the text and sets it to ""
  if (endtext) {
    Serial.println("texto final");
    Serial.println(text);
    text = "";
    last_character = ' ';
    endtext = false;
  }
}

// When a key is pressed, it has to be translated to the correct letter.
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      if (key == '#') {
        // key '#' sets the end of the text.
        if (last_character != ' ') {
          text += last_character;
        }
        endtext = true;
      } else {
        if (key == last_key_pressed) {
          /* If the same key is pressed several times, with an interval of less than 2 seconds, it is considered "looking for the next character".
             If the interval is of more than 2 seconds, it is considered a new character
          */
          difference = millis() - latter_time;
          latter_time = millis();
          if (difference > 2000) {
            text += last_character;

            byte indice;
            /* For keys A, B, C and D, corresponding positions 10 to 14 of the keys array, the way of calculating the index is, when converting to integer,
                substracting the ASCII corresponding to '7', that leaves A as 10, B as 11, and so on.
            */
            if (key > '9') {
              indice = key - '7';
            } else {
              indice = key - '0';
            }

            last_character = letters[indice][0];
            Serial.print("caracter: ");
            Serial.println(last_character);
            num_times_pressed = 1;
          }
          else {
            byte indice;
            if (key > '9') {
              indice = key - '7';
            } else {
              indice = key - '0';
            }

            last_character = letters[indice][num_times_pressed % 4];
            Serial.print("caracter: ");
            Serial.println(last_character);
            num_times_pressed++;

          }
        }
        else {
          last_key_pressed = key;
          if (last_character != ' ') {
            text += last_character;
          }

          byte indice;
          if (key > '9') {
            indice = key - '7';
          } else {
            indice = key - '0';
          }

          last_character = letters[indice][0];
          Serial.print("caracter: ");
          Serial.println(last_character);
          num_times_pressed = 1;
          latter_time = millis();
        }
      }
      break;
  }
}
