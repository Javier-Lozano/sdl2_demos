#include "input.h"

/***** Globals  *****/

static SDL_Scancode g_Bindings[BUTTON_COUNT] = {
	SDL_SCANCODE_UP,
	SDL_SCANCODE_DOWN,
	SDL_SCANCODE_LEFT,
	SDL_SCANCODE_RIGHT,
	SDL_SCANCODE_Z,
	SDL_SCANCODE_X,
	SDL_SCANCODE_C,
	SDL_SCANCODE_V
};

static Uint8 g_States[BUTTON_COUNT];

/***** Functions ******/

void UpdateInput()
{
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < BUTTON_COUNT; i++)
	{
		Uint8 state = 0;

		if (keyboard[g_Bindings[i]])
		{
			state |= BUTTON_STATE_HELD;

			if (!CHECK_HELD(i))
			{
				state |= BUTTON_STATE_PRESSED;
			}
		}
		else
		{
			if (CHECK_HELD(i))
			{
				state |= BUTTON_STATE_RELEASED;
			}
		}

		g_States[i] = state;
	}
}

int CheckInput(Button button, ButtonState state)
{
	if (button < 0 || button > BUTTON_COUNT) { return 0; }

	return (g_States[button] & state) > 0;
}

