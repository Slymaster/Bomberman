#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>
#include <unistd.h>
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

typedef struct
{
    float x_pos;
    float y_pos;
    float width;
    float height;
    SDL_Surface *playerSurface;
    SDL_Texture *playerTexture;
    SDL_Rect bombRect;
}Player;

enum BombState { NOT_EXPLODED, EXPLODING, EXPLODED };

typedef enum { FALSY = 0, TRUTHY = 1} Flag;

typedef struct
{
    enum BombState state;
    Uint32 lifeSpan;
    unsigned int createdAt;
    float x_pos;
    float y_pos;
    float width;
    float height;
    Player player;
    SDL_Surface *bombSurface;
    SDL_Texture *bombTexture;
    SDL_Rect bombRect;
    SDL_TimerID timerId;
}Bomb;

int map();
int menu_options();
Uint32 blastBomb(Uint32 time, void *voidBomb);
Bomb *putBomb(float x_pos, float y_pos, SDL_Renderer *rend);
Player *loadPlayer(SDL_Renderer *rend, SDL_Window *win);

#endif
