#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

/***** Macros *****/

// Keyboard
#define CHECK_KEY_PRESSED(key)  ( Input_CheckScancode(SDL_GetScancodeFromKey(key), BUTTON_STATE_PRESSED)  )
#define CHECK_KEY_RELEASED(key) ( Input_CheckScancode(SDL_GetScancodeFromKey(key), BUTTON_STATE_RELEASED) )
#define CHECK_KEY_DOWN(key)     ( Input_CheckScancode(SDL_GetScancodeFromKey(key), BUTTON_STATE_DOWN)     )

#define CHECK_SC_PRESSED(key)  ( Input_CheckScancode(key, BUTTON_STATE_PRESSED)  )
#define CHECK_SC_RELEASED(key) ( Input_CheckScancode(key, BUTTON_STATE_RELEASED) )
#define CHECK_SC_DOWN(key)     ( Input_CheckScancode(key, BUTTON_STATE_DOWN)     )

// Mouse
#define CHECK_MOUSE_PRESSED(key)  ( Input_CheckMouseButton(key, BUTTON_STATE_PRESSED)  )
#define CHECK_MOUSE_RELEASED(key) ( Input_CheckMouseButton(key, BUTTON_STATE_RELEASED) )
#define CHECK_MOUSE_DOWN(key)     ( Input_CheckMouseButton(key, BUTTON_STATE_DOWN)     )
#define GET_MOUSE_POSITION(x, y)  ( Input_GetMousePosition(x, y) )

/***** Types *****/

typedef enum {
	BUTTON_STATE_DOWN     = 1,
	BUTTON_STATE_PRESSED  = 2,
	BUTTON_STATE_RELEASED = 4
} Input_ButtonState;

typedef enum {
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_COUNT
} Input_MouseButton;

/***** Functions *****/

void Input_Update();

bool Input_CheckScancode(SDL_Scancode scancode, Input_ButtonState state);
bool Input_CheckMouseButton(Input_MouseButton mouse_button, Input_ButtonState state);
void Input_GetMousePosition(int *x, int *y);

#endif // _INPUT_H_
