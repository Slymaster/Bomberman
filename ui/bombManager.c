#include "helper.h"


Fire* drawFire(float bombPosX, float bombPosY, SDL_Renderer *rend, SDL_Window *win)
{
    Fire *fire;
    fire = malloc(sizeof(Fire));

    if (fire == NULL)
    {
        printf("Error creating fire struct\n");
        return NULL;
    }

    fire->fireSurface = IMG_Load("asset/fire_center_sprite.png");

    if(!fire->fireSurface)
    {
        printf("error creating surface fireSurface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        return NULL;
    }

    fire->fireTexture = SDL_CreateTextureFromSurface(rend, fire->fireSurface);
    SDL_FreeSurface(fire->fireSurface);

    if (!fire->fireTexture)
    {
        printf("error creating texture player: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        return NULL;
    }

    fire->fireRect.w = TILE_WIDTH;
    fire->fireRect.h = TILE_HEIGHT;
    fire->fireRect.x = bombPosX;
    fire->fireRect.y = bombPosY;

    return fire;
}

Uint32 blastBomb(Uint32 time, void *voidBomb)
{
    Bomb *bomb = (Bomb *)voidBomb;

    if (bomb != NULL)
    {
        bomb->state = EXPLODED;
        printf("BOUUUUUUUUUUUUM\n");
    }
    else
    {
        printf("No bomb to blast\n");
    }

    return 0;
}

Bomb *putBomb(float x_pos, float y_pos, SDL_Renderer *rend)
{
    Bomb *bomb;
    bomb = malloc(sizeof(Bomb));

    if (bomb == NULL)
    {
        printf("Error creating bomb struct\n");
        return NULL;
    }

    bomb->bombSurface = IMG_Load("asset/bomb.png");

    if (!bomb->bombSurface)
    {
        printf("error creating surface bomb\n");
        SDL_DestroyRenderer(rend);
        return NULL;
    }

    bomb->bombTexture = SDL_CreateTextureFromSurface(rend, bomb->bombSurface);
    SDL_FreeSurface(bomb->bombSurface);

    if (!bomb->bombTexture)
    {
        printf("error creating texture player: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        return NULL;
    }

    bomb->bombRect.w = TILE_WIDTH;
    bomb->bombRect.h = TILE_HEIGHT;
    bomb->bombRect.x = x_pos;
    bomb->bombRect.y = y_pos;

    bomb->lifeSpan = 2000;
    bomb->state = NOT_EXPLODED;
    bomb->timerId = SDL_AddTimer(bomb->lifeSpan, blastBomb, bomb);

    if (bomb->timerId == 0)
    {
        fprintf(stderr, "cannot create bomb timer\n");
    }

    return bomb;
}