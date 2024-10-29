#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "backdrop.h"
#include "elements.h"
#include "player.h"

void stateGamePlaying() {
 gameState = STATE_GAME_PLAYING;

 if (BIT_CHECK(input, INPUT_RIGHT)) {
    player_move(FACING_RIGHT);
 }
 
 if (BIT_CHECK(input, INPUT_LEFT)) {
    player_move(FACING_LEFT);
 }
 
if (BIT_CHECK(input, INPUT_A)) {
  player_jump();
 }
 
 backdrop_draw();
 
 player_handle();
 level_element_handle();

};

void stateMenuPlay()
{
  backdrop.view_x = 0;
  backdrop.view_y = 0;
  backdrop_init();
  level_element_add(TYPE_PUMPKIN,32,24,PUMPKIN_ANIMATE,1,1);

  player_init();
  stateGamePlaying();
 
};

void stageComplete() {
  gameState = STATE_STAGE_COMPLETE;
}

void stateStageComplete() {
    if (arduboy.justPressed(A_BUTTON | B_BUTTON)) stateMenuPlay();   
}


void stateGameOver() {
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    gameState = STATE_MENU_MAIN;
  }
}



//-----------

#endif
