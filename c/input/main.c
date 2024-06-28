#include <SDL2/SDL.h>
#include "input.h"

void DrawInput(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255 ,0);
	// HELD
	if (CHECK_HELD(BUTTON_UP)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 64, 16, 16}); }
	if (CHECK_HELD(BUTTON_DOWN)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 96, 16, 16}); }
	if (CHECK_HELD(BUTTON_LEFT)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 128, 16, 16}); }
	if (CHECK_HELD(BUTTON_RIGHT)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 160, 16, 16}); }
	if (CHECK_HELD(BUTTON_A)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 192, 16, 16}); }
	if (CHECK_HELD(BUTTON_B)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 224, 16, 16}); }
	if (CHECK_HELD(BUTTON_C)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 256, 16, 16}); }
	if (CHECK_HELD(BUTTON_D)) { SDL_RenderFillRect(renderer, &(SDL_Rect){140, 288, 16, 16}); }

	// PRESSED
	if (CHECK_PRESSED(BUTTON_UP)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 64, 16, 16}); }
	if (CHECK_PRESSED(BUTTON_DOWN)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 96, 16, 16}); }
	if (CHECK_PRESSED(BUTTON_LEFT)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 128, 16, 16}); }
	if (CHECK_PRESSED(BUTTON_RIGHT)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 160, 16, 16}); }
	if (CHECK_PRESSED(BUTTON_A)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 192, 16, 16}); }
	if (CHECK_PRESSED(BUTTON_B)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 224, 16, 16}); }
	if (CHECK_PRESSED(BUTTON_C)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 256, 16, 16}); }
	if (CHECK_PRESSED(BUTTON_D)) { SDL_RenderFillRect(renderer, &(SDL_Rect){220, 288, 16, 16}); }

	// RELEASED
	if (CHECK_RELEASED(BUTTON_UP)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 64, 16, 16}); }
	if (CHECK_RELEASED(BUTTON_DOWN)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 96, 16, 16}); }
	if (CHECK_RELEASED(BUTTON_LEFT)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 128, 16, 16}); }
	if (CHECK_RELEASED(BUTTON_RIGHT)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 160, 16, 16}); }
	if (CHECK_RELEASED(BUTTON_A)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 192, 16, 16}); }
	if (CHECK_RELEASED(BUTTON_B)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 224, 16, 16}); }
	if (CHECK_RELEASED(BUTTON_C)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 256, 16, 16}); }
	if (CHECK_RELEASED(BUTTON_D)) { SDL_RenderFillRect(renderer, &(SDL_Rect){300, 288, 16, 16}); }
}


int main(int argc, char *argv[])
{
	// Variables
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	SDL_Texture *ui;
	int loop = 1;

	SDL_Rect player = {320, 220, 32, 32};

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error: Couldn't init. SDL. %s.\n", SDL_GetError());
		return 0;
	}

	window = SDL_CreateWindow("SDL2 Demo Input", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "Error: Couldn't create window. %s.\n", SDL_GetError());
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		fprintf(stderr, "Error: Couldn't create renderer. %s.\n", SDL_GetError());
		return 0;
	}

	// Load UI Image
	SDL_Surface *surface = SDL_LoadBMP("input.bmp");
	if (surface == NULL) {
		fprintf(stderr, "Error: Couldn't load image. %s.\n", SDL_GetError());
		return 0;
	}
	ui = SDL_CreateTextureFromSurface(renderer, surface);

	// Main Loop
	while(loop)
	{
		/***** Update *****/

		// SDL Events
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					loop = 0;
					break;
			}
		}

		// Update Input
		UpdateInput();

		// Update Player
		player.x += (CHECK_HELD(BUTTON_RIGHT) - CHECK_HELD(BUTTON_LEFT)) * 5;
		player.y += (CHECK_HELD(BUTTON_DOWN) - CHECK_HELD(BUTTON_UP)) * 5;

		if (player.x < 0) { player.x = 0; }
		if (player.x > 640 - 32) { player.x = 640 - 32; }
		if (player.y < 0) { player.y = 0; }
		if (player.y > 480 - 32) { player.y = 480 - 32; }

		/***** Rendering *****/

		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0 ,0);
		SDL_RenderClear(renderer);

		// Draw Player
		SDL_SetRenderDrawColor(renderer, 0, 0x80, 0 ,0);
		SDL_RenderFillRect(renderer, &player);

		// Draw UI
		DrawInput(renderer);
		SDL_RenderCopy(renderer, ui, NULL, NULL);

		SDL_RenderPresent(renderer);
	}

	// Close SDL
	SDL_DestroyTexture(ui);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}


