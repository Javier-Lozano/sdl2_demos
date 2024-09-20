#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

/***** Macros *****/

// Keyboard
#define CHECK_KEY_PRESSED(key)  ( InputCheckScancode(SDL_GetScancodeFromKey(key), STATE_PRESSED)  )
#define CHECK_KEY_RELEASED(key) ( InputCheckScancode(SDL_GetScancodeFromKey(key), STATE_RELEASED) )
#define CHECK_KEY_DOWN(key)     ( InputCheckScancode(SDL_GetScancodeFromKey(key), STATE_DOWN)     )

#define CHECK_SC_PRESSED(key)  ( InputCheckScancode(key, STATE_PRESSED)  )
#define CHECK_SC_RELEASED(key) ( InputCheckScancode(key, STATE_RELEASED) )
#define CHECK_SC_DOWN(key)     ( InputCheckScancode(key, STATE_DOWN)     )

// Mouse
#define CHECK_MOUSE_PRESSED(key)  ( InputCheckMouseButton(key, STATE_PRESSED)  )
#define CHECK_MOUSE_RELEASED(key) ( InputCheckMouseButton(key, STATE_RELEASED) )
#define CHECK_MOUSE_DOWN(key)     ( InputCheckMouseButton(key, STATE_DOWN)     )
#define GET_MOUSE_POSITION(x, y)  ( InputGetMousePosition(x, y) )
#define GET_MOUSE_DELTA(x, y)     ( SDL_GetRelativeMouseState(x, y) )

/***** Types *****/

typedef enum {
	STATE_DOWN     = 1,
	STATE_PRESSED  = 2,
	STATE_RELEASED = 4
} InputButtonState;

typedef enum {
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,
	MOUSE_BUTTON_COUNT
} InputMouseButton;

/***** Functions *****/

void InputUpdate();

bool InputCheckScancode(SDL_Scancode scancode, InputButtonState state);
bool InputCheckMouseButton(InputMouseButton mouse_button, InputButtonState state);
void InputGetMousePosition(int *x, int *y);

#endif // _INPUT_H_
