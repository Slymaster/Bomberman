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

int main(void)
{
	srandom(time(0));

	menu_return_t *menu_ret;

	menu_ret = menu();

	setup_game(menu_ret);

	free(menu_ret);

	return 0;
}
