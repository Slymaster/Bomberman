#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <include/bomberman.h>
#include <include/server.h>

bomb_t *init_bomb(SDL_Renderer *renderer)
{
	bomb_t *bomb = NULL;

	bomb = malloc(sizeof(bomb_t));

	if (bomb == NULL) {
		return NULL;
	}

	bomb->exist = 0;
	setRectangle(&bomb->srcRect, 8*16, 6*16, 16, 16);

	// load bomb texture
	bomb->TexBomb = set_texture_bomb(renderer);
	if (bomb->TexBomb == NULL) {
		destroy_bomb(bomb);
		return NULL;
	}

	return bomb;
}

void placeBomb(bomb_t *bomb, player_t *player)
{
	bomb->exist= 1;
	setRectangle(&bomb->destRect, player->destRectPlayer->x + 5, player->destRectPlayer->y + 20, 40, 55);
}

void destroy_bomb(bomb_t *bomb)
{
	if (bomb) {
		if (bomb->TexBomb)
			SDL_DestroyTexture(bomb->TexBomb);

		free(bomb);
	}
}

SDL_Texture *set_texture_bomb(SDL_Renderer *renderer)
{
	SDL_Surface *bombSurface = IMG_Load("images/tiles_bomberman.png");
	SDL_Texture *texture;

	if (!bombSurface) {
		return NULL;
	} else {
		texture = SDL_CreateTextureFromSurface(renderer, bombSurface);

		if (!texture) {
			SDL_FreeSurface(bombSurface);
			return NULL;
		}

		SDL_FreeSurface(bombSurface);
	}
	return texture;
}
