#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

#define MAX_IP_LENGTH 16
#define MAX_PORT_LENGTH 8

menu_return_t *menu(void)
{
	Images images;
	Buttons buttons;
	gameState game_state = MENU;

	menu_return_t *menu_ret;

	int quit_bomberman = 0;
	int IPTextLength = 0;
	int PortTextLength = 0;
	char *IPTextInput = (char*) malloc(16);
	char *PortTextInput = (char*) malloc(8);
	int isIPTextInput = FALSE;
	int isPortTextInput = FALSE;

	SDL_Surface *IPSurfaceText = NULL, *PortSurfaceText = NULL;
	SDL_Texture *IPText = NULL, *PortText = NULL;

	SDL_Rect IPTextRect, PortTextRect;
	IPTextRect.x = 68;
	PortTextRect.x = 471;
	IPTextRect.y = PortTextRect.y = 150;
	IPTextRect.w = PortTextRect.w = 336;
	IPTextRect.h = PortTextRect.h = 90;

	SDL_Color white = {255, 255, 255, 90};

	init_buttons(&buttons);

	interface_t *interface;

	interface = init_interface();

	if (interface == NULL)
		return NULL;

	// active transparency
	SDL_SetRenderDrawBlendMode(interface->Renderer, SDL_BLENDMODE_BLEND);
	load_images(&images, interface->Renderer);

	menu_ret = malloc(sizeof(menu_return_t));

	if (menu_ret == NULL) {
		return NULL;
	}

	menu_ret->ip = "127.0.0.1";
	menu_ret->port = 12345;
	menu_ret->ret = -1;

	while (game_state != EXIT) {
		switch (game_state) {
			case MENU:
				if (display_image(interface->Renderer, images.mainMenuTex) == EXIT_FAILURE)
					return NULL;
			break;
			case JOIN:
				if (display_image(interface->Renderer, images.joinPartyMenuTex) == EXIT_FAILURE)
					return NULL;
				SDL_RenderCopy(interface->Renderer, IPText, NULL, &IPTextRect);
				SDL_RenderCopy(interface->Renderer, PortText, NULL, &PortTextRect);
			break;
			default:
			break;
		}

		SDL_Event event;
		SDL_WaitEvent(&event);
		switch(event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (game_state == JOIN) {
						// IP
						if (isIPTextInput && IPTextLength > 0) {
							IPTextLength--;
							IPTextInput[strlen(IPTextInput)-1] = 0;
							IPSurfaceText = TTF_RenderText_Blended(interface->Font, IPTextInput, white);
							IPText = SDL_CreateTextureFromSurface(interface->Renderer, IPSurfaceText);
						} // Port
						else if (isPortTextInput && PortTextLength > 0) {
							PortTextLength--;
							PortTextInput[strlen(PortTextInput)-1] = 0;
							PortSurfaceText = TTF_RenderText_Blended(interface->Font, PortTextInput, white);
							PortText = SDL_CreateTextureFromSurface(interface->Renderer, PortSurfaceText);
						}
					}
				}
			break;
			case SDL_TEXTINPUT:
				if (game_state == JOIN) {
					// IP
					if (isIPTextInput && IPTextLength < MAX_IP_LENGTH) {
						IPTextLength++;
						strcat(IPTextInput, event.text.text);
						IPSurfaceText = TTF_RenderText_Blended(interface->Font, IPTextInput, white);
						IPText = SDL_CreateTextureFromSurface(interface->Renderer, IPSurfaceText);
					} // Port
					else if (isPortTextInput && PortTextLength < MAX_PORT_LENGTH) {
						PortTextLength++;
						strcat(PortTextInput, event.text.text);
						PortSurfaceText = TTF_RenderText_Blended(interface->Font, PortTextInput, white);
						PortText = SDL_CreateTextureFromSurface(interface->Renderer, PortSurfaceText);
					}
				}
			break;
			case SDL_MOUSEBUTTONDOWN:
				if (game_state == JOIN) {
					int x = event.button.x, y=event.button.y;
					// Click on the "IP" or "Port" text input
					if (x >= IPTextRect.x && x <= IPTextRect.x+IPTextRect.w &&
						y >= IPTextRect.y && y <= IPTextRect.y+IPTextRect.h)
					{
						SDL_StartTextInput();
						isIPTextInput = TRUE;
						isPortTextInput = FALSE;
					} else if (x >= PortTextRect.x && x <= PortTextRect.x+PortTextRect.w &&
						y >= PortTextRect.y && y <= PortTextRect.y+PortTextRect.h)
					{
						SDL_StartTextInput();
						isIPTextInput = FALSE;
						isPortTextInput = TRUE;
					} else {
						isIPTextInput = FALSE;
						isPortTextInput = FALSE;
						SDL_StopTextInput();
					}

					// Click on the "Validate" button
					if (x >= buttons.validateBtn.min_x && x <= buttons.validateBtn.max_x &&
						y >= buttons.validateBtn.min_y && y <= buttons.validateBtn.max_y)
					{

						// inject data in the struct
						// we return at the end of the function
						menu_ret->ip = IPTextInput;
						menu_ret->port = atoi(PortTextInput);
						menu_ret->ret = 2;

						// flag set so we can leave menu loop
						game_state = EXIT;
					}

					// Click on the "Return" button
					if (x >= buttons.returnBtn.min_x && x <= buttons.returnBtn.max_x &&
						y >= buttons.returnBtn.min_y && y <= buttons.returnBtn.max_y)
					{
						game_state = MENU;
					}
				} else if (game_state == MENU) {
					int button = mouse_bouton(event.button.x, event.button.y, buttons);
					if (button == 0) {
						game_state = JOIN;
					} else if (button == 1) {
						menu_ret->ret = 1;
						game_state = EXIT;
					} else if (button == 2) {
						game_state = EXIT;
						quit_bomberman = 1;
					}
				}
				break;
			case SDL_QUIT:
				game_state = EXIT;
				quit_bomberman = 1;
				break;
		}

		SDL_SetRenderDrawColor(interface->Renderer, 0, 0, 0, 0);
		if (isIPTextInput)
			SDL_RenderFillRect(interface->Renderer, &IPTextRect);
		if (isPortTextInput)
			SDL_RenderFillRect(interface->Renderer, &PortTextRect);

		SDL_RenderPresent(interface->Renderer);
	}

	free_images(&images);
	SDL_FreeSurface(IPSurfaceText);
	SDL_FreeSurface(PortSurfaceText);
	SDL_DestroyTexture(IPText);
	SDL_DestroyTexture(PortText);
	SDL_DestroyWindow(interface->Window);
	SDL_DestroyRenderer(interface->Renderer);
	//destroy_interface(interface);

	if (quit_bomberman == 1) {
		exit(0);
	}

	return menu_ret;
}

void init_buttons(Buttons *buttons)
{
	int width = 365;
	int height = 90;

	// coordinate min_x
	buttons->createPartyBtn.min_x
		= buttons->joinPartyBtn.min_x
		= buttons->exitBtn.min_x
		= buttons->validateBtn.min_x
		= buttons->returnBtn.min_x
		= 255;

	// coordinate max_x
	buttons->createPartyBtn.max_x
		= buttons->joinPartyBtn.max_x
		= buttons->exitBtn.max_x
		= buttons->validateBtn.max_x
		= buttons->returnBtn.max_x
		= 255 + width;

	// coordinate y
	buttons->createPartyBtn.min_y = 148;
	buttons->createPartyBtn.max_y = 148 + height;
	buttons->joinPartyBtn.min_y = buttons->validateBtn.min_y = 269;
	buttons->joinPartyBtn.max_y = buttons->validateBtn.max_y = 269 + height;
	buttons->exitBtn.min_y = buttons->returnBtn.min_y = 385;
	buttons->exitBtn.max_y = buttons->returnBtn.max_y = 385 + height;
}

void load_images(Images *images, SDL_Renderer *renderer)
{
	SDL_Surface *surface = IMG_Load("images/Bomberman_main_menu.jpg");
	images->mainMenuTex = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load("images/Bomberman_join_game.jpg");
	images->joinPartyMenuTex = SDL_CreateTextureFromSurface(renderer, surface);
}

int display_image(SDL_Renderer *Renderer, SDL_Texture *texture)
{
	SDL_Rect dest = {0, 0, 900, 780};

	if (texture == NULL || SDL_RenderCopy(Renderer, texture, NULL, &dest) != 0) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int mouse_bouton(int x, int y, Buttons buttons)
{
	int button = -1;

	if (x >= buttons.createPartyBtn.min_x && x <= buttons.createPartyBtn.max_x) {
		// Click on the "Join Party" button
		if (y >= buttons.joinPartyBtn.min_y && y <= buttons.joinPartyBtn.max_y)
			button = 0;
		// Click on the "Create Party" button
		else if (y >= buttons.createPartyBtn.min_y && y <= buttons.createPartyBtn.max_y)
			button = 1;
		// Click on the "exit" button
		else if (y >= buttons.exitBtn.min_y && y <= buttons.exitBtn.max_y)
			button = 2;
	}

	return button;
}

void free_images(Images *images)
{
	SDL_DestroyTexture(images->mainMenuTex);
	SDL_DestroyTexture(images->joinPartyMenuTex);
}
