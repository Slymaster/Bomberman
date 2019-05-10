#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (496)
#define WINDOW_HEIGHT (208)

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define NUMBER_BLOCS_WIDTH 31
#define NUMBER_BLOCS_HEIGHT 13

// speed in pixels/second
#define SPEED (300)

typedef enum {
    MAIN_MENUE,
    INITIAL_MENU,
    DOWN_MENU_1,
    DOWN_MENU_2,
    DOWN_MENU_3,
    THIS,
    PLAY_SOLO,
    OPTIONS,
    PLAY_AT_TWO,
    PLAY_AT_THREE,
    PLAY_AT_FOUR
} e_statMenue;

int map();
int menu_options();

#endif
