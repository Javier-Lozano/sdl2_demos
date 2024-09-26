#include "main.h"

bool SDLInit(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font)
{
	if (SDL_Init(INIT_FLAGS) < 0)
	{
		fprintf(stderr, "Error: Couldn't init SDL. %s\n", SDL_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Error: Couldn't init SDL_ttf. %s\n", TTF_GetError());
		return false;
	}

	// Create Window and Renderer
	*window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
	if (*window == NULL)
	{
		fprintf(stderr, "Error: Couldn't create window. %s\n", SDL_GetError());
		return false;
	}
	*renderer = SDL_CreateRenderer(*window, -1, RENDER_FLAGS);
	if (*renderer == NULL)
	{
		fprintf(stderr, "Error: Couldn't create renderer. %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

	// Load Font
	*font = TTF_OpenFont(FONT_PATH, 8);
	if (*font == NULL)
	{
		fprintf(stderr, "Error: Couldn't load TTF_Font. %s\n", TTF_GetError());
		return false;
	}
	
	return true;
}

void SDLClose(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font)
{
	// Close TTF
	TTF_CloseFont(*font);
	TTF_Quit();

	// Close SDL2
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	SDL_Quit();
}
