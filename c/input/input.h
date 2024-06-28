#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

/***** Macros *****/

#define CHECK_PRESSED(key)  ( CheckInput(key, BUTTON_STATE_PRESSED)  )
#define CHECK_RELEASED(key) ( CheckInput(key, BUTTON_STATE_RELEASED) )
#define CHECK_HELD(key)     ( CheckInput(key, BUTTON_STATE_HELD)     )

/***** Types *****/

typedef enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_C,
	BUTTON_D,
	BUTTON_COUNT
} Button;

typedef enum {
	BUTTON_STATE_NONE     = 0,
	BUTTON_STATE_PRESSED  = 1,
	BUTTON_STATE_RELEASED = 2,
	BUTTON_STATE_HELD     = 4
} ButtonState;

/***** Functions *****/

void UpdateInput();
int CheckInput(Button button, ButtonState state);

#endif // _INPUT_H_
