#include "include/helper.h"

int killPlayer(Fire *fire, Player *player)
{
    printf("player in x in KP: %f\n", player->x_pos);
    printf("player in y in KP: %f\n", player->y_pos);


    printf("fire in x in KP: %d \n", fire->fireRect.x );
    printf("fire in x in KP: %d \n", fire->fireRect.y);

    if(
        (player->x_pos <  fire->fireRect.x +15 && player->x_pos >  fire->fireRect.x -15) && 
        (player->y_pos <  fire->fireRect.y +15 && player->y_pos >  fire->fireRect.y -15)
    )
    {
        SDL_DestroyTexture(player->playerTexture);
        player = NULL;
        printf("player is killed\n");

        return 1;
    }else{
        printf("Cannot kill player or player don't touch flame\n");
        return 0;
    }
}

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