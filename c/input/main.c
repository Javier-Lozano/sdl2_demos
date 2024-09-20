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

static bool SDLInit(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);
static void SDLClose(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);

static void RenderText(SDL_Renderer *renderer, TTF_Font *font, int font_size, char *text, int x, int y, SDL_Color color);
static bool PointCircleCollison(int x1, int y1, int x2, int y2, float radius);

static void DrawPolygon(SDL_Renderer *renderer, TTF_Font *font);

int main( int argc, char **argv)
{
	SDL_Window   *window   = NULL;
	SDL_Renderer *renderer = NULL;
	TTF_Font     *font     = NULL;
	SDL_Event     event;
	bool is_running = true;

	// Init
	if (!SDLInit(&window, &renderer, &font)) { return 0; }

	// Main Loop
	while(is_running)
	{
		// SDL Events
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) { is_running = 0; }
		}

		// Update
		InputUpdate(&event);

		// Rendering
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		DrawPolygon(renderer, font);

		SDL_RenderPresent(renderer);
	}

	SDLClose(&window, &renderer, &font);
	printf("\nSEE YOU SPACE COWBOY\n");
	return 0;
}

static bool SDLInit(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font)
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
	*font = TTF_OpenFont("../../assets/ModernDOS8x8.ttf", 16);
	if (font == NULL)
	{
		fprintf(stderr, "Error: Couldn't load TTF_Font. %s\n", TTF_GetError());
		return false;
	}
	
	return true;
}

static void SDLClose(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font)
{
	// Close TTF
	TTF_CloseFont(*font);
	TTF_Quit();

	// Close SDL2
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	SDL_Quit();
}

static void RenderText(SDL_Renderer *renderer, TTF_Font *font, int font_size, char *text, int x, int y, SDL_Color color)
{
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;
	SDL_Rect     rect;

	// Font Size
	TTF_SetFontSize(font, font_size);

	// Create Surface
	surface = TTF_RenderText_Solid(font, text, color);
	if (surface == NULL)
	{
		fprintf(stderr, "Error: Couldn't render text. %s.", TTF_GetError());
		return;
	}

	// Get Dimensions
	rect.x = x;
	rect.y = y;
	rect.w = surface->w;
	rect.h = surface->h;

	// Create Texture
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL)
	{
		fprintf(stderr, "Error: Couldn't render text. %s.", SDL_GetError());
		SDL_FreeSurface(surface);
		return;
	}

	// Render Texture
	SDL_RenderCopy(renderer, texture, NULL, &rect);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

static bool PointCircleCollison(int x1, int y1, int x2, int y2, float radius)
{
	float c1 = SDL_powf(x1 - x2, 2);
	float c2 = SDL_powf(y1 - y2, 2);
	float distance = SDL_sqrtf(c1 + c2);
	return distance < SDL_abs(radius);
}

static void DrawPolygon(SDL_Renderer *renderer, TTF_Font *font)
{
	const float pi_2 = 6.2831f;

	static SDL_Point polygon_coords;
	static bool      polygon_snap;
	static float     polygon_angle;
	static float     polygon_radius = 100.0f;
	static int       polygon_sides  = 6;
	static float     angle_speed = 0.01f;

	SDL_Point window_size;
	SDL_Point center;
	SDL_Point polygon_pos; // Offseted Polygon Coords
	SDL_Point mouse_pos;
	SDL_Point mouse_dt;

	// Get Window's center
	SDL_GetWindowSize(SDL_RenderGetWindow(renderer), &window_size.x, &window_size.y);
	center.x = window_size.x / 2;
	center.y = window_size.y / 2;
	// Get Polygon center position
	polygon_pos.x = polygon_coords.x + center.x;
	polygon_pos.y = polygon_coords.y + center.y;
	// Mouse Position
	GET_MOUSE_POSITION(&mouse_pos.x, &mouse_pos.y);
	//  Mouse Delta
	GET_MOUSE_DELTA(&mouse_dt.x, &mouse_dt.y);

	// Check Collision
	bool collision = PointCircleCollison(mouse_pos.x, mouse_pos.y, polygon_pos.x, polygon_pos.y, polygon_radius);

	// Polygon Mechanics? (Controls)

	// Drag
	if ((collision && CHECK_MOUSE_PRESSED(MOUSE_LEFT)) || polygon_snap)
	{
		polygon_coords.x += mouse_dt.x;
		polygon_coords.y += mouse_dt.y;
		polygon_snap = true;
	}
	if (CHECK_MOUSE_RELEASED(MOUSE_LEFT)) { polygon_snap = false; }

	// Increase Radius
	if (CHECK_KEY_DOWN(SDLK_UP))   { polygon_radius += 2.5f; }
	if (CHECK_KEY_DOWN(SDLK_DOWN)) { polygon_radius -= 2.5f; }
	// Increase Rotation
	if (CHECK_KEY_DOWN(SDLK_RIGHT)) { angle_speed += 0.001f; }
	if (CHECK_KEY_DOWN(SDLK_LEFT))  { angle_speed -= 0.001f; }
	// Increase Side number
	if (CHECK_KEY_PRESSED(SDLK_PAGEUP))   { polygon_sides += 1; }
	if (CHECK_KEY_PRESSED(SDLK_PAGEDOWN)) { polygon_sides -= 1; }
	if (polygon_sides < 3) { polygon_sides = 3; }

	// Draw Polygon
	if (collision || polygon_snap)
		SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0 ,0xFF);
	else
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF ,0xFF);

	polygon_angle += angle_speed; // Spin!
	for (int i = 0; i < polygon_sides; i++)
	{
		const float section = pi_2 / (float)polygon_sides;
		const float angle = polygon_angle + (section * i);

		float x1 = (SDL_cos(angle) * polygon_radius) + polygon_pos.x;
		float y1 = (SDL_sin(angle) * polygon_radius) + polygon_pos.y;
		float x2 = (SDL_cos(angle + section) * polygon_radius) + polygon_pos.x;
		float y2 = (SDL_sin(angle + section) * polygon_radius) + polygon_pos.y;

		SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
		if (i == 0) { SDL_RenderDrawLineF(renderer, polygon_pos.x, polygon_pos.y, x1, y1); }
	}

	// Draw Text
	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
	SDL_Color red = {0xFF, 0, 0, 0xFF};
	char buffer[64];

	// Polygon Draw
	if (CHECK_MOUSE_DOWN(MOUSE_LEFT))
		RenderText(renderer, font, 32, "[MOUSE LEFT BUTTON]", 16, 16, red);
	else
		RenderText(renderer, font, 32, "[MOUSE LEFT BUTTON]", 16, 16, white);
	RenderText(renderer, font, 32, "- DRAGS THE POLYGON AROUND.", 336, 16, white);

	// Increase / Decrease Radius
	if (CHECK_KEY_DOWN(SDLK_UP) || CHECK_KEY_DOWN(SDLK_DOWN))
		RenderText(renderer, font, 32, "[UP/DOWN]", 176, (16 * 3), red);
	else
		RenderText(renderer, font, 32, "[UP/DOWN]", 176, (16 * 3), white);
	RenderText(renderer, font, 32, "- INCREASE/DECREASE RADIUS.", 336, (16 * 3), white);

	// Increase / Decrease Rotation Speed
	if (CHECK_KEY_DOWN(SDLK_LEFT) || CHECK_KEY_DOWN(SDLK_RIGHT))
		RenderText(renderer, font, 32, "[LEFT/RIGHT]", 128, (16 * 5), red);
	else
		RenderText(renderer, font, 32, "[LEFT/RIGHT]", 128, (16 * 5), white);
	RenderText(renderer, font, 32, "- INCREASE/DECREASE ROTATION.", 336, (16 * 5), white);

	// Increase / Decrease Sides
	if (CHECK_KEY_DOWN(SDLK_PAGEUP) || CHECK_KEY_DOWN(SDLK_PAGEDOWN))
		RenderText(renderer, font, 32, "[PAGE UP/PAGE DOWN]", 16, (16 * 7), red);
	else
		RenderText(renderer, font, 32, "[PAGE UP/PAGE DOWN]", 16, (16 * 7), white);
	RenderText(renderer, font, 32, "- INCREASE/DECREASE SIDES.", 336, (16 * 7), white);

	snprintf(buffer, 64, "%.2f", polygon_radius);
	RenderText(renderer, font, 32, "RADIUS:", 64, window_size.y - (16 * 8), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_size.y - (16 * 8), red);
	snprintf(buffer, 64, "%.2f", angle_speed);
	RenderText(renderer, font, 32, "SPEED:", 80, window_size.y - (16 * 6), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_size.y - (16 * 6), red);
	snprintf(buffer, 64, "%d", polygon_sides);
	RenderText(renderer, font, 32, "SIDES:", 80, window_size.y - (16 * 4), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_size.y - (16 * 4), red);
	snprintf(buffer, 64, "(%d, %d)", polygon_pos.x, polygon_pos.y);
	RenderText(renderer, font, 32, "POSITION:", 32, window_size.y - (16 * 2), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_size.y -(16 * 2), red);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 0xFF);
	SDL_RenderDrawLineF(renderer, mouse_pos.x, mouse_pos.y, mouse_pos.x - mouse_dt.x, mouse_pos.y - mouse_dt.y);
}


