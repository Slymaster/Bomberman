#include <stdio.h>
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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <include/bomberman.h>
#include <include/server.h>

#define IP "127.0.0.1"
#define PORT 12345

player_t *init_player(void)
{
	player_t *player = NULL;

	player = malloc(sizeof(player_t));

	player->magic = 0;
	player->score = 0;
	player->speed = 10;
	player->dir = 3;
	player->command = 0;

	player->posX = 0;
	player->posY = 0;

	player->playerTexture = NULL;
	player->srcRectPlayer = malloc(sizeof(SDL_Rect));
	player->destRectPlayer = malloc(sizeof(SDL_Rect));

	return player;
}

void destroy_player(player_t *player)
{
	if (player) {
		if (player->playerTexture)
			SDL_DestroyTexture(player->playerTexture);

		if (player->srcRectPlayer)
			free(player->srcRectPlayer);

		if (player->destRectPlayer)
			free(player->destRectPlayer);

		free(player);
	}
}

SDL_Texture *set_texture_player(SDL_Renderer *pRenderer)
{
	SDL_Surface *surfacePlayer = IMG_Load("images/sprite_player.png");
	SDL_Texture *texture = NULL;
	if (!surfacePlayer) {
		return (NULL);
	} else {

		texture = SDL_CreateTextureFromSurface(pRenderer, surfacePlayer);
		if (texture == NULL) {
			SDL_FreeSurface(surfacePlayer);
			return NULL;
		}
		SDL_FreeSurface(surfacePlayer);
	}

	return texture;

}
