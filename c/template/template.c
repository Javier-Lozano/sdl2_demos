#include "SDL.h"

#define WINDOW_WIDTH  (800)
#define WINDOW_HEIGHT (600)

#define INIT_FLAGS   (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define RENDER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

static Uint8 SDLInit(SDL_Window **window, SDL_Renderer **renderer);
static void SDLClose(SDL_Window **window, SDL_Renderer **renderer);

int main( int argc, char **argv)
{
	SDL_Window   *window   = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event     event;
	Uint8 is_running = 1;

	if (!SDLInit(&window, &renderer))
	{
		return 0;
	}

	// Main Loop
	while(is_running)
	{
		// SDL Events
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) { is_running = 0; }
		}
		
		// SDL Rendering
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		// Draw Something Here
		int window_w;
		int window_h;
		SDL_GetWindowSize(window, &window_w, &window_h);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &(SDL_Rect){32, 32, window_w - 64, window_h - 64});

		SDL_RenderPresent(renderer);
	}

	SDLClose(&window, &renderer);
	printf("\nSEE YOU SPACE COWBOY\n");
	return 0;
}

static Uint8 SDLInit(SDL_Window **window, SDL_Renderer **renderer)
{
	if (SDL_Init(INIT_FLAGS) < 0)
	{
		fprintf(stderr, "Error: Couldn't init SDL. %s\n", SDL_GetError());
		return 0;
	}
	*window = SDL_CreateWindow("Sim(ple) Music Player SDL", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
	if (*window == NULL)
	{
		fprintf(stderr, "Error: Couldn't create window. %s\n", SDL_GetError());
		return 0;
	}
	*renderer = SDL_CreateRenderer(*window, -1, RENDER_FLAGS);
	if (*renderer == NULL)
	{
		fprintf(stderr, "Error: Couldn't create renderer. %s\n", SDL_GetError());
		return 0;
	}
	SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
	return 1;
}

static void SDLClose(SDL_Window **window, SDL_Renderer **renderer)
{
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	SDL_Quit();
}
