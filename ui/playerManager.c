#include "helper.h"

Player *loadPlayer(SDL_Renderer *rend, SDL_Window *win)
{
    Player *player;
    player = malloc(sizeof(Player));

    if (player == NULL)
    {
        printf("Error creating player struct\n");
        return NULL;
    }

    player->playerSurface = IMG_Load("asset/view1.png");

    if (!player->playerSurface)
    {
        printf("error creating surface player\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return NULL;
    }

    player->playerTexture = SDL_CreateTextureFromSurface(rend, player->playerSurface);
    SDL_FreeSurface(player->playerSurface);

    if (!player->playerTexture)
    {

        printf("error creating texture player: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return NULL;
    }

    return player;
}