#include "include/menu.h"

int menu_multiplayer(SDL_Window* win)
{    
    int run = 3;
    int down = 0;
    SDL_Window *window = NULL;
    SDL_Renderer *render = NULL;
    SDL_Surface* surface;
    SDL_Texture* img;
    SDL_Texture* start;
    SDL_Rect rectangle;
    SDL_Event event;

    puts("Nous sommes dans le menu multiplayer.");

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

        surface = IMG_Load("./asset/menu_multiplayer.png");
        img = SDL_CreateTextureFromSurface(rend, surface);
        SDL_FreeSurface(surface);

        surface = IMG_Load("./asset/fleche.png");
        start = SDL_CreateTextureFromSurface(rend, surface);
        SDL_FreeSurface(surface);

    if(img == NULL)
    {
        SDL_Log("Error: can't load img > %s \n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        exit(EXIT_FAILURE);
    }

        if(start == NULL)
    {
        SDL_Log("Error: can't load start img > %s \n", SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
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
                        choiceUser = PLAY_SOLO;
                    break;

                    case SDL_SCANCODE_KP_ENTER:
                    case SDLK_KP_ENTER: // ne marche pas sur mon pc
                        choiceUser = THIS;
                        printf("enter");
                    break;

                    case SDLK_d:
                        choiceUser = THIS;
                    break;

                    case SDLK_DOWN:
                    case SDLK_s:
                        puts("down");
                        if(down == 0)
                            choiceUser = DOWN_MENU_1;
                        if(down == 1)
                            choiceUser = DOWN_MENU_2;
                        if(down == 2)
                            choiceUser = DOWN_MENU_3;
                        break;

                    case SDLK_UP:
                    case SDLK_z:
                        puts("up");
                        if(down == 1)
                            choiceUser = INITIAL_MENU;
                        if(down == 2)
                            choiceUser = DOWN_MENU_1;
                        if(down == 3)
                            choiceUser = DOWN_MENU_2;
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
                // SDL_DestroyRenderer(rend);
                // SDL_DestroyWindow(window);
                // SDL_Quit();
            break;

            case OPTIONS:
                // SDL_DestroyRenderer(rend);
                // SDL_DestroyWindow(window);
                // SDL_Quit();
                //menu_options(); // display options
            break;

            case RETURN:
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(window);
                SDL_Quit();
                menu_default();
            break;

            case QUIT:
                SDL_DestroyRenderer(rend);
                SDL_DestroyWindow(window);
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
                if(down == 2)
                    choiceUser = RETURN;
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
    SDL_DestroyWindow(window);
    SDL_Quit();

   return EXIT_SUCCESS;
}
