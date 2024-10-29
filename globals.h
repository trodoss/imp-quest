#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include <Sprites.h>
#include "bitmaps.h"
#include "ArduboyWiiChuck.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1

#define STATE_MENU_INFO              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INPUT             4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_PLAYING           6

#define STATE_STAGE_COMPLETE         7
#define STATE_GAME_OVER              8

// define mappable inputs 
#define INPUT_UP                     0
#define INPUT_DOWN                   1
#define INPUT_LEFT                   2
#define INPUT_RIGHT                  3
#define INPUT_A                      4
#define INPUT_B                      5

#define BIT_SET(var, flag) ((var) |= (1 << (flag))) 
#define BIT_CHECK(var, flag) ((var) & (1 << (flag))) 

Arduboy2Base arduboy;
ArduboyWiiChuck wii;
Sprites sprites;

unsigned long score = 0;


byte gameState = STATE_MENU_INTRO;   // start the game with the logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;

uint8_t wii_on = 0;
uint8_t input = 0;

#endif
