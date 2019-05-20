#include "include/bomb.h"

int map()
{
   // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Bomberman",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    Player *player = loadPlayer(rend, win);
    Bomb *bomb = NULL;
    Fire *fire = NULL;

    SDL_Surface *tileset = IMG_Load("asset/unnamed.bmp");
    if (!tileset)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tilesetTexture = SDL_CreateTextureFromSurface(rend, tileset);
    SDL_FreeSurface(tileset);

    if (!tilesetTexture)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // get and scale the dimensions of tileset texture
    SDL_Rect tiles;
    SDL_QueryTexture(tilesetTexture, NULL, NULL, &tiles.w, &tiles.h);
    tiles.w = TILE_WIDTH;
    tiles.h = TILE_WIDTH;

    // displayObstacles(rend, tilesetTexture);
    SDL_RenderClear(rend);

    char *map[] = {
        "0000000000000000000000000000000",
        "0111111111111111111111111111110",
        "0101010101010101010101010101010",
        "0111111111111111111111111111110",
        "0101010101010101010101010101010",
        "0111111111111111111111111111110",
        "0101010101010101010101010101010",
        "0111111111111111111111111111110",
        "0101010101010101010101010101010",
        "0111111111111111111111111111110",
        "0101010101010101010101010101010",
        "0111111111111111111111111111110",
        "0000000000000000000000000000000"};

    int i, j;
    SDL_Rect rect_dst;
    SDL_Rect rect_src;
    rect_src.w = TILE_WIDTH;
    rect_src.h = TILE_HEIGHT;
    for (i = 0; i < NUMBER_BLOCS_WIDTH; i++)
    {
        for (j = 0; j < NUMBER_BLOCS_HEIGHT; j++)
        {
            rect_dst.x = i * TILE_WIDTH;
            rect_dst.y = j * TILE_HEIGHT;
            rect_src.x = (map[j][i] - '0') * TILE_WIDTH;
            rect_src.y = 0;
            SDL_RenderCopy(rend, tilesetTexture, &rect_src, &rect_dst);
        }
    }

    // struct to hold the position and size of the sprite
    SDL_Rect dest;
    // get and scale the dimensions of texture
    SDL_QueryTexture(player->playerTexture, NULL, NULL, &dest.w, &dest.h);
    dest.w = 9;
    dest.h = 14;

    printf("Player position : [%d, %d]", dest.w, dest.h);
    fflush(stdout);

    // start sprite in center of screen
    float x_pos_old, y_pos_old = 0;
    float x_pos = (WINDOW_WIDTH - dest.w) / 2;  //(WINDOW_WIDTH - dest.w) / 2;
    float y_pos = (WINDOW_HEIGHT - dest.h) / 2; //(WINDOW_HEIGHT - dest.h) / 2;
    float x_vel = 0;
    float y_vel = 0;

    // keep track of which inputs are given
    int move_up = 0;
    int move_down = 0;
    int move_left = 0;
    int move_right = 0;

    int nb_grid_x = 0;
    int nb_grid_y = 0;
    float r_x = 0;
    float r_y = 0;
    int step_x = 0;
    int step_y = 0;
    // set to 1 when window close button is pressed
    int close_requested = 0;

    Flag bombFlag = TRUTHY;

    // animation loop
    while (!close_requested)
    {
        // process events
        SDL_Event event;
        SDL_WaitEvent(&event);

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
                move_up = 1;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                move_left = 1;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                move_down = 1;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                move_right = 1;
                break;

            case SDL_SCANCODE_B:
                bombFlag = FALSY;
                if(bombFlag == FALSY) {
                    bomb = putBomb(x_pos, y_pos, rend);
                }
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                move_up = 0;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                move_left = 0;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                move_down = 0;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                move_right = 0;
                break;
            }
            break;
        }

        // determine velocity
        x_vel = y_vel = 0;
        if (move_up && !move_down && !move_left && !move_right)
            y_vel = -SPEED;
        if (move_down && !move_up && !move_left && !move_right)
            y_vel = SPEED;
        if (move_left && !move_right && !move_down && !move_up)
            x_vel = -SPEED;
        if (move_right && !move_left && !move_down && !move_up)
            x_vel = SPEED;

        // update positions
        x_pos_old = x_pos;
        y_pos_old = y_pos;
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        player->x_pos = x_pos;
        player->y_pos = y_pos;

        // collision detection with bounds
        if (x_pos <= 0)
            x_pos = 0;
        if (y_pos <= 0)
            y_pos = 0;
        if (x_pos >= WINDOW_WIDTH - dest.w)
            x_pos = WINDOW_WIDTH - dest.w;
        if (y_pos >= WINDOW_HEIGHT - dest.h)
            y_pos = WINDOW_HEIGHT - dest.h;
        nb_grid_x = (int)(x_pos / TILE_WIDTH);
        r_x = x_pos - nb_grid_x * TILE_WIDTH;
        step_x = step_y = 0;
        if (r_x > TILE_WIDTH - dest.w)
        {
            step_x += 1;
        }
        nb_grid_y = (int)(y_pos / TILE_HEIGHT);
        r_y = y_pos - nb_grid_y * TILE_HEIGHT;
        if (r_y > TILE_HEIGHT - dest.h)
        {
            step_y += 1;
        }
        //printf("GRID : [%d, %d] => %c\n", (int)(y_pos / TILE_HEIGHT) + step_y, (int)(x_pos / TILE_WIDTH) + step_x, map[(int)(y_pos / TILE_HEIGHT) + step_y][(int)(x_pos / TILE_WIDTH) + step_x]);
        if (map[(int)(y_pos / TILE_HEIGHT) + step_y][(int)(x_pos / TILE_WIDTH) + step_x] == '0' || map[(int)(y_pos / TILE_HEIGHT)][(int)(x_pos / TILE_WIDTH) + step_x] == '0' || map[(int)(y_pos / TILE_HEIGHT) + step_y][(int)(x_pos / TILE_WIDTH)] == '0' || map[(int)(y_pos / TILE_HEIGHT)][(int)(x_pos / TILE_WIDTH)] == '0')
        {
            x_pos = x_pos_old;
            y_pos = y_pos_old;
            printf("obstacle detected\n");
        }
        else
        {
            // set the positions in the struct
            dest.y = (int)y_pos;
            dest.x = (int)x_pos;
            //printf("[x_pos, y_pos] : [%d, %d]", dest.x, dest.y);

            // clear the window
            SDL_RenderClear(rend);
            // displayObstacles(rend, tilesetTexture);
            for (i = 0; i < NUMBER_BLOCS_WIDTH; i++)
            {
                for (j = 0; j < NUMBER_BLOCS_HEIGHT; j++)
                {
                    rect_dst.x = i * TILE_WIDTH;
                    rect_dst.y = j * TILE_HEIGHT;
                    rect_src.x = (map[j][i] - '0') * TILE_WIDTH;
                    rect_src.y = 0;
                    SDL_RenderCopy(rend, tilesetTexture, &rect_src, &rect_dst);
                }
            }

            if (bomb != NULL)
            {
                if (bomb->state == EXPLODED)
                {
                    fire = drawFire(bomb->bombRect.x, bomb->bombRect.y, rend, win);
                    if(fire)
                    {
                        SDL_RenderCopy(rend, fire->fireTexture, NULL, &fire->fireRect);
                        killPlayer(fire, player);

                        fire = drawFire(bomb->bombRect.x +15, bomb->bombRect.y, rend, win);
                        SDL_RenderCopy(rend, fire->fireTexture, NULL, &fire->fireRect);
                        killPlayer(fire, player);


                        fire = drawFire(bomb->bombRect.x -15, bomb->bombRect.y, rend, win);
                        SDL_RenderCopy(rend, fire->fireTexture, NULL, &fire->fireRect);
                        killPlayer(fire, player);

                        fire = drawFire(bomb->bombRect.x , bomb->bombRect.y+15, rend, win);
                        SDL_RenderCopy(rend, fire->fireTexture, NULL, &fire->fireRect);
                        killPlayer(fire, player);

                        fire = drawFire(bomb->bombRect.x , bomb->bombRect.y-15, rend, win);
                        SDL_RenderCopy(rend, fire->fireTexture, NULL, &fire->fireRect);
                        killPlayer(fire, player);
                    }
                    bomb = NULL;
                }
                else
                {
                    SDL_RenderCopy(rend, bomb->bombTexture, NULL, &bomb->bombRect);
                }
            }

            SDL_RenderCopy(rend, player->playerTexture, NULL, &dest);
            SDL_RenderPresent(rend);
        }

        // wait 1/60th of a second
        SDL_Delay(1000 / 60);
    }

    // clean up resources before exiting
    SDL_DestroyTexture(tilesetTexture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
