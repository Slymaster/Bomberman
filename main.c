#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

// speed in pixels/second
#define SPEED (300)
int map(void)
{
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

    // load the image into memory using SDL_image library function
    SDL_Surface* surface = IMG_Load("asset/view1.png");
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }


    // load the image data into the graphics hardware's memory
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

	SDL_SetRenderDrawColor(rend,46,204,64,0.8);

    // struct to hold the position and size of the sprite
    SDL_Rect dest;

    // get and scale the dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
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
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }
    
    // clean up resources before exiting
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


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
		map();







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


    SDL_RenderPresent(render);
    //SDL_Delay(5000);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

   return EXIT_SUCCESS;
}
