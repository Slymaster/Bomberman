#include "helper.h"

int map(void)
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

    // // load the image into memory using SDL_image library function
    // SDL_Surface* surface = IMG_Load("asset/view1.png");
    // if (!surface)
    // {
    //     printf("error creating surface\n");
    //     SDL_DestroyRenderer(rend);
    //     SDL_DestroyWindow(win);
    //     SDL_Quit();
    //     return 1;
    // }
    //
    // // load the image data into the graphics hardware's memory
    // SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    // SDL_FreeSurface(surface);
    // if (!tex || !tex2)
    // {
    //     printf("error creating texture: %s\n", SDL_GetError());
    //     SDL_DestroyRenderer(rend);
    //     SDL_DestroyWindow(win);
    //     SDL_Quit();
    //     return 1;
    // }
    //
    // SDL_Surface* surface = IMG_Load("asset/NES-Bomberman-Playfield.png");
    // if (!surface)
    // {
    //     printf("error creating surface\n");
    //     SDL_DestroyRenderer(rend);
    //     SDL_DestroyWindow(win);
    //     SDL_Quit();
    //     return 1;
    // }
    // SDL_Texture* tex2 = SDL_CreateTextureFromSurface(rend, surface);
    // SDL_FreeSurface(surface);

    surface = IMG_Load("asset/NES-Bomberman-Playfield.png");
    background = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("asset/sprite_player.png");
    player = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    //SDL_SetRenderDrawColor(rend,46,204,64,0.8);

    // struct to hold the position and size of the sprite
    SDL_Rect dest;

    // get and scale the dimensions of texture
    //SDL_QueryTexture(tex2, NULL, NULL, NULL, NULL);
    SDL_QueryTexture(player, NULL, NULL, &dest.w, &dest.h);

    dest.w /= 1;
    dest.h /= 1;

    // start sprite in center of screen
    float x_pos = (WINDOW_WIDTH - dest.w) / 2;
    float y_pos = (WINDOW_HEIGHT - dest.h) / 2;
    float x_vel = 0;
    float y_vel = 0;

    // keep track of which inputs are given
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    // set to 1 when window close button is pressed
    int close_requested = 0;

    // animation loop
    while (!close_requested)
    {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                close_requested = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 1;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    break;

                    case SDL_SCANCODE_C: //Quitter en attendant d'avoir quelque chose de mieux
                    SDL_DestroyTexture(player);
                    SDL_DestroyRenderer(rend);
                    SDL_DestroyWindow(win);
                    SDL_Quit();
                    exit(EXIT_SUCCESS);
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                }
                break;
            }
        }

        // determine velocity
        x_vel = y_vel = 0;
        if (up && !down) y_vel = -SPEED;
        if (down && !up) y_vel = SPEED;
        if (left && !right) x_vel = -SPEED;
        if (right && !left) x_vel = SPEED;

        // update positions
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        // collision detection with bounds
        if (x_pos <= 0) x_pos = 0;
        if (y_pos <= 0) y_pos = 0;
        if (x_pos >= WINDOW_WIDTH - dest.w) x_pos = WINDOW_WIDTH - dest.w;
        if (y_pos >= WINDOW_HEIGHT - dest.h) y_pos = WINDOW_HEIGHT - dest.h;

        // set the positions in the struct
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;

        // clear the window
        SDL_RenderClear(rend);

        // draw the image to the window
        SDL_RenderCopy(rend, background, NULL, NULL);
        SDL_RenderCopy(rend, player, NULL, &dest);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }

    // clean up resources before exiting
    SDL_DestroyTexture(player);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}




// -------------------------- FIN MAP --------------------------




// -- FIN SPRITE --//






typedef enum {

    MAIN_MENUE,
    INITIAL_MENU,
    DOWN_MENU_1,
    DOWN_MENU_2,
    THIS,
    PLAY_SOLO,
    PLAY_AT_TWO,
    PLAY_AT_THREE,
    PLAY_AT_FOUR
}e_statMenue;

int main(int argc, char const *argv[])
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

        surface = IMG_Load("./asset/header.jpg");
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


    /* ok */

        // struct to hold the position and size of the sprite
    SDL_Rect dest;

    // get and scale the dimensions of texture
    //SDL_QueryTexture(tex2, NULL, NULL, NULL, NULL);
    SDL_QueryTexture(start, NULL, NULL, &dest.w, &dest.h);

    dest.y = 100; // hat
    dest.x = 200; // lar



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
                        printf("escape");
                    break;

                    case SDLK_a:
                        choiceUser = PLAY_SOLO;
                    break;

                    case SDLK_KP_ENTER: // ne marche pas sur mon pc
                        choiceUser = THIS;
                    break;

                    case SDLK_d:
                        choiceUser = THIS;
                    break;

                    case SDLK_s:
                        if(down == 0)
                        choiceUser = DOWN_MENU_1;
                        if(down == 1)
                        choiceUser = DOWN_MENU_2;
                    break;

                    case SDLK_z:
                        if(down == 1)
                            choiceUser = INITIAL_MENU;
                        if(down == 2)
                            choiceUser = DOWN_MENU_1;
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
                SDL_DestroyRenderer(render);
                SDL_DestroyWindow(window);
                map(); // display map
            break;

            case INITIAL_MENU:
                dest.y = 100;
                down = 0;
            break;

            case DOWN_MENU_1:
                dest.y = 200;
                down = 1;
            break;

            case DOWN_MENU_2:
                dest.y = 300;
                down = 2;
            break;

            case THIS:
                if(down == 0)
                    choiceUser = PLAY_SOLO;
                if(down == 2) // quit
                    run = 0;
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
