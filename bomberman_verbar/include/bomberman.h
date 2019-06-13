#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

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

extern int status;

#define TRUE 1
#define true 1
#define false 0
#define FALSE 0

//structs
typedef enum gameState gameState;
typedef struct Images Images;
typedef struct Button Button;
typedef struct Buttons Buttons;
typedef struct menu_return_s menu_return_t;
typedef struct bomb_s bomb_t;
typedef struct player_s player_t;
typedef struct client_s client_t;
typedef struct clientRequest_s clientRequest_t;
typedef struct explosion_s explosion_t;
typedef struct global_game_s global_game_t;
typedef struct interface_s interface_t;
typedef char tileIndex;
typedef struct map_s map_t;
typedef struct tileProp_s tileProp_t;


//structs body
struct bomb_s
{
	// Does the bomb exist
	int exist;
	int posX, posY;

	// bomb textures
	SDL_Texture *TexBomb;

	// position of bomb
	SDL_Rect srcRect;
	SDL_Rect destRect;
	
};

struct client_s {
	int sock;
	struct sockaddr_in server;
	server_game_t *server_game;
};

struct clientRequest_s {
	unsigned int magic; 
	int x_pos;			
	int y_pos;			
	int dir;			
	int command;		
	int speed;			
	int checksum;		
};

struct explosion_s {

    SDL_Texture     *explTexture;
    SDL_Rect        destRect;
    SDL_Rect        srcRect;
    char            type;
};

struct global_game_s {
	interface_t *interface;
	player_t *player;
	bomb_t *bomb;
	map_t *map;
	char *ip;
	u_short port;
	explosion_t *explosion;
};

struct interface_s {
	// SDL variables
	SDL_Point screenSize;
	SDL_Window *Window;
	SDL_Renderer *Renderer;
	SDL_Rect *destRect;

	// TTF font
	TTF_Font *Font;
};

struct tileProp_s {
	SDL_Rect	*tile;
	SDL_Texture *textureTile;
	int plein;
	char charTest;
};

struct map_s {
	int largeur_tile;
	int hauteur_tile;
	int nbTileX;
	int nbTileY;
	tileProp_t **tabTiles;
	tileIndex **schema;
	SDL_Texture *mapTexture;
};

enum gameState {
    MENU, JOIN, EXIT
};
struct Images {
    SDL_Texture* mainMenuTex;
    SDL_Texture* joinPartyMenuTex;
};

struct Button {
    int min_x, max_x, min_y, max_y;
};

struct Buttons {
    Button createPartyBtn;
    Button joinPartyBtn;
    Button validateBtn;
    Button returnBtn;
    Button exitBtn;
};

struct menu_return_s {
    char *ip;
    u_short port;
    int ret;
};

struct player_s
{
	int magic;
	int score;
	int speed;
	int dir;
	char command;

	SDL_Texture *playerTexture;
    SDL_Rect    *srcRectPlayer;
    SDL_Rect    *destRectPlayer;
    int         posX, posY;
};

//methods
clientRequest_t *receiveData(server_t *server);
void draw_game(global_game_t *game, server_game_t *sg);
int game_event(global_game_t *game, client_t *client_struct);
void destroy_game(global_game_t *game);
int get_magic(client_t *client_struct);
void *client_listening(void *client_data);
server_game_t *global_game;
client_t *init_client(char *ip_addr, unsigned short port);
int send_client_data(client_t *client_data, player_t *player);
server_game_t *init_server_game(void);
server_game_t *receive_server_data(client_t *client_data);
bomb_t *init_bomb(SDL_Renderer *renderer);
void placeBomb(bomb_t *bomb, player_t *player);
void destroy_bomb(bomb_t *bomb);
SDL_Texture *set_texture_bomb(SDL_Renderer *renderer);
explosion_t *init_explosion(SDL_Renderer *renderer);
void destroy_explosion(explosion_t *explosion);
SDL_Texture *set_texture_explosion(SDL_Renderer *renderer);
global_game_t *init_game(void);
void *game_loop(void *game_struct);
void setup_game(menu_return_t *menu);
void *handler(void *input);
interface_t *init_interface(void);
void destroy_interface(interface_t *interface);
void setRectangle(SDL_Rect *rectangle, int x, int y, int w, int h);
map_t *init_map(const char *file);
char **handle_file(const char *file);
SDL_Texture *set_texture_map(SDL_Renderer *pRenderer);
int set_tile_array(map_t *map);
void destroy_map(map_t *map);
menu_return_t *menu(void);
void init_buttons(Buttons *btns);
void load_images(Images *images, SDL_Renderer *renderer);
int display_image(SDL_Renderer *renderer, SDL_Texture *texture);
int mouse_bouton(int x, int y, Buttons btns);
void free_images(Images *images);
void movePlayer(player_t *player, interface_t *interface, SDL_Keycode direction, client_t *client_struct);
bomb_t *dropBomb(player_t *player, bomb_t *bomb);
player_t *init_player(void);
void destroy_player(player_t *player);
SDL_Texture *set_texture_player(SDL_Renderer *pRenderer);

#endif
