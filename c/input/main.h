#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdbool.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "input.h"

/***** MACROS *****/

#define WINDOW_TITLE  "Input"
#define WINDOW_WIDTH  (800)
#define WINDOW_HEIGHT (600)
#define INIT_FLAGS   (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define RENDER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
#define FONT_PATH    ("../../assets/ModernDOS8x16.ttf")

/***** Functions *****/

bool SDLInit(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);
void SDLClose(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font);

#endif // _MAIN_H_
