#include <Keypad.h>
#include <LedControl.h>

const byte ROWS = 4; // four keypad rows
const byte COLS = 3; // four keypad columns

// define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {3, 2, 1, 0};  // connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 4};     // connect to the column pinouts of the keypad

LedControl lc = LedControl(12, 11, 10, 1);

Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int PRESSED_BUTTON_NUM = 0; // keeping state of pressed button


void setup() {
  lc.shutdown(0, false);  // включаем дисплей энергосбережение дисплей
  lc.setIntensity(0, 8);  // устанавливаем яркость (0-минимум, 15-максимум)
  lc.clearDisplay(0);     // очищаем дисплей
}

void loop() {
  char button = keypad.waitForKey();

  switch(button){
    case '*': displayInit(); break;
    case '#': lc.clearDisplay(0); PRESSED_BUTTON_NUM = 0; break;
    default: 
    {
      if(PRESSED_BUTTON_NUM == 4) PRESSED_BUTTON_NUM = 0;
      
      lc.setChar(0, PRESSED_BUTTON_NUM, button, false);
      PRESSED_BUTTON_NUM++;
    }
  }
}

void displayInit(){
  for(int i=0; i<4; i++){
    for(int j=1; j<=7; j++){
      lc.setLed(0, i, j, true);
      delay(100);
    }
  }
  
  delay(1000);
  
  for(int i=3; i>=0; i--){
    for(int j=7; j>=1; j--){
      lc.setLed(0, i, j, false);
      delay(100);
    }
  }
}
