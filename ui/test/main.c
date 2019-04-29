#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

// speed in pixels/second
#define SPEED (120)
int main(void)
{
  SDL_Surface* surface;
  SDL_Texture* background;
  SDL_Texture* player;

    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Bomberman",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    surface = IMG_Load("../asset/sprite_player.png");
    player = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    //SDL_SetRenderDrawColor(rend,46,204,64,0.8);

    // struct to hold the position and size of the sprite
    SDL_Rect dest;

    // get and scale the dimensions of texture
    //SDL_QueryTexture(tex2, NULL, NULL, NULL, NULL);
    SDL_QueryTexture(player, NULL, NULL, &dest.w, &dest.h);

}