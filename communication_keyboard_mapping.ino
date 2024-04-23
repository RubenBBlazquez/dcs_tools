// this code must be executed with an arduino leonardo that have a driver for keyboard

#include <Keypad.h>
#include <Keyboard.h>

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5};
byte colPins[COLS] = {4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600);
}

void loop(){
  char key = keypad.getKey();
  
  if (key == '*'){
    Keyboard.press(KEY_F11);
    Keyboard.release(KEY_F11);
  }

  if (key == '#'){
    Keyboard.press(KEY_F12);
    Keyboard.release(KEY_F12);
  }

  if (key == '1'){
    Keyboard.press(KEY_F1);
    Keyboard.release(KEY_F1);
  }

  if (key == '2'){
    Keyboard.press(KEY_F2);
    Keyboard.release(KEY_F2);
  }

  if (key == '3'){
    Keyboard.press(KEY_F3);
    Keyboard.release(KEY_F3);
  }

  if (key == '4'){
    Keyboard.press(KEY_F4);
    Keyboard.release(KEY_F4);
  }

  if (key == '5'){
    Keyboard.press(KEY_F5);
    Keyboard.release(KEY_F5);
  }

  if (key == '6'){
    Keyboard.press(KEY_F6);
    Keyboard.release(KEY_F6);
  }

  if (key == '7'){
    Keyboard.press(KEY_F7);
    Keyboard.release(KEY_F7);
  }

  if (key == '8'){
    Keyboard.press(KEY_F8);
    Keyboard.release(KEY_F8);
  }

  if (key == '9'){
    Keyboard.press(KEY_F9);
    Keyboard.release(KEY_F9);
  }

  if (key == '0'){
    Keyboard.press(KEY_F10);
    Keyboard.release(KEY_F10);
  }
}
 
