#ifndef _BOMB_H_
#define _BOMB_H_
#include "helper.h"

enum BombState { NOT_EXPLODED, EXPLODING, EXPLODED };
typedef enum { FALSY = 0, TRUTHY = 1} Flag;

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

typedef struct
{
    float x_pos;
    float y_pos;
    float width;
    float height;
    SDL_Surface *fireSurface;
    SDL_Texture *fireTexture;
    SDL_Rect fireRect;
}Fire;

Uint32 blastBomb(Uint32 time, void *voidBomb);
Bomb *putBomb(float x_pos, float y_pos, SDL_Renderer *rend);
Player *loadPlayer(SDL_Renderer *rend, SDL_Window *win);
int killPlayer(Fire* fire, Player* player);
Fire* drawFire(float bombPosX, float bombPosY, SDL_Renderer *rend, SDL_Window *win);

#endif
