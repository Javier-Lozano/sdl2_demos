#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdbool.h>
#include "SDL.h"
#include "SDL_ttf.h"

/***** Macros *****/

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

/***** Types *****/

typedef struct hsv_st {
	float h, s, v, a;
} HSV;

/***** Function *****/

// Collision Check
bool CollisionPointCircle(int x1, int y1, int x2, int y2, double radius);
bool CollisionPointRect(int x, int y, SDL_Rect *rect);

// Linear Interpolation
double Lerp(double a, double b, double t);

// Rendering
int SetRenderDrawColor(SDL_Renderer *renderer, Uint32 color);
int ModulateTextureRGBA(SDL_Texture *texture, Uint32 color);

// Uint32 - Color conversion
SDL_Color Uint32_to_Color(Uint32 color);
Uint32 Color_to_Uint32(SDL_Color color);

// HSV - Color Conversion
SDL_Color HSV_to_RGBA(HSV color);
HSV RGBA_to_HSV(SDL_Color color);
HSV Uint32_to_HSV(Uint32 color);
Uint32 HSV_to_Uint32(HSV color);

#endif // _COMMON_H_
