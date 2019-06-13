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
#include <stdbool.h>
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

int sock;
int sockArray[4];

static int launchServer(int mySocket, server_t *server);

void *initServer(void *input)
{
	menu_return_t *menu = input;
	struct sockaddr_in serverIn;

	int port = menu->port;
	int isEnable = 1;

	memset(&sockArray, -1, sizeof(sockArray));
	server_t *server;

	server = malloc(sizeof(server_t));

	if (server == NULL) {
		return NULL;
	}

	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock < 0) {
		return NULL;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &isEnable, sizeof(int)) < 0) {
		return NULL;
	}

	memset(&serverIn, 0, sizeof(serverIn));
	serverIn.sin_family = AF_INET;
	serverIn.sin_addr.s_addr = htonl(INADDR_ANY);
	serverIn.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *) &serverIn, sizeof(serverIn)) < 0) {
		return NULL;
	}

	if (listen(sock, 4) == 0) {
		printf("waiting for connections...\n");
	} else {
		return NULL;
	}


	launchServer(sock, server);
	free(server);

	return 0;
}


static int launchServer(int sock, server_t *server)
{
	int sockFd = 1;
	int clientCmpt = 0;
	unsigned int clientAddrLen;
	pthread_t threadId;
	int magicArray[5];
	struct sockaddr_in clientIn;

	clientCmpt = 0;
	clientAddrLen = sizeof(struct sockaddr_in);

	server->serverGame = malloc(sizeof(server_game_t));

	if (server->serverGame == NULL){
		return -1;
	}

	initPlayeInformations(server);

	server->serverBomb = malloc(sizeof(server_bomb_t));

	server->serverExplosion = malloc(sizeof(server_explosion_t));
	server->serverExplosion->first = true;

	if (server->serverBomb == NULL) {
		return -1;
	}

	if (server->serverExplosion == NULL) {
		return -1;
	}

	while (sockFd && status != -1) {
		memset(&magicArray, 0, sizeof(magicArray));

		if (clientCmpt >= 4) {
			sockFd = -1;
		} else {
			sockFd = accept(sock, (struct sockaddr *)&clientIn, (socklen_t *)&clientAddrLen);

			if (sockFd == -1) {
				printf("connections closed\n");
				return -1;
			}

			sockArray[clientCmpt] = sockFd;

			clientCmpt++;
			server->socketId = clientCmpt - 1;

			magicArray[server->socketId] = rand();
			server->magic[(server->socketId + 1)] = magicArray[server->socketId];

            playerInfos_t *player;
            player = getPlayer(server->serverGame, (server->socketId + 1));
            player->livePoint = 1;

			server->socketFd[clientCmpt - 1] = sockFd;

			memcpy(&(server->client), &clientIn, sizeof(struct sockaddr_in));
			server->clientAddressLength = clientAddrLen;

			send(server->socketFd[clientCmpt - 1], &magicArray[server->socketId], sizeof(int), 0);

			if (pthread_create(&threadId, NULL, handler, (void*) server) < 0) {
				return -1;
			}
		}
	}

	if (sockFd < 0) {
		return 1;
	}

	close(sockFd);
	close(sock);
	pthread_exit(NULL);
	return 0;
}

int sendData(server_t *server, server_game_t *gameServer)
{
	ssize_t sender;
	int i;

	for (i = 0; i < 4; ++i) {
		if (sockArray[i] != -1){
			sender = sendto(sockArray[i], gameServer,
					sizeof(*(gameServer)), MSG_NOSIGNAL,
					(struct sockaddr *)&server->client,
					server->clientAddressLength);
		}

		if (sender == -1) {
			close(server->socketFd[i]);
		}
	}

	return 0;
}

void initPlayeInformations(server_t *server)
{
	// init datum of player
	server->serverGame->player1.positionX = 2;
	server->serverGame->player1.positionY = 2;
	server->serverGame->player1.livePoint = 0;
	server->serverGame->player1.bombsLeft = 3;
	server->serverGame->player2.positionX = 12;
	server->serverGame->player2.positionY = 2;
	server->serverGame->player2.livePoint = 0;
	server->serverGame->player2.bombsLeft = 3;
	server->serverGame->player3.positionX = 2;
	server->serverGame->player3.positionY = 10;
	server->serverGame->player3.livePoint = 0;
	server->serverGame->player3.bombsLeft = 3;
	server->serverGame->player4.positionX = 12;
	server->serverGame->player4.positionY = 10;
	server->serverGame->player4.livePoint = 0;
	server->serverGame->player4.bombsLeft = 3;
}

//player methods implementation
void action(server_game_t *gameServer, server_bomb_t *bombServer, int player, char command)
{
    playerInfos_t *tplayer;
    tplayer = get_the_player(gameServer, player);

    if (tplayer->livePoint != 1)
    {
        return;
    }

    switch (command)
    {
        case 'U' :
        case 'D' :
        case 'L' :
        case 'R' :
            doMovement(gameServer, bombServer, player, command);
            break;
        case 'B' :
            if (tplayer->bombsLeft > 0)
            {
                putBomb(gameServer, bombServer, player);
            }
            break;
    }
}

playerInfos_t *getPlayer(server_game_t *gameServer, int player)
{
    playerInfos_t *tplayer;

    switch (player)
    {
        case 1 :
            tplayer = &gameServer->player1;
            break;
        case 2 :
            tplayer = &gameServer->player2;
            break;
        case 3 :
            tplayer = &gameServer->player3;
            break;
        case 4 :
            tplayer = &gameServer->player4;
            break;
    }

    return tplayer;
}

void doMovement(server_game_t *gameServer, server_bomb_t *bombServer, int player, char command)
{
    playerInfos_t *tplayer;
    tplayer = getPlayer(gameServer, player);

    switch (command)
    {
        case 'U' :
            if (isFreePlace(gameServer, tplayer->positionX, tplayer->positionY-1)
            && isPlayerFreePlace(gameServer, tplayer->positionX, tplayer->positionY-1)
            && isBombFreePlace(bombServer, tplayer->positionX, tplayer->positionY-1)) {
                tplayer->positionY--;
            }
            break;
        case 'D' :
            if (isFreePlace(gameServer, tplayer->positionX, tplayer->positionY+1)
            && isPlayerFreePlace(gameServer, tplayer->positionX, tplayer->positionY+1)
            && isBombFreePlace(bombServer, tplayer->positionX, tplayer->positionY+1)) {
                tplayer->positionY++;
            }
            break;
        case 'L' :
            if (isFreePlace(gameServer, tplayer->positionX-1, tplayer->positionY)
            && isPlayerFreePlace(gameServer, tplayer->positionX-1, tplayer->positionY)
            && isBombFreePlace(bombServer, tplayer->positionX-1, tplayer->positionY)) {
                tplayer->positionX--;
            }
            break;
        case 'R' :
            if (isFreePlace(gameServer, tplayer->positionX+1, tplayer->positionY)
            && isPlayerFreePlace(gameServer, tplayer->positionX+1, tplayer->positionY)
            && isBombFreePlace(bombServer, tplayer->positionX+1, tplayer->positionY)) {
                tplayer->positionX++;
            }
            break;
    }
}

bool isFreePlace(server_game_t *gameServer, int x, int y)
{
    if (gameServer->matrice[y][x] == '0' || gameServer->matrice[y][x] == '1'|| gameServer->matrice[y][x] == '2')
    {
        return false;
    }

    return true;
}

bool isPlayerFreePlace(server_game_t *gameServer, int x, int y)
{
    if (gameServer->player1.positionX == x && gameServer->player1.positionY == y && gameServer->player1.livePoint == 1)
    {
        return false;
    }

    if (gameServer->player2.positionX == x && gameServer->player2.positionY == y && gameServer->player2.livePoint == 1)
    {
        return false;
    }

    if (gameServer->player3.positionX == x && gameServer->player3.positionY == y && gameServer->player3.livePoint == 1)
    {
        return false;
    }

    if (gameServer->player4.positionX == x && gameServer->player4.positionY == y && gameServer->player4.livePoint == 1)
    {
        return false;
    }

    return true;
}

bool isBombFreePlace(server_bomb_t *bombServer, int x, int y)
{
    bool lastBomb = false;
    server_bomb_t *bomb = bombServer;

    while (!lastBomb) {
        if (bomb->bombPositionX == x && bomb->bombPositionY == y)
        {
            return false;
        }

        if (bomb->next != NULL)
        {
            bomb = bomb->next;
        } else {
            lastBomb = true;
        }
    }

    return true;
}

void putBomb(server_game_t *gameServer, server_bomb_t *bombServer, int player)
{
    createBomb(gameServer, bombServer, player);
}


//map methods implementation
void mapImplementation(server_game_t *gameServer, server_bomb_t *bombServer, server_explosion_t *explosionServer)
{
    for (int y = 0; y < 13; y++) {
        for (int x = 0; x < 15; x++) {
            switch (gameServer->matrice[y][x])
            {
                case '6' :
                case '7' :
                case '8' :
                case '9' :
                case 'A' :
                case 'G' :
                case 'H' :
                case 'I' :
                    gameServer->matrice[y][x] = '3';
                    break;
            }
        }
    }

    bombOnMap(gameServer, bombServer);
    placePlayerOnMap(gameServer);
    explosionOnMap(gameServer, explosionServer);
}

void placePlayerOnMap(server_game_t *gameServer)
{
    if (gameServer->player1.livePoint == 1) {
        gameServer->matrice[gameServer->player1.positionY][gameServer->player1.positionX] = '6';
        gameServer->matrice[0][4] = 'S';
    } else {
        gameServer->matrice[0][4] = 'W';
    }

    if (gameServer->player2.livePoint == 1) {
        gameServer->matrice[gameServer->player2.positionY][gameServer->player2.positionX] = '7';
        gameServer->matrice[0][6] = 'T';
    } else {
        gameServer->matrice[0][6] = 'X';
    }

    if (gameServer->player3.livePoint == 1) {
        gameServer->matrice[gameServer->player3.positionY][gameServer->player3.positionX] = '8';
        gameServer->matrice[0][8] = 'U';
    } else {
        gameServer->matrice[0][8] = 'Y';
    }

    if (gameServer->player4.livePoint == 1) {
        gameServer->matrice[gameServer->player4.positionY][gameServer->player4.positionX] = '9';
        gameServer->matrice[0][10] = 'V';
    } else {
        gameServer->matrice[0][10] = 'Z';
    }
}

void bombOnMap(server_game_t *gameServer, server_bomb_t *bombServer)
{
    bool isLastBomb = false;
    server_bomb_t *bomb = bombServer;
    while (!isLastBomb) {
        if (bomb->player != 0) {
            gameServer->matrice[bomb->bombPositionY][bomb->bombPositionX] = 'A';
        }

        // go to the next bomb
        if (bomb->next != NULL) {
            bomb = bomb->next;
        } else {
            isLastBomb = true;
        }
    }
}

void explosionOnMap(server_game_t *gameServer, server_explosion_t *explosionServer)
{
    bool isLastExplosion = false;
    server_explosion_t *explosion = explosionServer;
    while (!isLastExplosion)
    {
        if (explosion->first != 1)
        {
            gameServer->matrice[explosion->explosionY][explosion->explosionX] = 'G';

            for (int i=1; i < (explosion->size + 1); i++ )
            {
                if ( i < (explosion->sizeUp + 1))
                {
                    gameServer->matrice[(explosion->explosionY - i)][explosion->explosionX] = 'H';
                }
                if ( i < (explosion->sizeRight + 1)) {
                    gameServer->matrice[explosion->explosionY][(explosion->explosionX + i)] = 'I';
                }
                if ( i < (explosion->sizeDown + 1)) {
                    gameServer->matrice[(explosion->explosionY + i)][explosion->explosionX] = 'H';
                }
                if ( i < (explosion->sizeLeft+ 1)) {
                    gameServer->matrice[explosion->explosionY][(explosion->explosionX - i)] = 'I';
                }
            }
        }

        if (explosion->next != NULL)
        {
            explosion = explosion->next;
        }else{
            isLastExplosion = true;
        }
    }

}

//explosion methods implementations
void newExplosion(server_game_t *gameServer, server_explosion_t *explosionServer, int size, int positionX, int positionY)
{
    server_explosion_t *explosion;
    explosion = initExplosion(gameServer, size, positionX, positionY);
    bool isLastExplosion = false;
    server_explosion_t *currentExplosion = explosionServer;
    while (!isLastExplosion)
    {
        if (currentExplosion->next != NULL)
        {
            currentExplosion = currentExplosion->next;
        }else{
            currentExplosion->next = explosion;
            isLastExplosion = true;
        }
    }
}

server_explosion_t *initExplosion(server_game_t *gameServer, int size, int positionX, int positionY)
{
    long putTime;
    putTime = time(NULL);

    // init explosion
    server_explosion_t *explosion = NULL;
    explosion = malloc(sizeof(server_explosion_t));

    if (explosion == NULL) {
        return NULL;
    }
    explosion->first = false;
    explosion->explosionX = positionX;
    explosion->explosionY = positionY;
    explosion->size = size;
    explosion->sizeUp    = 0;
    explosion->sizeRight = 0;
    explosion->sizeDown  = 0;
    explosion->sizeLeft  = 0;
    explosion->time = putTime;
    explosion->prev = NULL;
    explosion->next = NULL;

    bool up     = true;
    bool right  = true;
    bool down   = true;
    bool left   = true;

    for (int i=1; i < (explosion->size + 1); i++)
    {
        if (up)
        {
            switch (wallPosition(gameServer, positionX, positionY - i))
            {
                case 0:
                    explosion->sizeUp = i;
                    destroyPlayer(gameServer, positionX, positionY - i);
                    break;
                case 2:
                    explosion->sizeUp = i;
                    up = false;
                    break;
                case 1:
                    up = false;
                    break;
            }
        }

        if (right)
        {
            switch (wallPosition(gameServer, positionX + i, positionY))
            {
                case 0:
                    explosion->sizeRight = i;
                    destroyPlayer(gameServer, positionX + i, positionY);
                    break;
                case 2:
                    explosion->sizeRight = i;
                    right = false;
                    break;
                case 1:
                    right = false;
                    break;
            }
        }

        if (down)
        {
            switch (wallPosition(gameServer, positionX, positionY + i))
            {
                case 0:
                    explosion->sizeDown = i;
                    destroyPlayer(gameServer, positionX, positionY + i);
                    break;
                case 2:
                    explosion->sizeDown = i;
                    down = false;
                    break;
                case 1:
                    down = false;
                    break;
            }
        }

        if (left)
        {
            switch (wallPosition(gameServer, positionX - i, positionY))
            {
                case 0:
                    explosion->sizeLeft = i;
                    destroyPlayer(gameServer, positionX - i, positionY);
                    break;
                case 2:
                    explosion->sizeLeft = i;
                    left = false;
                    break;
                case 1:
                    left = false;
                    break;
            }
        }
    }

    return explosion;
}

int wallPosition(server_game_t *gameServer, int x, int y)
{
    switch (gameServer->matrice[y][x])
    {
        case '2' :
            return 2;
        case '1' :
            return 1;
    }

    return 0;
}

void destroyPlayer(server_game_t *gameServer, int x, int y)
{

    if (gameServer->player1.positionX == x && gameServer->player1.positionY == y)
    {
        gameServer->player1.livePoint = 0;
    }

    if (gameServer->player2.positionX == x && gameServer->player2.positionY == y)
    {
        gameServer->player2.livePoint = 0;
    }

    if (gameServer->player3.positionX == x && gameServer->player3.positionY == y)
    {
        gameServer->player3.livePoint = 0;
    }

    if (gameServer->player4.positionX == x && gameServer->player4.positionY == y)
    {
        gameServer->player4.livePoint = 0;
    }
}

void explosionsTimer(server_explosion_t *explosion)
{
    long now = time(NULL);

    bool isLastExplosion = false;
    server_explosion_t *texplosion = explosion;

    while (!isLastExplosion)
    {
        if (texplosion->first != 1)
        {
            if ((now - texplosion->time) > 0 )
            {
                explosion->next = texplosion->next;
                free(texplosion);
            }
        }
        if (texplosion->next != NULL)
        {
            texplosion = texplosion->next;
        }else{
            isLastExplosion = true;
        }
    }
}

//bomb methods implementations

void createBomb(server_game_t *gameServer, server_bomb_t *bombServer, int player)
{
    playerInfos_t *tplayer;
    tplayer = getPlayer(gameServer, player);

    server_bomb_t *bomb;
    bomb = initBombInServer(player, tplayer->positionX, tplayer->positionY);

    bool isLastBomb = false;
    server_bomb_t *currentBomb = bombServer;
    while (!isLastBomb)
    {
        if (currentBomb->next != NULL)
        {
            currentBomb = currentBomb->next;
        }else{
            currentBomb->next = bomb;
            isLastBomb = true;
        }
    }
    tplayer->bombsLeft = tplayer->bombsLeft - 1;
}

server_bomb_t *initBombInServer(int player, int bomPositionX, int bomPositionY)
{
    long putTime;
    putTime = time(NULL);

    server_bomb_t *bomb = NULL;
    bomb = malloc(sizeof(server_bomb_t));
    bomb->bombPositionX = bomPositionX;
    bomb->bombPositionY = bomPositionY;
    bomb->player = player;
    bomb->time = putTime;
    bomb->size = 4;
    bomb->next = NULL;

    return bomb;
}

void bombTimer(server_game_t *gameServer, server_bomb_t *bombServer, server_explosion_t *explosionServer)
{
    playerInfos_t *tplayer;

    long now;
    now = time(NULL);

    bool isLastBomb = false;
    server_bomb_t *bomb = bombServer;

    while (!isLastBomb)
    {
        if (bomb->player != 0)
        {
            if ((now - bomb->time) > 1 )
            {
                bombServer->next = bomb->next;

                tplayer = getPlayer(gameServer, bomb->player);

                tplayer->bombsLeft++;

                newExplosion(gameServer, explosionServer, bomb->size, bomb->bombPositionX, bomb->bombPositionY);
                free(bomb);
                return;
            }
        }

        if (bomb->next != NULL)
        {
            bomb = bomb->next;
        }else{
            isLastBomb = true;
        }
    }
}
