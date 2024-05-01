#include <Keyboard.h>
#include <Mouse.h>
#include <Adafruit_NeoPixel.h>

int DATA_CLOCK = 13;
int DATA_LATCH = 12;
int DATA_SERIAL = 11;
int gui_led = 10;
int mouseSens = 5;
int gui = 0;
int buttons[12];
int buttonsOS[12];

int boatGuiID = 2;
int inventoryGuiID = 1;
int inventoryLastGuiID = 0;
int playingGuiID = 0;

Adafruit_NeoPixel strip(1, gui_led, NEO_RGB + NEO_KHZ800);

void setup(){
  Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();
  setupPins();
  strip.begin();
  strip.show();
}

void loop(){
  getControllerData();
  if (gui == playingGuiID){
    strip.setPixelColor(0, 255, 0, 0);
    strip.show();
  } else if (gui == inventoryGuiID) {
    strip.setPixelColor(0, 0, 255, 0);
    strip.show();
  } else if (gui == boatGuiID) {
    strip.setPixelColor(0, 0, 0, 255);
    strip.show();
  }
}

void setupPins(void){
  // Set DATA_CLOCK normally HIGH
  pinMode(DATA_CLOCK, OUTPUT);
  digitalWrite(DATA_CLOCK, HIGH);
  
  // Set DATA_LATCH normally LOW
  pinMode(DATA_LATCH, OUTPUT);
  digitalWrite(DATA_LATCH, LOW);

  // Set DATA_SERIAL normally HIGH
  pinMode(DATA_SERIAL, OUTPUT);
  digitalWrite(DATA_SERIAL, HIGH);
  pinMode(DATA_SERIAL, INPUT);  
}

void getControllerData(void){
    // Latch for 12us
    digitalWrite(DATA_LATCH, HIGH);
    delayMicroseconds(12);
    digitalWrite(DATA_LATCH, LOW);
    delayMicroseconds(6);
    
    // Retrieve button presses from shift register by pulling the clock high for 6us
    for(int i = 0; i < 16; i++){
        digitalWrite(DATA_CLOCK, LOW);
        delayMicroseconds(6);
        if(i <= 11){
            buttons[i] = digitalRead(DATA_SERIAL);
        }
        digitalWrite(DATA_CLOCK, HIGH);
            delayMicroseconds(6);

    }
  
    
  if (!array_cmp(buttons, buttonsOS, 12,12)){
    for(int i = 0; i < 12; i++){
      buttonsOS[i] = buttons[i];
    }
    Serial.println(String(buttons[0]) + String(buttons[1])
    + String(buttons[2]) + String(buttons[3])
    + String(buttons[4]) + String(buttons[5])
    + String(buttons[6]) + String(buttons[7])
    + String(buttons[8]) + String(buttons[9])
    + String(buttons[10]) + String(buttons[11]));
    togglebinds(gui);
  }
  spambinds(gui);
}


// Prints which buttons are pressed
void togglebinds(int guiMode){
  if (guiMode == playingGuiID) {
    if(buttons[0] == 0){ //B
      Mouse.press(MOUSE_LEFT);
    } else {
      Mouse.release(MOUSE_LEFT);
    }
    if(buttons[1] == 0){ //Y
      Keyboard.press(KEY_UP_ARROW);
    } else {
      Keyboard.release(KEY_UP_ARROW);
    }
    if(buttons[4] == 0){ //UP
      Keyboard.press('w');
    } else {
      Keyboard.release('w');
    }
    if(buttons[5] == 0){ //DOWN
      Keyboard.press(KEY_LEFT_SHIFT);
    } else {
      Keyboard.release(KEY_LEFT_SHIFT);
    }
    if(buttons[8] == 0 and buttons[11] != 0 buttons[10] != 0){ //A (preventing from glitching with the oher command)
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }
    if(buttons[9] == 0){ //X
      Keyboard.press('e');
      //Changing gui mode
      inventoryLastGuiID = playingGuiID;
      gui = inventoryGuiID;
      Keyboard.release('e');
    }
    if(buttons[10] == 0 and buttons[8] != 0){ //LB (preventing from glitching with the oher command)){ //LB
      Mouse.move(0, 0, 1);
    }
    if(buttons[11] == 0 and buttons[8] != 0){ //RB (preventing from glitching with the oher command)
      Mouse.move(0, 0, -1);
    }
    if (buttons[11] == 0 and buttons[8] == 0){ //RB + A
      inventoryLastGuiID = playingGuiID;
      gui = inventoryGuiID;
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }
    if (buttons[10] == 0 and buttons[8] == 0){ //LB + A
      gui = boatGuiID;
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }
  }
  if (guiMode == inventoryGuiID) {
    if(buttons[0] == 0){ //B 
      Keyboard.press(KEY_ESC);
      //Changing gui mode
      gui = inventoryLastGuiID;
      Keyboard.release(KEY_ESC);
    }
    if(buttons[9] == 0){ //X
      Keyboard.press(KEY_LEFT_SHIFT);
      Mouse.press(MOUSE_LEFT);
    } else {
      Keyboard.release(KEY_LEFT_SHIFT);
      Mouse.release(MOUSE_LEFT);
    }
    if(buttons[8] == 0){ //A
      Mouse.press(MOUSE_LEFT);
    } else {
      Mouse.release(MOUSE_LEFT);
    }
    if(buttons[1] == 0){ //Y
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }
    if(buttons[11] == 0){ //RB
      Keyboard.press('9');
    } else {
      Keyboard.release('9');
    }
    if(buttons[10] == 0){ //LB
      Keyboard.press('f');
    } else {
      Keyboard.release('f');
    }
  }
  if (guiMode == boatGuiID) {
    if(buttons[0] == 0){ //B
      Mouse.press(MOUSE_LEFT);
    } else {
      Mouse.release(MOUSE_LEFT);
    }
    if(buttons[1] == 0){ //Y
      Keyboard.press(KEY_UP_ARROW);
    } else {
      Keyboard.release(KEY_UP_ARROW);
    }
    if(buttons[4] == 0){ //UP
      Keyboard.press('w');
    } else {
      Keyboard.release('w');
    }
    if(buttons[5] == 0){ //DOWN
      Keyboard.press(KEY_LEFT_SHIFT);
      gui = playingGuiID;
    } else {
      Keyboard.release(KEY_LEFT_SHIFT);
    }
    if(buttons[8] == 0 and buttons[11] != 0){ //A (preventing from glitching with the oher command)
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }
    if(buttons[9] == 0){ //X
      Keyboard.press('e');
      //Changing gui mode
      inventoryLastGuiID = boatGuiID;
      gui = inventoryGuiID;
    }
    if(buttons[10] == 0 and buttons[8] != 0){ //LB (preventing from glitching with the oher command)
      Mouse.move(0, 0, 1);
    }
    if(buttons[11] == 0 and buttons[8] != 0){ //RB (preventing from glitching with the oher command)
      Mouse.move(0, 0, -1);
    }
    if (buttons[11] == 0 and buttons[8] == 0){ //RB + A
      inventoryLastGuiID = boatGuiID;
      gui = inventoryGuiID;
      Mouse.press(MOUSE_RIGHT);
    } else {
      Mouse.release(MOUSE_RIGHT);
    }
    if(buttons[6] == 0){ //LEFT
      Keyboard.press('a');
    } else {
      Keyboard.release('a');
    }
    if(buttons[7] == 0){ //RIGHT
      Keyboard.press('d');
    } else {
      Keyboard.release('d');
    }
  }
}
void spambinds(int guiMode){
  if (guiMode == playingGuiID) {
    if(buttons[6] == 0){ //LEFT
      Mouse.move(mouseSens * -1, 0, 0);
    }
    if(buttons[7] == 0){ //RIGHT
      Mouse.move(mouseSens, 0, 0);
    }
    if(buttons[2] == 0){ //SELECT
      Mouse.move(0, mouseSens * -1, 0);
    }
    if(buttons[3] == 0){ //START
      Mouse.move(0, mouseSens * 2, 0);
    }
  }
  if (guiMode == inventoryGuiID) {
    if(buttons[6] == 0){ //LEFT
      Mouse.move(mouseSens * -1, 0, 0);
    }
    if(buttons[7] == 0){ //RIGHT
      Mouse.move(mouseSens, 0, 0);
    }
    if(buttons[4] == 0){ //UP
      Mouse.move(0, mouseSens * -1, 0);
    }
    if(buttons[5] == 0){ //DOWN
      Mouse.move(0, mouseSens, 0);
    }
  }
  if (guiMode == boatGuiID) {
    if(buttons[2] == 0){ //SELECT
      Mouse.move(0, mouseSens * -1, 0);
    }
    if(buttons[3] == 0){ //START
      Mouse.move(0, mouseSens * 2, 0);
    }
  }
}
boolean array_cmp(int *a, int *b, int len_a, int len_b){
      int n;

      // if their lengths are different, return false
      if (len_a != len_b) return false;

      // test each element to be the same. if not, return false
      for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;

      //ok, if we have not returned yet, they are equal :)
      return true;
}
