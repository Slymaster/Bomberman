#include "include/menu.h"

int main(int argc, char const *argv[])
{
    puts("Bienvenue sur Bomberman");

    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Bomberman",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       MENU_WIDTH, MENU_HEIGHT, 0);

    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Lancement du menu par défaut */
    menu_default(win);

    /* Debug */
    //map();
    //menu_options();
    //menu_multiplayer();

    return (0);
}
