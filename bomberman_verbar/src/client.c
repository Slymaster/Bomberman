#include <stdlib.h>
#include <time.h>
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

client_t *init_client(char *ip_addr, unsigned short port)
{
	sleep(1);
	int sock;
	client_t *client_struct;
	struct sockaddr_in server;

	client_struct = malloc(sizeof(client_t));

	if (client_struct == NULL) {
		return NULL;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		NULL;
	}

	// build server address struct
	server.sin_family = AF_INET; // IPv4
	server.sin_addr.s_addr = inet_addr(ip_addr); // serveur IP to connect to
	server.sin_port = htons(port); // serveur port

	usleep(500);

	// connect to server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		return NULL;
	}

	client_struct->server = server;
	client_struct->sock = sock;

	if (client_struct == NULL) {
		return NULL;
	}

	return client_struct;
}

int send_client_data(client_t *client_data, player_t *player)
{
	ssize_t sender = -1;
	clientRequest_t *request;

	request = malloc(sizeof(clientRequest_t));

	if (client_data == NULL) {
		return -1;
	}

	if (request == NULL) {
		return -1;
	}

	request->magic = player->magic;
	request->x_pos = player->posX;
	request->y_pos = player->posY;
	request->dir = player->dir;
	request->command = player->command;
	request->speed = player->speed;
	request->checksum = request->magic /
			(request->x_pos +
			 request->y_pos +
			 request->dir +
			 request->command +
			 request->speed);

	sender = sendto(client_data->sock, request,
					sizeof(clientRequest_t), MSG_NOSIGNAL,
					(struct sockaddr *)&client_data->server,
					sizeof(client_data->server));

	if (sender == -1 ) {
		return -1;
	}

	free(request);
	return 0;
}

server_game_t *init_server_game(void)
{
	server_game_t *server_game;

	server_game = malloc(sizeof(server_game_t));

	if (server_game == NULL) {
		return NULL;
	}

	return server_game;
}

server_game_t *receive_server_data(client_t *client_data)
{
	ssize_t receiver;
	unsigned int server_addr_len;

	server_addr_len = sizeof(client_data->server);

	receiver = recvfrom(client_data->sock, client_data->server_game, sizeof(*client_data->server_game),
						0, (struct sockaddr *) &client_data->server,
						&server_addr_len);

	if (receiver == -1 ) {
		return NULL;
	}

	return client_data->server_game;
}

int get_magic(client_t *client_struct)
{
	int magic = -1;
	if (client_struct != NULL)
		recv(client_struct->sock, &magic, sizeof(int), 0);
	else
		magic = -1;
	return magic;
}

void *client_listening(void *client_data)
{
	while (global_game != NULL) {
		global_game = receive_server_data((client_t *)client_data);
	}

	return (void *)global_game;
}
