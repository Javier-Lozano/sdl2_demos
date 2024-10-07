#include "common.h"

// Collision Check

bool CollisionPointCircle(int x1, int y1, int x2, int y2, double radius)
{
	return (SDL_pow(x1 - x2, 2) + SDL_pow(y1 - y2, 2) < radius * radius);
}

bool CollisionPointRect(int x, int y, SDL_Rect *rect)
{
	return (rect != NULL &&
			x > rect->x && x < rect->x + rect->w &&
			y > rect->y && y < rect->y + rect->h);
}

// Rendering

int SetRenderDrawColor(SDL_Renderer *renderer, Uint32 color)
{
	return SDL_SetRenderDrawColor(renderer,
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >> 8 ) & 0xFF,
			color & 0xFF);
}

int ModulateTextureRGBA(SDL_Texture *texture, Uint32 color)
{
	int result = 0;

	// Color Modulation
	if (SDL_SetTextureColorMod(texture, (color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8 ) & 0xFF) < 0)
	{
		fprintf(stderr, "Error: Couldn't modulate texture's color. %s.", TTF_GetError());
		result = -1;
	}
	// Alpha Modulation
	if (SDL_SetTextureAlphaMod(texture, color & 0xFF))
	{
		fprintf(stderr, "Error: Couldn't modulate texture's alpha. %s.", TTF_GetError());
		result = -1;
	}

	return result;
}

// Linear Interpolation

double Lerp(double a, double b, double t)
{
	return ((1 - t) * a) + (t * b);
}

// Uint32 - Color conversion

SDL_Color Uint32_to_Color(Uint32 color)
{
	return (SDL_Color) {
		.r = (color & 0xFF000000) >> 24,
		.g = (color & 0x00FF0000) >> 16,
		.b = (color & 0x0000FF00) >> 8,
		.a = (color & 0x000000FF)
	};
}

Uint32 Color_to_Uint32(SDL_Color color)
{
	return (Uint32)(color.r << 24 | color.g << 16 | color.b << 8 | color.a);
}

// HSV - Color Conversion

SDL_Color HSV_to_RGBA(HSV color)
{
	SDL_Color result;
	float r1, g1, b1;
	float C, H, X, d;

	// Chroma
	C = color.s * color.v;
	// Hue
	H = SDL_fmodf(color.h, 360) / 60.0f;
	// X intermediate value
	X = C * (1 - SDL_fabs(SDL_fmodf(H, 2) - 1));
	// Diffenrece between Value and Chroma
	d = color.v - C;

	switch((int)H)
	{
		case 0: r1 = (C + d) * 255; g1 = (X + d) * 255; b1 = d * 255; break;
		case 1: r1 = (X + d) * 255; g1 = (C + d) * 255; b1 = d * 255; break;
		case 2: r1 = d * 255; g1 = (C + d) * 255; b1 = (X + d) * 255; break;
		case 3: r1 = d * 255; g1 = (X + d) * 255; b1 = (C + d) * 255; break;
		case 4: r1 = (X + d) * 255; g1 = d * 255; b1 = (C + d) * 255; break;
		case 5: r1 = (C + d) * 255; g1 = d * 255; b1 = (X + d) * 255; break;
	}

	result.r = SDL_roundf(r1);
	result.g = SDL_roundf(g1);
	result.b = SDL_roundf(b1);
	result.a = color.a * 255;
	return result;
}

HSV RGBA_to_HSV(SDL_Color color)
{
	HSV result = {0};
	float r, g, b, a;
	float max, min;

	r = (float)color.r / 255.0f;
	g = (float)color.g / 255.0f;
	b = (float)color.b / 255.0f;
	a = (float)color.a / 255.0f;

	max = MAX(MAX(r,g),b);
	min = MIN(MIN(r,g),b);

	// Hue
	if (max == min)
	{
		result.h = 0;
	}
	else
	{
		if (max == r) {
			float f = (g - b) / (max - min);
			result.h = 60.0f * f;
		}
		else if (max == g)
		{
			float f = (b - r) / (max - min);
			result.h = (60.0f * f) + 120.0f;
		}
		else if (max == b)
		{
			float f = (r - g) / (max - min);
			result.h = (60.0f * f) + 240.0f;
		}
	}
	// Saturation
	if (max != 0) { result.s = 1 - (min / max); }
	// Value
	result.v = max;
	// Alpha
	result.a = a;

	return result;
}

HSV Uint32_to_HSV(Uint32 color)
{
	return RGBA_to_HSV(Uint32_to_Color(color));
}

Uint32 HSV_to_Uint32(HSV color)
{
	return Color_to_Uint32(HSV_to_RGBA(color));
}
