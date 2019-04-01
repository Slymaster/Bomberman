#include <SDL2/SDL.h>
#include <stdio.h>
#include "SDL2/SDL_image.h"

typedef enum {

    MAIN_MENUE,
    PLAY_SOLO,
    PLAY_AT_TWO,
    PLAY_AT_THREE,
    PLAY_AT_FOUR
}e_statMenue;

int main(int argc, char const *argv[])
{
    int run = 3;
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Surface *img = NULL;
    SDL_Surface *img2 = NULL;
    SDL_Texture *textur = NULL;

    SDL_Rect rectangle;
    SDL_Event event;
    
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error: init SDL > %s \n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Bomberman!!!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    if(window == NULL)
    {
        SDL_Log("Error: screen can't be created > %s \n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if(render == NULL)
    {
        SDL_Log("Error: can't applied renderer > %s \n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    img = SDL_LoadBMP("./asset/header.bmp");
    img2 = SDL_LoadBMP("./asset/NES-Bomberman-Playfield.bmp");
    //SDL_FreeSurface(img);

    if(img == NULL)
    {
        SDL_Log("Error: can't load img > %s \n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        exit(EXIT_FAILURE);
    }

    textur = SDL_CreateTextureFromSurface(render, img);

    if(textur == NULL)
    {
        SDL_Log("Error: can't load texture > %s \n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        exit(EXIT_FAILURE);
    }

    if(SDL_QueryTexture(textur, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
    {
        SDL_Log("Error: can't load texture > %s \n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        exit(EXIT_FAILURE);
    }

    rectangle.x = (800 - rectangle.w) / 2;
    rectangle.y = (600 - rectangle.h) /2;

    e_statMenue choiceUser;
    
    while(run)
    {
        SDL_RenderClear(render);
        SDL_PollEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                run = 0;
            break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        run = 0;
                    break;

                    case SDLK_a:
                        choiceUser = PLAY_SOLO;
                    break;
                
                    default:
                       // printf("no case match");
                    break;
                }
            break;

            default:
                //printf("no case match");
            break;
        }

        switch(choiceUser)
        {
            case PLAY_SOLO:
                if(img2 == NULL)
                {
                    SDL_Log("Error: can't load img > %s \n", SDL_GetError());
                    SDL_DestroyRenderer(render);
                    SDL_DestroyWindow(window);
                    exit(EXIT_FAILURE);
                }

                textur = SDL_CreateTextureFromSurface(render, img2);
                //Display img:
                if(SDL_RenderCopy(render, textur, NULL, &rectangle) != 0)
                {
                    SDL_Log("Error: can't display texture > %s \n", SDL_GetError());
                    SDL_DestroyRenderer(render);
                    SDL_DestroyWindow(window);
                    exit(EXIT_FAILURE);
                }
            break;

            case MAIN_MENUE:
                //Display img:
                if(SDL_RenderCopy(render, textur, NULL, &rectangle) != 0)
                {
                    SDL_Log("Error: can't display texture > %s \n", SDL_GetError());
                    SDL_DestroyRenderer(render);
                    SDL_DestroyWindow(window);
                    exit(EXIT_FAILURE);
                }
            break;

            default:
                
            break;
        }
        SDL_RenderPresent(render);
    }


   // SDL_RenderPresent(render);
    //SDL_Delay(5000);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

   return EXIT_SUCCESS;
}