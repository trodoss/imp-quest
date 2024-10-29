/*

Imp Quest - Mini Game Jam 2024

2024 - trodoss
	  
This game supports I2C Based off of WiiChuck library:
https://github.com/coopermaa/Wiichuck   

This was tested with the Adafruit Wii Nunchuck Breakout Adapter - Qwiic / STEMMA QT
https://www.adafruit.com/product/4836

*/


#include "globals.h"
#include "menu.h"
#include "game.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuInfo,
  stateMenuPlay,
  stateMenuInput,
  stateMenuSoundfx,
  stateGamePlaying,
  stateStageComplete,
  stateGameOver
};


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  wii_on = 0;
}


void loop() {
  if (!(arduboy.nextFrame())) return;

  // input mapping
  input = 0;
  arduboy.pollButtons();

  //only check if in a playing state
  if (gameState >= STATE_GAME_PLAYING) {
    if (wii_on > 0) {
      wii.poll();

      if (wii.joyLeft()) BIT_SET(input,INPUT_LEFT);
      if (wii.joyRight()) BIT_SET(input,INPUT_RIGHT);
      if (wii.joyUp()) BIT_SET(input,INPUT_UP);
      if (wii.joyDown()) BIT_SET(input,INPUT_DOWN);
      if (wii.buttonZ()) BIT_SET(input,INPUT_A);
      if (wii.buttonC()) BIT_SET(input,INPUT_B);
    }

    if (arduboy.pressed(LEFT_BUTTON)) BIT_SET(input,INPUT_LEFT);
    if (arduboy.pressed(RIGHT_BUTTON)) BIT_SET(input,INPUT_RIGHT);
    if (arduboy.pressed(UP_BUTTON)) BIT_SET(input,INPUT_UP);
    if (arduboy.pressed(DOWN_BUTTON)) BIT_SET(input,INPUT_DOWN);
    if (arduboy.pressed(A_BUTTON)) BIT_SET(input,INPUT_A);
    if (arduboy.pressed(B_BUTTON)) BIT_SET(input,INPUT_B);
  }
  
  arduboy.clear();
((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

