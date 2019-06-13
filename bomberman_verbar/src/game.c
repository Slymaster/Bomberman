#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
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

#define PORT 12345

global_game_t *init_game(void)
{
	global_game_t *game = NULL;

	game = malloc(sizeof(global_game_t));

	if (game == NULL) {
		return NULL;
	}

	game->interface = init_interface();
	if (game->interface == NULL) 
		return NULL;

	game->map = init_map("map.txt");
	if (game->map == NULL)
		return NULL;

	game->map->mapTexture = set_texture_map(game->interface->Renderer);
	if (game->map->mapTexture == NULL)
		return NULL;

	game->player = init_player();
	if (game->player == NULL)
		return NULL;

	game->player->playerTexture = set_texture_player(game->interface->Renderer);
	if (game->player->playerTexture == NULL)
		return NULL;

	game->bomb = init_bomb(game->interface->Renderer);
	if (game->bomb == NULL) 
		return NULL;

	game->explosion = init_explosion(game->interface->Renderer);
	if (game->explosion == NULL)
		return NULL;

	return game;
}

int get_client_id(client_t *client_struct)
{
	int client_id;

	recv(client_struct->sock, &client_id, sizeof(int), 0);

	return client_id;
}

void *game_loop(void *game_struct)
{
	int status = 0, magic;
	global_game_t *game = (global_game_t *)game_struct;
	client_t *client_struct;
	pthread_t thread_client;

	client_struct = init_client(game->ip, game->port);

	if (client_struct != NULL) {
		magic = get_magic(client_struct);
		client_struct->server_game = init_server_game();
		game->player->magic = magic;

		global_game = malloc(sizeof(server_game_t));

		if (global_game == NULL) {
			return NULL;
		}

		if (pthread_create(&thread_client, NULL, client_listening, (void*) client_struct) < 0) {
			exit(EXIT_FAILURE);
		}
	} else {
		destroy_game(game);
		exit(-1);
	}

	while (status != -1) {

		draw_game(game, global_game);

		status = game_event(game, client_struct);

		SDL_Delay(20);
	}
	free(global_game);
	free(client_struct);
	shutdown(socket, SHUT_RDWR);
	close(socket);
	return (void *)game_struct;
}

void setup_game(menu_return_t *menu)
{
	pthread_t thread_sdl, thread_net;
	global_game_t *game = NULL;

	game = init_game();

	if (game == NULL) {
		exit(EXIT_FAILURE);
	}

	if (menu->ret == 1) {
		if (pthread_create(&thread_net, NULL, initServer, (void*) menu) < 0) {
			exit(EXIT_FAILURE);
		}
	}

	game->ip = menu->ip;
	game->port = menu->port;

	if (pthread_create(&thread_sdl, NULL, game_loop, (void*) game) < 0) {
		exit(EXIT_FAILURE);
	}

	/* join SDL thead */
	if (pthread_join(thread_sdl, NULL) != 0) {
		exit(EXIT_FAILURE);
	}
}
