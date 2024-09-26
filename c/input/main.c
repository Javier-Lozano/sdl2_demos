#include "main.h"

/***** Types *****/

typedef struct polygon_st {
	SDL_Point position;
	float     radius;
	float     angle;
	float     speed;
	int       sides;
	bool      is_held;
	bool      is_colliding;
} Polygon;

/***** Functions *****/

static void RenderText(SDL_Renderer *renderer, TTF_Font *font, int font_size, char *text, int x, int y, SDL_Color color);
static bool PointCircleCollison(int x1, int y1, int x2, int y2, float radius);
static void PolygonUpdate(Polygon *polygon);
static void PolygonDraw(SDL_Renderer *renderer, TTF_Font *font, Polygon *polygon);

/***** Main *****/

int main( int argc, char **argv)
{
	SDL_Window   *window   = NULL;
	SDL_Renderer *renderer = NULL;
	TTF_Font     *font     = NULL;
	SDL_Event     event;
	bool          is_running = true;

	Polygon polygon = {
		.position = { .x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2 },
		.radius = 100.0f,
		.sides  = 6,
		.speed  = 0.01f
	};

	if (SDLInit(&window, &renderer, &font))
	{
		while(is_running)
		{
			// Events
			while(SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT) { is_running = 0; }
			}

			// Update Input
			InputUpdate();

			// Update Polygon
			PolygonUpdate(&polygon);

			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(renderer);

			// Draw Polygon
			PolygonDraw(renderer, font, &polygon);

			// Present to screen
			SDL_RenderPresent(renderer);
		}
	}

	SDLClose(&window, &renderer, &font);
	printf("\nSEE YOU SPACE COWBOY\n");
	return 0;
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

static void PolygonUpdate(Polygon *polygon)
{
	SDL_Point mouse_position;
	SDL_Point mouse_delta;

	// Mouse Position
	GET_MOUSE_POSITION(&mouse_position.x, &mouse_position.y);
	GET_MOUSE_DELTA(&mouse_delta.x, &mouse_delta.y);

	// Check Mouse / Polygon Collision
	polygon->is_colliding = PointCircleCollison(
			mouse_position.x, mouse_position.y,
			polygon->position.x, polygon->position.y,
			polygon->radius);

	// Drag
	if ((polygon->is_colliding && CHECK_MOUSE_PRESSED(MOUSE_LEFT)) || polygon->is_held)
	{
		polygon->position.x += mouse_delta.x;
		polygon->position.y += mouse_delta.y;
		polygon->is_held = true;
	}
	if (CHECK_MOUSE_RELEASED(MOUSE_LEFT)) { polygon->is_held = false; }
	// Increase Radius
	if (CHECK_KEY_DOWN(SDLK_UP))   { polygon->radius += 2.5f; }
	if (CHECK_KEY_DOWN(SDLK_DOWN)) { polygon->radius -= 2.5f; }
	// Increase Rotation Speed
	if (CHECK_KEY_DOWN(SDLK_RIGHT)) { polygon->speed += 0.001f; }
	if (CHECK_KEY_DOWN(SDLK_LEFT))  { polygon->speed -= 0.001f; }
	// Increase Side number
	if (CHECK_KEY_PRESSED(SDLK_PAGEUP))   { polygon->sides += 1; }
	if (CHECK_KEY_PRESSED(SDLK_PAGEDOWN)) { polygon->sides -= 1; }
	if (polygon->sides < 3) { polygon->sides = 3; }
	// Increase Angle
	polygon->angle += polygon->speed; // Spin!
}

static void PolygonDraw(SDL_Renderer *renderer, TTF_Font *font, Polygon *polygon)
{
	const SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
	const SDL_Color red   = {0xFF, 0, 0, 0xFF};

	SDL_Color selected;
	int       window_h;
	char      buffer[64];

	selected = (polygon->is_colliding || polygon->is_held) ? red : white;
	SDL_SetRenderDrawColor(renderer, selected.r, selected.g, selected.b, selected.a);

	for (int i = 0; i < polygon->sides; i++)
	{
		const float pi_2    = 6.2831f;
		const float section = pi_2 / (float)polygon->sides;

		float x1 = (SDL_cos(polygon->angle + (section * i)) * polygon->radius) + polygon->position.x;
		float y1 = (SDL_sin(polygon->angle + (section * i)) * polygon->radius) + polygon->position.y;
		float x2 = (SDL_cos(polygon->angle + (section * (i + 1))) * polygon->radius) + polygon->position.x;
		float y2 = (SDL_sin(polygon->angle + (section * (i + 1))) * polygon->radius) + polygon->position.y;

		SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
		if (i == 0) { SDL_RenderDrawLineF(renderer, polygon->position.x, polygon->position.y, x1, y1); }
	}

	// Polygon Draw
	RenderText(renderer, font, 32, "[Mouse Left Button]", 16, 0, CHECK_MOUSE_DOWN(MOUSE_LEFT) ? red : white);
	RenderText(renderer, font, 32, "- Drags the polygon around.", 336, 0, white);

	// Increase / Decrease Radius
	RenderText(renderer, font, 32, "[Up/Down]", 176, 32, (CHECK_KEY_DOWN(SDLK_UP) || CHECK_KEY_DOWN(SDLK_DOWN)) ? red : white);
	RenderText(renderer, font, 32, "- Increase/Decrease Radius.", 336, 32, white);

	// Increase / Decrease Rotation Speed
	RenderText(renderer, font, 32, "[Left/Right]", 128, 64, (CHECK_KEY_DOWN(SDLK_LEFT) || CHECK_KEY_DOWN(SDLK_RIGHT)) ? red : white);
	RenderText(renderer, font, 32, "- Increase/Decrease Rotation.", 336, 64, white);

	// Increase / Decrease Sides
	RenderText(renderer, font, 32, "[Page Up/Page Down]", 16, 96, (CHECK_KEY_DOWN(SDLK_PAGEUP) || CHECK_KEY_DOWN(SDLK_PAGEDOWN)) ? red : white);
	RenderText(renderer, font, 32, "- Increase/Decrease Sides.", 336, 96, white);

	// Get Window Height
	SDL_GetWindowSize(SDL_RenderGetWindow(renderer), NULL, &window_h);

	// Draw values
	snprintf(buffer, 64, "%.2f", polygon->radius);
	RenderText(renderer, font, 32, "Radius:", 64, window_h - (16 * 8), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_h - (16 * 8), red);
	snprintf(buffer, 64, "%.2f", polygon->speed);
	RenderText(renderer, font, 32, "Speed:", 80, window_h - (16 * 6), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_h - (16 * 6), red);
	snprintf(buffer, 64, "%d", polygon->sides);
	RenderText(renderer, font, 32, "Sides:", 80, window_h - (16 * 4), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_h - (16 * 4), red);
	snprintf(buffer, 64, "(%d, %d)", polygon->position.x, polygon->position.y);
	RenderText(renderer, font, 32, "Position:", 32, window_h - (16 * 2), white);
	RenderText(renderer, font, 32, buffer, (16 * 12), window_h -(16 * 2), red);
}

