#ifndef _MENU_H_
#define _MENU_H_
#include "helper.h"

#define MENU_WIDTH (557)
#define MENU_HEIGHT (608)

typedef enum {
    MAIN_MENUE,
    INITIAL_MENU,
    DOWN_MENU_1,
    DOWN_MENU_2,
    DOWN_MENU_3,
    THIS,
    PLAY_SOLO,
    MULTIPLAYER,
    OPTIONS,
    RETURN,
    QUIT,
    PLAY_AT_TWO,
    PLAY_AT_THREE,
    PLAY_AT_FOUR
} e_statMenue;

int menu_default();
int menu_multiplayer(SDL_Window* win);
int menu_options(SDL_Window* win);
int map();
int main();

#endif
