#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

/***** Macros *****/

// Keyboard
#define CHECK_SC_PRESSED(scancode)  ( Input_CheckScancode(scancode, BUTTON_STATE_PRESSED)  )
#define CHECK_SC_RELEASED(scancode) ( Input_CheckScancode(scancode, BUTTON_STATE_RELEASED) )
#define CHECK_SC_DOWN(scancode)     ( Input_CheckScancode(scancode, BUTTON_STATE_DOWN)     )

#define CHECK_KEY_PRESSED(key)  ( CHECK_SC_PRESSED(SDL_GetScancodeFromKey(key))  )
#define CHECK_KEY_RELEASED(key) ( CHECK_SC_RELEASED(SDL_GetScancodeFromKey(key)) )
#define CHECK_KEY_DOWN(key)     ( CHECK_SC_DOWN(SDL_GetScancodeFromKey(key))     )

// Mouse
#define CHECK_MOUSE_PRESSED(button)  ( Input_CheckMouseButton(button, BUTTON_STATE_PRESSED)  )
#define CHECK_MOUSE_RELEASED(button) ( Input_CheckMouseButton(button, BUTTON_STATE_RELEASED) )
#define CHECK_MOUSE_DOWN(button)     ( Input_CheckMouseButton(button, BUTTON_STATE_DOWN)     )

#define GET_MOUSE_POSITION(x, y)  ( Input_GetMousePosition(x, y, NULL, NULL) )
#define GET_MOUSE_DELTA(x, y)     ( Input_GetMousePosition(NULL, NULL, x, y) )

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
void Input_GetMousePosition(int *x, int *y, int *dx, int *dy);

#endif // _INPUT_H_
