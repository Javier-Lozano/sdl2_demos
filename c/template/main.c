#include "main.h"

/***** Main *****/

int main( int argc, char **argv)
{
	SDL_Window   *window   = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event     event;
	bool          is_running = true;

	const float pi_2 = 6.2831f;

	SDL_FPoint p[3];
	SDL_Point  center;
	float      radius;
	float      angle = 0;

	if (SDLInit(&window, &renderer))
	{
		while(is_running)
		{
			// Events
			while(SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT) { is_running = 0; }
			}

			// Get Window's center
			SDL_GetWindowSize(window, &center.x, &center.y);
			center.x /= 2;
			center.y /= 2;

			// Set Radius to half height
			radius = (float)center.y;

			// SPIN!!!
			angle += 0.01f;
			if (angle > pi_2) { angle -= pi_2; }

			for(int i = 0; i < 3; i++)
			{
				p[i].x = (SDL_cos(angle + (pi_2 / 3 * (i + 1))) * radius) + center.x;
				p[i].y = (SDL_sin(angle + (pi_2 / 3 * (i + 1))) * radius) + center.y;
			}

			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(renderer);

			// Draw a spinning Triangle
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x80, 0xFF);
			SDL_RenderDrawLineF(renderer, center.x, center.y, p[0].x, p[0].y);
			SDL_RenderDrawLineF(renderer, p[0].x, p[0].y, p[1].x, p[1].y);
			SDL_RenderDrawLineF(renderer, p[1].x, p[1].y, p[2].x, p[2].y);
			SDL_RenderDrawLineF(renderer, p[2].x, p[2].y, p[0].x, p[0].y);

			// Present to screen
			SDL_RenderPresent(renderer);
		}
	}

	SDLClose(&window, &renderer);
	printf("\nSEE YOU SPACE COWBOY...\n");
	return 0;
}

