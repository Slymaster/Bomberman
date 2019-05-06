#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (600)
#define WINDOW_HEIGHT (600)

#define TILE_WIDTH 60
#define TILE_HEIGHT 60

#define NUMBER_BLOCS_WIDTH 10
#define NUMBER_BLOCS_HEIGHT 10

// speed in pixels/second
#define SPEED (300)

int map();

#endif
