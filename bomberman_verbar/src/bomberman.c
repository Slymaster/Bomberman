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

int status;

void draw_game(global_game_t *game, server_game_t *sg)
{
	interface_t *interface 	= game->interface;
	map_t 		*map 		= game->map;
	bomb_t 		*bomb 		= game->bomb;
	player_t 	*player 	= game->player;
	explosion_t *explosion 	= game->explosion;
	int index;

	// back screen
	SDL_SetRenderDrawColor(interface->Renderer, 128, 20, 0, 255);
	SDL_RenderClear(interface->Renderer);

	setRectangle(&sg->player1.source, 0*16, 0*16, 16, 16);
	setRectangle(&sg->player2.source, 1*16, 0*16, 16, 16);
	setRectangle(&sg->player3.source, 2*16, 0*16, 16, 16);
	setRectangle(&sg->player4.source, 3*16, 0*16, 16, 16);

	if (sg != NULL) {
		for (int i = 0; i < map->nbTileY; i++) {
			for (int j = 0; j < map->nbTileX; j++) {

				setRectangle(interface->destRect, map->largeur_tile*i, map->hauteur_tile*j, map->largeur_tile, map->hauteur_tile);
		
				setRectangle(&sg->player1.destination,  map->largeur_tile * sg->player1.positionX, map->hauteur_tile * sg->player1.positionY, map->largeur_tile, map->hauteur_tile);
				setRectangle(&sg->player2.destination,  map->largeur_tile * sg->player2.positionX, map->hauteur_tile * sg->player2.positionY, map->largeur_tile, map->hauteur_tile);
				setRectangle(&sg->player3.destination,  map->largeur_tile * sg->player3.positionX, map->hauteur_tile * sg->player3.positionY, map->largeur_tile, map->hauteur_tile);
				setRectangle(&sg->player4.destination,  map->largeur_tile * sg->player4.positionX, map->hauteur_tile * sg->player4.positionY, map->largeur_tile, map->hauteur_tile);

				// index is the ASCII code of the character
				index = sg->matrice[j][i]-48;

				switch (index) {
					case 0:
					case 1:
					case 2:
					case 3:
						SDL_RenderCopy(interface->Renderer, map->mapTexture, map->tabTiles[index]->tile, interface->destRect);
						break;
					case 6:
						SDL_RenderCopy(interface->Renderer, player->playerTexture, &sg->player1.source, &sg->player1.destination);
						break;
					case 7:
						SDL_RenderCopy(interface->Renderer, player->playerTexture, &sg->player2.source, &sg->player2.destination);
						break;
					case 8:
						SDL_RenderCopy(interface->Renderer, player->playerTexture, &sg->player3.source, &sg->player3.destination);
						break;
					case 9:
						SDL_RenderCopy(interface->Renderer, player->playerTexture, &sg->player4.source, &sg->player4.destination);
						break;
					case 17:
						setRectangle(&bomb->srcRect, 8*16, 6*16, 16, 16);
						setRectangle(&bomb->destRect, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, bomb->TexBomb, &bomb->srcRect, &bomb->destRect);
						break;
					case 18:
						setRectangle(&bomb->srcRect, 7*16, 6*16, 16, 16);
						setRectangle(&bomb->destRect, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, bomb->TexBomb, &bomb->srcRect, &bomb->destRect);
						break;
					case 19:
						setRectangle(&bomb->srcRect, 6*16, 6*16, 16, 16);
						setRectangle(&bomb->destRect, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, bomb->TexBomb, &bomb->srcRect, &bomb->destRect);
						break;
					case 23:
						setRectangle(&explosion->srcRect, 5*16, 3*16, 16, 16);
						setRectangle(&explosion->destRect, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, explosion->explTexture, &explosion->srcRect, &explosion->destRect);
						break;
					case 24:
						setRectangle(&explosion->srcRect, 5*16, 4*16, 16, 16);
						setRectangle(&explosion->destRect, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, explosion->explTexture, &explosion->srcRect, &explosion->destRect);
						break;
					case 25:
						setRectangle(&explosion->srcRect, 0*16, 5*16, 16, 16);
						setRectangle(&explosion->destRect, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, explosion->explTexture, &explosion->srcRect, &explosion->destRect);
						break;
					case 83-48:
						setRectangle(player->srcRectPlayer, 0*16, 2*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
					case 84-48:
						setRectangle(player->srcRectPlayer, 1*16, 2*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
					case 85-48:
						setRectangle(player->srcRectPlayer, 2*16, 2*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
					case 86-48:
						setRectangle(player->srcRectPlayer, 3*16, 2*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
					case 87-48:
						setRectangle(player->srcRectPlayer, 0*16, 1*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
					case 88-48:
						setRectangle(player->srcRectPlayer, 1*16, 1*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
					case 89-48:
						setRectangle(player->srcRectPlayer, 2*16, 1*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
					case 90-48:
						setRectangle(player->srcRectPlayer, 3*16, 1*16, 16, 16);
						setRectangle(player->destRectPlayer, i * map->largeur_tile, j * map->hauteur_tile, map->largeur_tile, map->hauteur_tile);
						SDL_RenderCopy(interface->Renderer, player->playerTexture, player->srcRectPlayer, player->destRectPlayer);
						break;
				}
			}
		}
	}

	// show renderer
	SDL_RenderPresent(interface->Renderer);
}

int game_event(global_game_t *game, client_t *client_struct)
{
	SDL_Event e;
	status = 0;

	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			status = -1;
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					status = -1;
					break;
				case SDLK_UP:
					game->player->command = 'U';
					send_client_data(client_struct, game->player);
					break;
				case SDLK_DOWN:
					game->player->command = 'D';
					send_client_data(client_struct, game->player);
					break;
				case SDLK_LEFT:
					game->player->command = 'L';
					send_client_data(client_struct, game->player);
					break;
				case SDLK_RIGHT:
					game->player->command = 'R';
					send_client_data(client_struct, game->player);
					break;
				case SDLK_SPACE:
					game->player->command = 'B';
					send_client_data(client_struct, game->player);
					break;
				default :
				break;
			}
		}
	}

	return status;
}

void destroy_game(global_game_t *game)
{
	destroy_bomb(game->bomb);
	destroy_player(game->player);
	destroy_map(game->map);
	destroy_interface(game->interface);
	free(game);
}

clientRequest_t *receiveData(server_t *server)
{
	ssize_t receiver;

	clientRequest_t *request;

	request = malloc(sizeof(clientRequest_t));

	if (request == NULL) {
		return NULL;
	}

	receiver = recvfrom(server->socketFd[server->socketId],
						request, sizeof(*(request)), 0,
						(struct sockaddr *) &server->client,
						&server->clientAddressLength);

	if (receiver == -1) {
		free(request);
		return NULL;
	}

	return request;
}
