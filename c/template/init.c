#include "main.h"

bool SDLInit(SDL_Window **window, SDL_Renderer **renderer)
{
	// Init SDL2
	if (SDL_Init(INIT_FLAGS) < 0)
	{
		fprintf(stderr, "Error: Couldn't init SDL. %s\n", SDL_GetError());
		return false;
	}

	// Window and Renderer
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
	
	return true;
}

void SDLClose(SDL_Window **window, SDL_Renderer **renderer)
{
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	SDL_Quit();
}
