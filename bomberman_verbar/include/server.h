#ifndef SERVER_H_
#define SERVER_H_


#include <include/bomberman.h>
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

extern int mySocket;

typedef struct server_s server_t;
typedef struct server_game_s server_game_t;
typedef struct playerInfos_s playerInfos_t;
typedef struct server_bomb_s server_bomb_t;
typedef struct server_explosion_s server_explosion_t;

typedef char t_map[12];

struct message {
	int messageId;
	char enteredMessage[10];
};

struct server_s {
	int magic[5];//what is it
	int socketFd[4];//what is it
	int socketId;
	struct sockaddr_in client;
	unsigned int clientAddressLength;
	server_game_t *serverGame;
	server_bomb_t *serverBomb;
	server_explosion_t *serverExplosion;
};

struct playerInfos_s {
	int livePoint;
	int positionX;
	int positionY;
	int currentDirection;
	int bombsLeft;

	SDL_Rect	destination;
	SDL_Rect	source;
};

playerInfos_t *player1;
playerInfos_t *player2;
playerInfos_t *player3;
playerInfos_t *player4;

struct server_game_s {
	playerInfos_t player1;
	playerInfos_t player2;
	playerInfos_t player3;
	playerInfos_t player4;

	char matrice[13][15];
};

struct server_bomb_s {
    int bombPositionX;
    int bombPositionY;
    int player;
    long time;
    int size;
    server_bomb_t *next;
};

struct server_explosion_s {
    bool first;
    int explosionX;
    int explosionY;
    int size;
    int sizeUp;
    int sizeRight;
    int sizeDown;
    int sizeLeft;
    long time;
    server_explosion_t *next;
    server_explosion_t *prev;
};

//
void *initServer(void *input);
void initPlayeInformations(server_t *server);
int sendData(server_t *server, server_game_t *game);

//bomb methods
server_bomb_t *initBombInServer(int player, int bomPositionX, int bomPositionY);
void createBomb(server_game_t *game, server_bomb_t *bomb, int player);
void bombTimer(server_game_t *game, server_bomb_t *bomb, server_explosion_t *explosion);

//player methods
void action(server_game_t *game,server_bomb_t *bomb , int player, char command);
playerInfos_t *getPlayer(server_game_t *game, int player);
void doMovement(server_game_t *game, server_bomb_t *bomb, int player, char command);

bool isFreePlace(server_game_t *game, int positionX, int positionY);
bool isPlayerFreePlace(server_game_t *game, int positionX, int positionY);
bool isBombFreePlace(server_bomb_t *bomb, int positionX, int positionY);

void putBomb(server_game_t *game, server_bomb_t *bomb, int player);

//map methods
void mapImplementation(server_game_t *game, server_bomb_t *bomb, server_explosion_t *explosion);

void playerOnMap(server_game_t *game);
void bombOnMap(server_game_t *game, server_bomb_t *bomb);
void explosionOnMap(server_game_t *game, server_explosion_t *explosion);

void placePlayerOnMap(server_game_t *game);

//boumm methods
server_explosion_t *initExplosion(server_game_t *game, int size, int positionX, int positionY);
void newExplosion(server_game_t *game, server_explosion_t *explosion, int size, int positionX, int positionY);
int wallPosition(server_game_t *game, int positionX, int positionY);
void destroyPlayer(server_game_t *game, int positionX, int positionY);
void explosionsTimer(server_explosion_t *explosion);

#endif
