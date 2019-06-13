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
#include <include/server.h>"


explosion_t *init_explosion(SDL_Renderer *renderer)
{
    explosion_t *explosion = malloc(sizeof(explosion_t));
    if (explosion == NULL) {
        return NULL;
    }

    explosion->explTexture = set_texture_explosion(renderer);
    if (explosion->explTexture == NULL) {
        destroy_explosion(explosion);
        return NULL;
    }

    return explosion; 
}

void destroy_explosion(explosion_t *explosion)
{
    if (explosion) {
        if (explosion->explTexture)
            SDL_DestroyTexture(explosion->explTexture);

        free(explosion);
    }
}

SDL_Texture *set_texture_explosion(SDL_Renderer *renderer)
{
    SDL_Surface *surf = IMG_Load("images/tiles_bomberman.png");
    SDL_Texture *texture;

    if (!surf) {
		return NULL;
	} else {
		texture = SDL_CreateTextureFromSurface(renderer, surf);

		if (!texture) {
			SDL_FreeSurface(surf);
			return NULL;
		}

		SDL_FreeSurface(surf);
	}
	return texture;
}
