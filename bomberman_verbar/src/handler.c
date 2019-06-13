#include <stdio.h>
#include <string.h>
#include <pthread.h>
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


void *handler(void *input)
{
	int m;
	int num_player = 0;
	char **schema;
	server_t *server_data;
	clientRequest_t *request;

	server_data = malloc(sizeof(server_t));

	if (server_data == NULL) {
		return NULL;
	}

	memcpy(server_data, (server_t *)input, sizeof(server_t));


	server_data->serverBomb->player = 0;
	server_data->serverBomb->next = NULL;

	schema = malloc(13 * sizeof(char*));

	if (schema == NULL) {
		return NULL;
	}

	schema = handle_file("map.txt");

	for (int i = 0; i < 13; ++i) {
		memcpy(server_data->serverGame->matrice[i], schema[i], sizeof(char) * 15);
	}

	while (status != -1) {

		bombTimer(server_data->serverGame, server_data->serverBomb, server_data->serverExplosion);
		explosionsTimer(server_data->serverExplosion);

		// Sending players and bombs into map
		mapImplementation(server_data->serverGame, server_data->serverBomb, server_data->serverExplosion);

		sendData(server_data, server_data->serverGame);
		
		request = receiveData(server_data);

		if (request == NULL) {
			free(request);
			free(server_data);
			pthread_exit(NULL);
		}

		for (int i = 1; i < 5; i++) {
			m = request->magic;
			if (m == server_data->magic[i]) {
				num_player = i;
				break;
			}
		}

		action(server_data->serverGame, server_data->serverBomb, num_player, request->command);

		free(request);
	}

	free(schema);
	free(server_data);
	return (void *)input;
}

