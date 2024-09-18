#include <stdbool.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "input.h"

#define WINDOW_TITLE  "Input"
#define WINDOW_WIDTH  (800)
#define WINDOW_HEIGHT (600)

#define INIT_FLAGS   (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define RENDER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)

static bool SDLInit(SDL_Window **window, SDL_Renderer **renderer);
static void SDLClose(SDL_Window **window, SDL_Renderer **renderer);
static void DrawPolygon(SDL_Renderer *renderer, int x, int y, int sides, int radius);
static void RenderText(SDL_Renderer *renderer, TTF_Font *font, int font_size, char *text, int x, int y, SDL_Color color);

int main( int argc, char **argv)
{
	SDL_Window   *window   = NULL;
	SDL_Renderer *renderer = NULL;
	TTF_Font     *font     = NULL;
	SDL_Event     event;
	bool is_running = true;

	if (!SDLInit(&window, &renderer))
		return 0;

	// Load Font
	font = TTF_OpenFont("../../assets/ModernDOS8x8.ttf", 16);

	int  polygon_x = 0;
	int  polygon_y = 0;
	int  sides     = 5;
	int  radius    = WINDOW_WIDTH / 4;
	bool collision = false;
	bool snap      = false;
	char buffer[128];

	// Main Loop
	while(is_running)
	{
		// SDL Events
		while(SDL_PollEvent(&event))
		{
			// Exit
			if (event.type == SDL_QUIT) { is_running = 0; }
		}
		// Input
		Input_Update(&event);

		// Update

		// Get Window Size
		int window_w;
		int window_h;
		SDL_GetWindowSize(window, &window_w, &window_h);

		// Get Mouse Position
		int mouse_x;
		int mouse_y;
		GET_MOUSE_POSITION(&mouse_x, &mouse_y);

		// Get Mouse Delta
		int mouse_dx;
		int mouse_dy;
		GET_MOUSE_DELTA(&mouse_dx, &mouse_dy);

		// This is a Circle Point Collision
		collision = (SDL_sqrt(
					SDL_pow(((window_w / 2) + polygon_x - mouse_x), 2) +
					SDL_pow(((window_h / 2) + polygon_y - mouse_y), 2)
					) < radius) ? true: false;

		// Increase / Decrease Polygon Radius
		if (CHECK_KEY_DOWN(SDLK_UP))   { radius += 1.0f; }
		if (CHECK_KEY_DOWN(SDLK_DOWN)) { radius -= 1.0f; }

		// Increase / Decrease Polygon Sides
		if (CHECK_KEY_PRESSED(SDLK_RIGHT)) { sides += 1; }
		if (CHECK_KEY_PRESSED(SDLK_LEFT))  { sides -= 1; }
		if (sides < 3)  { sides = 3;  }
		if (sides > 32) { sides = 32; }

		// Polygon movement
		if ((collision && CHECK_MOUSE_PRESSED(MOUSE_BUTTON_LEFT)) || snap)
		{
			polygon_x += mouse_dx;
			polygon_y += mouse_dy;
			snap = true;
		}
		// Reset Snap when Left button is released
		if (CHECK_MOUSE_RELEASED(MOUSE_BUTTON_LEFT))
			snap = false;

		// Rendering

		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		// Render Text
		SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};
		RenderText(renderer, font, 32, "[LEFT BUTTON] - DRAG THE POLYGON AROUND.", 8, 32, color);
		RenderText(renderer, font, 32, "    [UP/DOWN] - INCREASE OR DECREASE RADIUS.", 8, 64, color);
		RenderText(renderer, font, 32, " [LEFT/RIGHT] - INCREASE OR DECREASE SIDE NUMBER.", 8, 96, color);
		RenderText(renderer, font, 32, " SIDES:", 16, 128, color);
		RenderText(renderer, font, 32, "RADIUS:", 16, 160, color);

		color = (SDL_Color) {0xFF, 0, 0, 0xFF};
		sprintf(buffer, "% 3d", sides);
		RenderText(renderer, font, 32, buffer, 128, 128, color);
		sprintf(buffer, "%d", radius);
		RenderText(renderer, font, 32, buffer, 128, 160, color);

		// Mouse Cursor Tail
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 0xFF);
		SDL_RenderDrawLine(renderer, mouse_x, mouse_y, mouse_x - mouse_dx, mouse_y - mouse_dy);

		// Polygon
		if (collision || snap)
			SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		DrawPolygon(renderer, (window_w/2) + polygon_x, (window_h / 2) + polygon_y, sides, radius);

		// Present
		SDL_RenderPresent(renderer);
	}

	SDLClose(&window, &renderer);
	printf("\nSEE YOU SPACE COWBOY\n");
	return 0;
}

static bool SDLInit(SDL_Window **window, SDL_Renderer **renderer)
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

static void SDLClose(SDL_Window **window, SDL_Renderer **renderer)
{
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	TTF_Quit();
	SDL_Quit();
}

static void DrawPolygon(SDL_Renderer *renderer, int x, int y, int sides, int radius)
{
	const  double step = 6.2831 / (double)sides;
	static double angle;
	// Make it Spin!
	angle += 0.01f;

	for (int i = 0; i < sides; i++)
	{
		float x1 = (SDL_cos(angle) * (float)radius) + x;
		float y1 = (SDL_sin(angle) * (float)radius) + y;
		float x2 = (SDL_cos(angle + step) * (float)radius) + x;
		float y2 = (SDL_sin(angle + step) * (float)radius) + y;
		angle += step;
		SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
		if (i == 0) { SDL_RenderDrawLineF(renderer, x, y, x1, y1); }
	}
}

static void RenderText(SDL_Renderer *renderer, TTF_Font *font, int font_size, char *text, int x, int y, SDL_Color color)
{
	SDL_Texture *texture = NULL;
	SDL_Rect rect = { .x = x, .y = y };

	TTF_SetFontSize(font, font_size);

	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	rect.w = surface->w;
	rect.h = surface->h;
	SDL_RenderCopy(renderer, texture, NULL, &rect);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
