#include <include/bomberman.h>
#include <include/server.h>
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

#define CACHE_SIZE 64

map_t *init_map(const char *file)
{
	// Malloc de la Map
	map_t *map = malloc(sizeof(map_t));
	if (map == NULL) {
		return NULL;
	}

	// Settings de la map
	map->hauteur_tile = 60;
	map->largeur_tile = 60;

	//malloc de nbLignes * sizeof(char*)
	map->schema = malloc(13 * sizeof(char*));
	if (map->schema == NULL) {
		destroy_map(map);
		return NULL;
	}

	map->tabTiles = malloc(5*sizeof(tileProp_t));
	if (map->tabTiles == NULL) {
		destroy_map(map);
		return NULL;
	}

	if (!set_tile_array(map)) {
		destroy_map(map);
		return NULL;
	}

	// Association de la map en fichier vers le schema de la map
	map->schema = handle_file(file);
	if (map->schema == NULL) {
		return NULL;
	}

	map->nbTileX = 13;
	map->nbTileY = 15;

	return map;
}


void destroy_map(map_t *map)
{
	if (map) {
		if (map->tabTiles) {
			for (int i = 0; i < 5; i++) {
				if (map->tabTiles[i])
					free(map->tabTiles[i]);
			}
			free(map->tabTiles);
		}

		if (map->mapTexture)
			SDL_DestroyTexture(map->mapTexture);

		if (map->schema) {
			free(map->schema);
		}
		free(map);
	}
}


void setRectangle(SDL_Rect *rectangle, int x, int y, int w, int h)
{
	rectangle->x = x;
	rectangle->y = y;
	rectangle->w = w;
	rectangle->h = h;
}

char **handle_file(const char *file)
{
	FILE *f;
	char **schema;
	int i = 0;
	char buf[CACHE_SIZE];

	f = fopen(file, "r");
	if (!f) {
		return NULL;
	}

	schema = malloc(sizeof(char) * 15 * 13);

	if (schema == NULL) {
		return NULL;
	}

	// parse file
	while (fgets(buf, CACHE_SIZE, f) != NULL) {
		schema[i] = strdup(buf);
		i++;
	}

	fclose(f);
	return schema;
}

SDL_Texture *set_texture_map(SDL_Renderer *pRenderer)
{
	SDL_Texture *texture = NULL;

	// Generate MAP texture
	SDL_Surface *surf = IMG_Load("images/tiles_bomberman.png");

	if (surf == NULL) {
		fprintf(stderr, "[ERROR] surface KO \n %s \n", IMG_GetError());
		return NULL;
	} else {

		texture = SDL_CreateTextureFromSurface(pRenderer, surf);
		if (texture == NULL) {
			printf("[ERROR] texture KO \n");
			SDL_FreeSurface(surf);
			return NULL;
		}

		SDL_FreeSurface(surf);
	}

	return texture;
}

int set_tile_array(map_t *map)
{
	// Associate a tile array for the MAP
	tileProp_t *tiles = NULL;
	for (int i = 0; i < 5; i++) {
		tiles = malloc(sizeof(tileProp_t));
		if (tiles == NULL) {
			destroy_map(map);
			return 0;
		}

		tiles->plein = 0;
		tiles->tile = malloc(sizeof(SDL_Rect));
		if (!tiles->tile) {
			destroy_map(map);
			return 0;
		}

		setRectangle(tiles->tile, 16*i, 0, 16, 16);
		tiles->charTest = i+48;
		map->tabTiles[i] = tiles;
	}

	free(tiles);
	return 1;
}
