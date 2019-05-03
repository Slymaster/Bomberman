#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <math.h>
#include <stdlib.h>

#define WINDOW_WIDTH (992)
#define WINDOW_HEIGHT (416)

// speed in pixels/second
#define SPEED (300)
#define TILE_WIDTH (32)

int tabTile [] = {
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                    0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
                    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                    0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
                    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                    0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
                    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                    0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
                    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                    0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
                    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

                };

int tabTile2[13][31] = {
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0},
                    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0},
                    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0},
                    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0},
                    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                    {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0},
                    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

                    };

int checkWall(float playerPosX, float playerPosY)
{
    //printf("Im in the function:");
    int yValInt = (int) ((playerPosY * 30)/495);
    int xValInt = (int) ((playerPosX * 12)/207);
      printf("valeur xInt: %d\n \n", xValInt);
       fflush(stdout);
        printf("yValInt: %d\n \n", yValInt);
         fflush(stdout);
        printf("valeur retounre: %d\n \n", tabTile2[yValInt][xValInt]);

    if(tabTile2[yValInt][xValInt] == 0)
    {
        printf("valeur retounre: %d\n \n", tabTile2[yValInt][xValInt]);
       // printf("yValInt: %d\n \n", yValInt);
        return 1;
    }else{
        return 0;
    }
}

/*int checkMapEdge(float playerPosX, float playerPosY, int playerW, int playerH, int mapW, int mapH)
{
    printf("Im in the check \n");
    fflush(stdout);

     int indexRow;

     printf("Value width of map: %d \n", mapW);
    fflush(stdout);

     double valDoubleX = (double) playerPosX;
     double valDoubleY = (double) playerPosY;

        printf("before floor \n");
    fflush(stdout);

     int valIntX = floor(valDoubleX)/16;
     int valIntY = floor(valDoubleY)/16;

     printf("valIntX: %d", valIntX);
     fflush(stdout);

    printf("valIntY: %d \n", valIntY);
    fflush(stdout);

     indexRow = valIntX + (mapW/16) * valIntY;

     printf("valeur de indexRow: %d \n", indexRow);
    fflush(stdout);

    if(tabTile[indexRow] == 0)
    {

        return 1;
    }else{
        return 0;
    }

}*/

int map(void)
{
  SDL_Surface* surface;
  SDL_Texture* background;
  SDL_Texture* player;

  SDL_Texture* wall;
  SDL_Texture* grass;


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

    surface = IMG_Load("asset/NESBombermanPlayfield_change.bmp");

    //int mapW = surface->w;
    //int mapH = surface->h;

    printf("voici la width de map: %d", surface->w);
    printf("voici la height de map: %d", surface->h);

    background = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("asset/view1.png");
    player = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);



    //SDL_SetRenderDrawColor(rend,46,204,64,0.8);

    // struct to hold the position and size of the spritec
    SDL_Rect dest;

    // get and scale the dimensions of texture
    //SDL_QueryTexture(tex2, NULL, NULL, NULL, NULL);
    SDL_QueryTexture(player, NULL, NULL, &dest.w, &dest.h);

    dest.w *= 19/16;
    dest.h *= 32/17;

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
    
    int touch;

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
                    printf("Salut voici ypos: %f \n", y_pos);
                   fflush(stdout);
                  up = 1;
                        /*if(y_pos < 51)
                        {
                            up = 0;
                        }else{

                            up = 1;
                        }*/
                        if(checkWall(x_pos, y_pos) == 1)
                        {
                            up = 0;
                        }else{
                            up = 1;
                        }
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    printf("Salut voici xpos: %f \n", x_pos);
                   fflush(stdout);
                    left = 1;
                   /*if(x_pos<34)
                   {
                       left = 0;
                   }else{

                    left = 1;
                   }*/

                   if(checkWall(x_pos, y_pos) == 1)
                   {
                       left = 0;
                   }else{
                       left = 1;
                   }

                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    printf("Salut voici ypos: %f \n", y_pos);
                   fflush(stdout);
                   down = 1;
                   /* if(y_pos > 495)
                    {
                        down = 0;
                    }else{

                    down = 1;
                    }*/

                    if(checkWall(x_pos, y_pos) == 1)
                   {
                       down = 0;
                   }else{
                       down = 1;
                   }
                    break;

                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    printf("Salut voici xpos: %f \n", x_pos);
                   fflush(stdout);
                    right = 1;
                   /*if(x_pos > 745)
                   {
                    right = 0;
                   }else{
                    right = 1;

                   }*/

                   if(checkWall(x_pos, y_pos) == 1)
                   {
                       right = 0;
                   }else{
                       right = 1;
                   }

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
        if (up && !down) y_vel = -1;
        if (down && !up) y_vel = 1;
        if (left && !right) x_vel = -1;
        if (right && !left) x_vel = 1;


        // update positions
        x_pos += x_vel * TILE_WIDTH;
        y_pos += y_vel * TILE_WIDTH;

        if(x_pos > 800)
        {
            x_pos = x_pos + 0;
        }
        //touch =  checkMapEdge(dest.x, dest.y, surface->w, surface->h, mapW, mapH);
        
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
        SDL_Delay(100);
    }

    // clean up resources before exiting
    SDL_DestroyTexture(player);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


// -------------------------- FIN MAP --------------------------

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
                SDL_DestroyRenderer(render);
                SDL_DestroyWindow(window);
                map(); // display map
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
