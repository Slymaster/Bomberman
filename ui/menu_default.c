#include "include/menu.h"

int menu_default(SDL_Window* win)
{
    int run = 3;
    int down = 0;
    SDL_Surface* surface;
    SDL_Texture* img;
    SDL_Texture* start;
    SDL_Rect rectangle;
    SDL_Event event;

    puts("Menu par défaut");

        // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
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


    surface = IMG_Load("asset/initial_menu.png");
    img = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    if(img == NULL)
    {
        SDL_Log("Error: can't load img > %s \n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        exit(EXIT_FAILURE);
    }

    surface = IMG_Load("asset/fleche.png");
    start = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    if(start == NULL)
    {
        SDL_Log("Error: can't load start img > %s \n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        exit(EXIT_FAILURE);
    }

    // struct to hold the position and size of the sprite
    SDL_Rect dest;
    SDL_Rect source;

    // get and scale the dimensions of texture
    //SDL_QueryTexture(tex2, NULL, NULL, NULL, NULL);
    SDL_QueryTexture(start, NULL, NULL, &dest.w, &dest.h);

    dest.x = 0;
    dest.y = 0;

    e_statMenue choiceUser;

    while(run)
    {
        // SDL_RenderClear(rend);
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
                        printf("escape");
                    break;

                    case SDLK_a:
                        SDL_DestroyRenderer(rend);
                        SDL_DestroyWindow(win);
                        SDL_Quit();
                        map();
                    break;

                    case SDLK_e:
                        choiceUser = MULTIPLAYER;
                    break;

                    case SDL_SCANCODE_KP_ENTER:
                    case SDLK_KP_ENTER: // ne marche pas sur mon pc
                        choiceUser = THIS;
                        puts("enter");
                    break;

                    case SDLK_d:
                        choiceUser = THIS;
                    break;

                    case SDLK_DOWN:
                    case SDLK_s:
                        if(down == 0)
                            choiceUser = DOWN_MENU_1;
                        if(down == 1)
                            choiceUser = DOWN_MENU_2;
                        if(down == 2)
                            choiceUser = DOWN_MENU_3;
                        break;

                    case SDLK_UP:
                    case SDLK_z:
                        if(down == 1)
                            choiceUser = INITIAL_MENU;
                        if(down == 2)
                            choiceUser = DOWN_MENU_1;
                        if(down == 3)
                            choiceUser = DOWN_MENU_2;
                        break;

                    default:
                       puts("no case match");
                    break;
                }
            break;

            default:
            break;
        }

        switch(choiceUser)
        {
            case PLAY_SOLO:
                // SDL_DestroyRenderer(rend);
                // SDL_DestroyWindow(win);
                // SDL_Quit();
                // map();
            break;

            case MULTIPLAYER:
                SDL_DestroyRenderer(rend);
                menu_multiplayer(win); // display options
            break;

            case OPTIONS:
                SDL_DestroyRenderer(rend);
                menu_options(win); // display options
            break;

            case QUIT:
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(win);
                SDL_Quit();
            break;

            case INITIAL_MENU:
                dest.y = 0;
                down = 0;
            break;

            case DOWN_MENU_1:
                dest.y = 150;
                down = 1;
            break;

            case DOWN_MENU_2:
                dest.y = 300;
                down = 2;
            break;

            case DOWN_MENU_3:
                dest.y = 450;
                down = 3;
            break;

            case THIS:
                if(down == 0)
                    choiceUser = PLAY_SOLO;
                if(down == 1)
                    choiceUser = MULTIPLAYER;
                if(down == 2)
                    choiceUser = OPTIONS;
                if(down == 3) // quit
                    choiceUser = QUIT;
            break;

            case MAIN_MENUE:
                //Display img:
            break;

            default:

            break;
        }
        SDL_RenderPresent(rend);
        
        // draw the image to the window
        SDL_RenderCopy(rend, img, NULL, NULL);
        SDL_RenderCopy(rend, start, NULL, &dest);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

   return EXIT_SUCCESS;
}
